#include "Interfaces.h"
#include "config.h"
#include "i2cLib.h"
#include "Constantes.h"
#ifdef DEBUG_GRAPHIQUE
#include <Magick++.h>
#endif
#include "Util.hpp"
#include "Obstacles.h"
#include <ctime>

#define DEMI_LARGEUR_ROBOT 77
using namespace std;
Adaptator* adaptateur_i2c;


/*********************************************************/
/*   ASSERVISSEMENT                                      */
/*********************************************************/

InterfaceAsservissement* InterfaceAsservissement::m_instance=NULL;

void InterfaceAsservissement::debugConsignes(){
    cout<<m_lastListeConsignes<<endl;
}

#ifdef DEBUG_GRAPHIQUE
using namespace Magick;
void InterfaceAsservissement::debugGraphique(){
    cout<<"Conversion du chemin emprunté par le robot en graphique..."<<endl;
    Image image( "img/table.png" );
    
    /* Affiche les obstacles */
    for(unsigned int i=0;i<listeObstacles.size();i++){
        (listeObstacles[i].first)->draw(&image);
    }
    
    /* Affiche la courbe */
    image.strokeColor(Color(MaxRGB,MaxRGB,MaxRGB,MaxRGB/2));
    //image.strokeWidth(2*TAILLE_ROBOT);
    image.strokeWidth(15);
    for(unsigned int i=0;i<m_lastTrajectory.size()-1;i++)
        image.draw(DrawableLine((m_lastTrajectory[i].getX())*900/3000,
        630-m_lastTrajectory[i].getY()*630/2100,
        m_lastTrajectory[i+1].getX()*900/3000,
        630-m_lastTrajectory[i+1].getY()*630/2100));
    image.magick("png");
    std::string tmp("cheminRobot");
    image.write(tmp + numToString(m_compteurImages++));
    cout<<"chemin emprunté dans le robot écrit dans cheminRobot.png"<<endl;
}
#endif


InterfaceAsservissement* InterfaceAsservissement::Instance(){
    if(m_instance==NULL){
       #ifdef DEBUG
         cout<<"Création de l'interface d'asservissement"<<endl;
       #endif
       std::string port = "/dev/ttyUSB" + exec((char*)"ls -1 /dev/ttyUSB* | cut -d '/' -f 3 | sed -e 's/ttyUSB//'");
       m_instance= new InterfaceAsservissement(port.substr(0, port.size()-1), 50);
    }
    else{
      #ifdef DEBUG
         cout<<"Interface d'asservissement déjà crée " <<endl;
      #endif
    }
    return m_instance;
}

vector<char> getTtyUSB(){
    vector<char> portsOuverts;
    string stringTmp = exec((char*)"ls -1 /dev/ttyUSB* | cut -d '/' -f 3 | sed -e 's/ttyUSB//'");
    for(unsigned int i=0;i<stringTmp.length();i++){
        if(stringTmp[i+1]=='\n'){
            portsOuverts.push_back(stringTmp[i]);
        }
    }
    return portsOuverts;
}

string exec(char* cmd) {
    FILE* pipe = popen(cmd, "r");
    if (!pipe) return "";
    char buffer[128];
    string result = "";
    while(!feof(pipe)) {
        if(fgets(buffer, 128, pipe) != NULL)
                result += buffer;
    }
    pclose(pipe);
    return result;
}

void detectionSerieUsb(InterfaceAsservissement* asserv){
    vector<char> portsOuverts;
    SerialStream streamTmp;
    string stringTmp;
    char charTmp;
    streamTmp.SetBaudRate( SerialStreamBuf::BAUD_57600 ) ;
    streamTmp.SetCharSize( SerialStreamBuf::CHAR_SIZE_8 ) ;
    streamTmp.SetNumOfStopBits(1) ;
    string listePorts = exec((char*)"ls -1 /dev/ttyUSB* | cut -d '/' -f 3 | sed -e 's/ttyUSB//'");
    for(unsigned int i=0;i<=listePorts.length();i++){
        if(listePorts[i+1]=='\n'){
            stringTmp="/dev/ttyUSB";
            stringTmp.push_back( listePorts[i] );
            streamTmp.Open( stringTmp );
            streamTmp << "?" << endl;
            streamTmp >> charTmp ;
            switch(charTmp){
                case '0':
                    asserv->m_port=stringTmp;
                    cout<<"Asservissement : ok"<<endl;
                    break;
            }
        }
    }
}


void InterfaceAsservissement::ecrireSerie(std::string msg){
	m_serialPort.Write(msg);
}
void InterfaceAsservissement::goTo(Point arrivee,int nbPoints){
    m_lastArrivee = arrivee;
    m_lastNbPoints = nbPoints;
    int xDepart = getXRobot();
    int yDepart = getYRobot();
   #ifdef DEBUG         
    cout<<"Tentative de déplacement du robot de" << "( " << xDepart << ", " << yDepart << ")"
     << "en : " << "(" << arrivee.getX() << ", " << arrivee.getY() << ")" << endl;
   #endif
   Point depart(xDepart,yDepart);
   vector<Point> listePointsTmp;
   
   if( ListeObstacles::contientCercle(xDepart,yDepart,TAILLE_ROBOT,NOIR)!=NULL
	|| ListeObstacles::contientCercle(xDepart,yDepart,TAILLE_ROBOT,COULEUR_ROBOT)!=NULL
	|| ListeObstacles::contientCercle(xDepart,yDepart,TAILLE_ROBOT,NEUTRE)!=NULL ){
		#ifdef DEBUG
		std::cout << "Le robot croit qu'il est bloqué dans un obstacle ! Génération d'une lolconsigne aléatoire" << std::endl;
		#endif
		tourner(M_PI*rand()/(float)RAND_MAX);
		if(rand()/(float)RAND_MAX>0.5){
			avancer(300);
		}
		else{
			reculer(300);
	    }
		goTo(arrivee,nbPoints);
   }
   else
   {
	   listePointsTmp = m_pathfinding.getChemin(depart,arrivee);
	   if(!listePointsTmp.empty()){
			m_lastTrajectory=ListePoints::lissageBezier(listePointsTmp,nbPoints);
			m_lastListeConsignes=ListePoints::convertirEnConsignes(m_lastTrajectory,getDistanceRobot()); 
			ListeConsignes::transfertSerie(m_lastListeConsignes,m_serialPort);
			attendreArrivee();
	   }
	   else{
			stop();
	   }
   }
}

inline void InterfaceAsservissement::eviter(){
	stop();
	m_evitement=false;
	int xRobot =  CONVERSION_TIC_MM*getXRobot();
	int yRobot =  CONVERSION_TIC_MM*getYRobot();
	double angleRobot = CONVERSION_TIC_RADIAN*getAngleRobot();
	double distanceUltraSon = TAILLE_ROBOT+InterfaceCapteurs::Instance()->distanceDernierObstacle()*CONVERSION_ULTRASONS_MM;
	
	//TODO : Dépend de la couleur du robot.
	double offsetX = cos(angleRobot)*distanceUltraSon;
	double offsetY = sin(angleRobot)*distanceUltraSon;
	
	std::cout << "xRobot : " << xRobot << std::endl;
	std::cout << "yRobot : " << yRobot << std::endl;
	
	std::cout << "Offset x : " << offsetX << std::endl;
	std::cout << "Offset y : " << offsetY << std::endl;
	
	RobotAdverse::Instance()->setCoords(
		xRobot-offsetX,
		yRobot-offsetY);
	
	reculer(distanceUltraSon);
	reGoTo();
}
void InterfaceAsservissement::attendreArrivee(){
	std::string result;
	while(result[0]!='f'){
		while(!m_serialPort.IsDataAvailable()){
			if(m_evitement==true){
				std::cout << "Obstacle détecté" << std::endl;
				eviter();
				return;
			}
		}
		result=m_serialPort.ReadLine();
	}
	sleep(1);
}
void InterfaceAsservissement::reGoTo(){
    goTo(m_lastArrivee,m_lastNbPoints);
}

void InterfaceAsservissement::avancer(unsigned int distanceMm){
	int distanceTicks = getDistanceRobot() + distanceMm*CONVERSION_MM_TIC;
	#ifdef DEBUG
		std::cout << "Avance de " << distanceTicks << " ticks." << std::endl;
	#endif
    if(distanceTicks>0){
		ecrireSerie("b1"+formaterInt(distanceTicks));
	}
	else{
		ecrireSerie("b0"+formaterInt(-distanceTicks));
	}
    attendreArrivee();
}

void InterfaceAsservissement::reculer(unsigned int distanceMm){
    int distanceTicks = getDistanceRobot() - distanceMm*CONVERSION_MM_TIC;
    #ifdef DEBUG
		std::cout << "Recule de " << distanceTicks << " ticks" << std::endl;
	#endif
    if(distanceTicks>0){
		ecrireSerie("b1"+formaterInt(distanceTicks));
	}
	else{
		ecrireSerie("b0"+formaterInt(-distanceTicks));
	}
    attendreArrivee();
}

void InterfaceAsservissement::tourner(double angleRadian){
	int angleTicks = angleRadian*CONVERSION_RADIAN_TIC;
	#ifdef DEBUG
		std::cout << "Tourne de " << angleTicks << std::endl;
	#endif
    if(angleRadian>0)
        ecrireSerie("a0"+formaterInt(angleTicks));
    else
        ecrireSerie("a1"+formaterInt(-angleTicks));
    attendreArrivee();
}

InterfaceAsservissement::InterfaceAsservissement(std::string port, int precision) :m_serialPort(port), m_evitement(false), m_compteurImages(0), m_pathfinding(precision){
    m_serialPort.Open();
    #ifdef DEBUG
      cout<<"Interface crée"<<endl;
      
    #endif
}

void InterfaceAsservissement::recalage()
{
	pwmMaxTranslation(100);
	pwmMaxRotation(0);
	reculer(500);
	pwmMaxRotation(255);
	if(COULEUR_ROBOT==BLEU){
		setXRobot(3000-DEMI_LARGEUR_ROBOT);
	}
	else if(COULEUR_ROBOT==ROUGE){
		setXRobot(DEMI_LARGEUR_ROBOT);
	}
	avancer(150);
	tourner(-M_PI/2);
	pwmMaxRotation(0);
	pwmMaxTranslation(100);
	reculer(500);
	pwmMaxRotation(255);
	setYRobot(2100-DEMI_LARGEUR_ROBOT);
	avancer(100);
}

void InterfaceAsservissement::pwmMaxTranslation(unsigned char valPWM){
	ecrireSerie("pt0"+formaterInt(valPWM));
}

void InterfaceAsservissement::pwmMaxRotation(unsigned char valPWM){
	ecrireSerie("pr0"+formaterInt(valPWM));
}


InterfaceAsservissement::~InterfaceAsservissement()
{
    m_serialPort.Close();
}

int InterfaceAsservissement::getDistanceRobot()
{
	ecrireSerie("t");
	return readInt();
}

void InterfaceAsservissement::setEvitement(){	
	boost::mutex::scoped_lock lolilol_evitement(m_evitement_mutex);
	m_evitement=true;
}
int InterfaceAsservissement::getAngleRobot(){
	ecrireSerie("u");
	return readInt();
}

int InterfaceAsservissement::readInt()
{
	while(!m_serialPort.IsDataAvailable());
    return atoi(m_serialPort.ReadLine().c_str());
}
int InterfaceAsservissement::getXRobot()
{
    ecrireSerie("xg");
    return readInt();
}

void InterfaceAsservissement::setXRobot(int xMm){
	ecrireSerie("xs0" + formaterInt(xMm));
}

int InterfaceAsservissement::getYRobot()
{
    ecrireSerie("yg");
    return readInt();
}

void InterfaceAsservissement::setYRobot(int yMm){
    ecrireSerie("ys0"+formaterInt(yMm));
}

void InterfaceAsservissement::stop()
{
	ecrireSerie("s");
}

void InterfaceAsservissement::stopAll()
{
	stop();
	pwmMaxTranslation(0);
	pwmMaxRotation(0);
}

/*********************************************************/
/*   Etranger, ici commence la terre des actionneurs...  */
/*           Fait attention ou tu marches                */
/*********************************************************/


InterfaceActionneurs::InterfaceActionneurs() 
{
}


InterfaceActionneurs::~InterfaceActionneurs() 
{
}


void InterfaceActionneurs::hauteurBrasGauche(Niveau Hauteur)
{
    unsigned char message[2];
    
    if (Hauteur == CAPTURE)
        message[0] = 0X41;
    else if (Hauteur == MILIEU)
        message[0] = 0X43;
    else if (Hauteur == TOUR)
        message[0] = 0X45;
    else if (Hauteur == SOCLE)
        message[0] = 0X47;
    
    message[1] = '\0';
    
    i2c_write(adaptateur_i2c, 0X10, message, 2);
}


void InterfaceActionneurs::hauteurBrasDroit(Niveau Hauteur)
{
    unsigned char message[2];
    
    if (Hauteur == CAPTURE)
        message[0] = 0X42;
    else if (Hauteur == MILIEU)
        message[0] = 0X44;
    else if (Hauteur == TOUR)
        message[0] = 0X46;
    else if (Hauteur == SOCLE)
        message[0] = 0X48;
    
    message[1] = '\0';
    
    i2c_write(adaptateur_i2c, 0X10, message, 2);
}


void InterfaceActionneurs::hauteurDeuxBras(Niveau Hauteur)
{
    unsigned char message[2];
    
    if (Hauteur == CAPTURE)
        message[0] = 0X51;
    else if (Hauteur == MILIEU)
        message[0] = 0X52;
    else if (Hauteur == TOUR)
        message[0] = 0X53;
    else if (Hauteur == SOCLE)
        message[0] = 0X54;
    
    message[1] = '\0';
    
    i2c_write(adaptateur_i2c, 0X10, message, 2);
}


void InterfaceActionneurs::angleBrasGauche(Orientation Angle)
{   
    unsigned char message[2];
    
    if (Angle == REPLIE)
        message[0] = 0X11;
    else if (Angle == CENTRE)
        message[0] = 0X13;
    else if (Angle == BALAYAGE)
        message[0] = 0X15;
    else if (Angle == DROIT)
        message[0] = 0X17;
    
    message[1] = '\0';
    
    i2c_write(adaptateur_i2c, 0X10, message, 2);
}


void InterfaceActionneurs::angleBrasDroit(Orientation Angle)
{
    unsigned char message[2];
    
    if (Angle == REPLIE)
        message[0] = 0X12;
    else if (Angle == CENTRE)
        message[0] = 0X14;
    else if (Angle == BALAYAGE)
        message[0] = 0X16;
    else if (Angle == DROIT)
        message[0] = 0X18;
    
    message[1] = '\0';
    
    i2c_write(adaptateur_i2c, 0X10, message, 2);
}


void InterfaceActionneurs::positionAimantGauche(ModeAimant mode)
{
    unsigned char message[2];
    
    if (mode == HAUT)
        message[0] = 0X21;
    else if (mode == BAS)
        message[0] = 0X31;
    
    message[1] = '\0';
    
    i2c_write(adaptateur_i2c, 0X10, message, 2);
}


void InterfaceActionneurs::positionAimantDroit(ModeAimant mode)
{
    unsigned char message[2];
    
    if (mode == HAUT)
        message[0] = 0X22;
    else if (mode == BAS)
        message[0] = 0X32;
    
    message[1] = '\0';
    
    i2c_write(adaptateur_i2c, 0X10, message, 2);
}

void InterfaceActionneurs::arret(void)
{
    unsigned char message[2];
    
    message[0] = 0XA0;
    message[1] = '\0';
    
    i2c_write(adaptateur_i2c, 0X10, message, 2);
}

/*********************************************************/
/*    Voyageur, ici s'arrete la terre des actionneurs    */
/*         Que les vents te soient favorables            */
/*********************************************************/



/*********************************************************/
/*   CAPTEURS                                            */
/*********************************************************/

InterfaceCapteurs* InterfaceCapteurs::m_instance=NULL;

InterfaceCapteurs* InterfaceCapteurs::Instance(){
    if(m_instance==NULL){
       #ifdef DEBUG
         cout<<"Création de l'interface capteurs"<<endl;
       #endif
       m_instance= new InterfaceCapteurs();
    }
    else{
      #ifdef DEBUG
         cout<<"Interface capteurs déjà crée " <<endl;
      #endif
    }
    return m_instance;
}

InterfaceCapteurs::InterfaceCapteurs() : Thread()  
{
}


InterfaceCapteurs::~InterfaceCapteurs() 
{
}

void InterfaceCapteurs::thread(){
	InterfaceAsservissement* interfaceAsservissement=InterfaceAsservissement::Instance();
    while(1){	
        //Il y a quelquechose devant
        int distanceUltraSon = DistanceUltrason();
        if(distanceUltraSon>0 && distanceUltraSon < 7000) {
			#ifdef DEBUG
			std::cout << "OBSTACLE DÉTECTÉ" << std::endl;
			#endif
			{
			boost::mutex::scoped_lock locklilol(m_ultrason_mutex);
			m_distanceDernierObstacle = distanceUltraSon;
			}
			interfaceAsservissement->setEvitement();
			sleep(3);
			/*
            int xRobot =  CONVERSION_TIC_MM*interfaceAsservissement->getXRobot();
            int yRobot =  CONVERSION_TIC_MM*interfaceAsservissement->getYRobot();
            double angleRobot = CONVERSION_TIC_RADIAN*interfaceAsservissement->getAngleRobot();         
            //On actualise la position du robot adverse
            RobotAdverse::Instance()->setCoords(
            xRobot+cos(angleRobot)*distanceUltraSon*CONVERSION_ULTRASONS_CM,
            yRobot+sin(angleRobot)*distanceUltraSon*CONVERSION_ULTRASONS_CM);
            */
        }
        usleep(10000);
    }
}

unsigned short InterfaceCapteurs::distanceDernierObstacle( void ) 
{
	boost::mutex::scoped_lock locklilol(m_ultrason_mutex);
	return m_distanceDernierObstacle;
}
unsigned short InterfaceCapteurs::DistanceUltrason( void ) {
    
    
    
    unsigned char msg[2] = {0X11, '\0'};
    
    unsigned char rec[3];
    
    int err;

    if( (err= i2c_write(adaptateur_i2c,0X20,msg,2)) != 0){
        fprintf(stderr, "Error writing to the adapter: %s\n", linkm_error_msg(err));
        exit(1);
    }
    if( (err= i2c_read(adaptateur_i2c,0X20,rec,2)) != 0){
        fprintf(stderr, "Error reading from the adapter: %s\n", linkm_error_msg(err));
        exit(1);
    }
    
    unsigned short resu;
    unsigned short temp;
    
    resu = rec[0];
    temp = rec[1];
    resu += (temp << 8);
    
    return resu;
}


bool InterfaceCapteurs::EtatBras ( Bras val ) {
    
    unsigned char msg[2] = {val, '\0'};
    unsigned char rec[1];
    
    int err;
    
    if( (err= i2c_write(adaptateur_i2c,0X20,msg,2)) != 0){
        fprintf(stderr, "Error writing to the adapter: %s\n", linkm_error_msg(err));
        exit(1);
    }
    if( (err= i2c_read(adaptateur_i2c,0X20,rec,1)) != 0){
        fprintf(stderr, "Error reading from the adapter: %s\n", linkm_error_msg(err));
        exit(1);
    }
    
    return rec[0];
}


char InterfaceCapteurs::LecteurCB ( void ) {

    unsigned char msg[2] = {0X30, '\0'};
    unsigned char rec[1];
    
    int err;
    
    if( (err= i2c_write(adaptateur_i2c,0X20,msg,2)) != 0){
        fprintf(stderr, "Error writing to the adapter: %s\n", linkm_error_msg(err));
        exit(1);
    }
    if( (err= i2c_read(adaptateur_i2c,0X20,rec,1)) != 0){
        fprintf(stderr, "Error reading from the adapter: %s\n", linkm_error_msg(err));
        exit(1);
    }
    
    return rec[0];
}
    

bool InterfaceCapteurs::EtatJumper ( void ) {
    
    unsigned char msg[2] = {0X50, '\0'};
    unsigned char rec[1];
    
    int err;
    
    if( (err= i2c_write(adaptateur_i2c,0X20,msg,2)) != 0){
        fprintf(stderr, "Error writing to the adapter: %s\n", linkm_error_msg(err));
        exit(1);
    }
    if( (err= i2c_read(adaptateur_i2c,0X20,rec,1)) != 0){
        fprintf(stderr, "Error reading from the adapter: %s\n", linkm_error_msg(err));
        exit(1);
    }
    
    return rec[0];
}


void InterfaceCapteurs::gestionJumper()
{
	#ifdef DEBUG
	std::cout << "Attente jumper" << std::endl;
    #endif
    while(EtatJumper()==false);
    std::cout << "Match commencé" << std::endl;
    boost::thread(&gestionFinMatch());
}

void InterfaceCapteurs::gestionFinMatch(){
	sleep(20);
	InterfaceAsservissement::Instance()->stopAll();
}


/*********************************************************/
/*  FIN CAPTEURS                                         */
/*********************************************************/

void ouvrir_adaptateur_i2c ()
{
    int err;
    if ( ( err = i2c_open( &adaptateur_i2c ) ) != 0 ) {
               fprintf(stderr, "Adaptateur pas branché : %s\n", linkm_error_msg(err));
    }
     
}
