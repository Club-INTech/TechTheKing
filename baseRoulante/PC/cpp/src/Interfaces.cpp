#include "Interfaces.h"
#include "config.h"
#include "Debug.h"
#include "i2cLib.h"
#include "Constantes.h"
#ifdef DEBUG_GRAPHIQUE
#include <Magick++.h>
#endif
#include "Util.hpp"
#include "Obstacles.h"
#include <ctime>

using namespace std;
Adaptator* adaptateur_i2c;


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
		listeObstacles[i]->draw(&image);
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

InterfaceAsservissement* InterfaceAsservissement::Instance(int precisionAStar){
    if(m_instance==NULL){
       #ifdef DEBUG
         cout<<"Création de l'interface d'asservissement"<<endl;
       #endif
       m_instance= new InterfaceAsservissement(precisionAStar);
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
    for(unsigned int i=0;i<listePorts.length();i++){
        if(listePorts[i+1]=='\n'){
            stringTmp="/dev/ttyUSB";
            stringTmp.push_back( listePorts[i] );
            streamTmp.Open( stringTmp );
            streamTmp << "?" << endl;
            streamTmp >> charTmp ;
            cout<<charTmp<<endl;
            switch(charTmp){
                case '0':
                    asserv->m_liaisonSerie.Open(stringTmp);
                    cout<<"Asservissement : ok"<<endl;
                    break;
            }
        }
    }
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
   listePointsTmp = m_pathfinding.getChemin(depart,arrivee);
   if(!listePointsTmp.empty()){
		m_lastTrajectory=ListePoints::lissageBezier(listePointsTmp,nbPoints);
		m_lastListeConsignes=ListePoints::convertirEnConsignes(m_lastTrajectory,getDistanceRobot()); 
		ListeConsignes::transfertSerie(m_lastListeConsignes,m_liaisonSerie);
		attendreArrivee();
   }
   else{
		
   }
   
}

void InterfaceAsservissement::attendreArrivee(){
	unsigned char result;	
	while(result != 'f'){
		m_liaisonSerie >> result;
	}
}
void InterfaceAsservissement::reGoTo(){
	goTo(m_lastArrivee,m_lastNbPoints);
}

void InterfaceAsservissement::avancer(unsigned int distanceMm){
	int distanceTicks = distanceMm*CONVERSION_MM_TIC;
	if(distanceTicks>0)
		m_liaisonSerie<<"b1"+formaterInt(distanceMm*CONVERSION_MM_TIC)<<endl;
	else
		m_liaisonSerie<<"b0"+formaterInt(-distanceMm*CONVERSION_MM_TIC)<<endl;
	attendreArrivee();
}

void InterfaceAsservissement::reculer(unsigned int distanceMm){
	int distanceTicks = distanceMm*CONVERSION_MM_TIC;
	if(distanceTicks>0)
		m_liaisonSerie<<"b1"+formaterInt(distanceMm*CONVERSION_MM_TIC)<<endl;
	else
		m_liaisonSerie<<"b0"+formaterInt(-distanceMm*CONVERSION_MM_TIC)<<endl;
	attendreArrivee();
}

void InterfaceAsservissement::tourner(double angleRadian){
	if(angleRadian>0)
		m_liaisonSerie<<"a0"+formaterInt(angleRadian*CONVERSION_RADIAN_TIC)<<endl;
	else
		m_liaisonSerie<<"a1"+formaterInt(-angleRadian*CONVERSION_RADIAN_TIC)<<endl;
	attendreArrivee();
}

InterfaceAsservissement::InterfaceAsservissement(int precision) : m_compteurImages(0), m_pathfinding(precision){
    detectionSerieUsb(this);
    m_liaisonSerie.SetBaudRate(SerialStreamBuf::BAUD_57600);
    m_liaisonSerie.SetNumOfStopBits(1);
    m_liaisonSerie.SetParity( SerialStreamBuf::PARITY_ODD ) ;
    #ifdef DEBUG
      cout<<"Interface crée"<<endl;
      
    #endif
}

void InterfaceAsservissement::recalage()
{
	reculer(500);
	avancer(500);
	if(COULEUR_ROBOT==BLEU){
		setXRobot(80);
	}
	else if(COULEUR_ROBOT==ROUGE){
		setXRobot(2920);
	}
	tourner(-M_PI/2);
	reculer(500);
	setYRobot(2020);
}

InterfaceAsservissement::~InterfaceAsservissement()
{
	m_liaisonSerie.Close();
}

int InterfaceAsservissement::getDistanceRobot()
{
	int result;
	m_liaisonSerie << "t" << endl ;
	m_liaisonSerie >> result;
	cout<< "ticks : " << result<<endl;
	return result;
}

int InterfaceAsservissement::getAngleRobot(){
	int result;
	m_liaisonSerie << "u" << endl ;
	m_liaisonSerie >> result;
	cout<< "ticks angle: " << result<<endl;
	return result;
}

int InterfaceAsservissement::getXRobot()
{
	int result;
	m_liaisonSerie << "xg";
	m_liaisonSerie >> result;
	return result;
}

void InterfaceAsservissement::setXRobot(int xMm){
	m_liaisonSerie << "xs0" << formaterInt(xMm) << endl;
}

int InterfaceAsservissement::getYRobot()
{
	int result;
	m_liaisonSerie << "yg";
	m_liaisonSerie >> result;
	return result;
}

void InterfaceAsservissement::setYRobot(int yMm){
	m_liaisonSerie << "ys0" << formaterInt(yMm) << endl;
}

void InterfaceAsservissement::stop()
{
	m_liaisonSerie << "s" ;
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


void InterfaceActionneurs::hauteurBrasGauche(unsigned char pourcentageHauteur)
{
    unsigned short tics = pourcentageHauteurConversion(pourcentageHauteur);
    unsigned char message[] = {0X41, (unsigned char) tics, (unsigned char) (tics >> 8),'\0'};
    
    i2c_write(adaptateur_i2c, 0X10, message, 3+1);
    
    usleep(i2c_wait);
}


void InterfaceActionneurs::hauteurBrasDroit(unsigned char pourcentageHauteur)
{
    unsigned short tics = pourcentageHauteurConversion(pourcentageHauteur);
    unsigned char message[] = {0X42, (unsigned char) tics, (unsigned char) (tics >> 8), '\0'};
    
    i2c_write(adaptateur_i2c, 0X10, message, 3+1);
    
    usleep(i2c_wait);
}


void InterfaceActionneurs::hauteurDeuxBras(unsigned char pourcentageHauteur)
{
    unsigned short tics = pourcentageHauteurConversion(pourcentageHauteur);
    unsigned char message[] = {0X4B, (unsigned char) tics, (unsigned char) (tics >> 8), '\0'};
    
    i2c_write(adaptateur_i2c, 0X10, message, 3+1);
    
    usleep(i2c_wait);
}


void InterfaceActionneurs::angleBrasGauche(unsigned char pourcentageAngle)
{   
    unsigned short angle = pourcentageAngleConversion(pourcentageAngle);
    
    unsigned char message[] = {0X11, (unsigned char) angle, (unsigned char) (angle >> 8), '\0'};
    
    i2c_write(adaptateur_i2c, 0X10, message, 3+1);
    
    usleep(i2c_wait);
}


void InterfaceActionneurs::angleBrasDroit(unsigned char pourcentageAngle)
{
    unsigned short angle = pourcentageAngleConversion(pourcentageAngle);
    unsigned char message[] = {0X12, (unsigned char) angle, (unsigned char) (angle >> 8), '\0'};
    
    i2c_write(adaptateur_i2c, 0X10, message, 3+1);
    
    usleep(i2c_wait);
}


void InterfaceActionneurs::positionAimantGauche(ModeAimant mode)
{
    unsigned char message[2];
    
    if (mode == HAUT) {
        message[0] = 0X21;
        message[1] = '\0';
    }
    else {
        message[0] = 0X31; 
        message[1] = '\0';
    }
    
    i2c_write(adaptateur_i2c, 0X10, message, 1+1);
    
    usleep(i2c_wait);
}


void InterfaceActionneurs::positionAimantDroit(ModeAimant mode)
{
    unsigned char message[2];
    
    if (mode == HAUT) {
        message[0] = 0X22;
        message[1] = '\0';
    }
    else {
        message[0] = 0X32; 
        message[1] = '\0';
    }
    
    i2c_write(adaptateur_i2c, 0X10, message, 1+1);
    
    usleep(i2c_wait);
}

void InterfaceActionneurs::recalage(void)
{
    unsigned char message[2];

    message[0] = 0XA1;
    message[1] = '\0';
    
    i2c_write(adaptateur_i2c, 0X10, message, 1+1);
    
    usleep(i2c_wait);
}

unsigned short InterfaceActionneurs::pourcentageHauteurConversion(unsigned char pourcentage)
{
    return (pourcentage*90);
}


unsigned short InterfaceActionneurs::pourcentageAngleConversion(unsigned char pourcentage)
{
    return(pourcentage*10.23);
}


/*********************************************************/
/*    Voyageur, ici s'arrete la terre des actionneurs    */
/*         Que les vents te soient favorables            */
/*********************************************************/



/*********************************************************/
/*   CAPTEURS                                            */
/*********************************************************/

void InterfaceCapteurs::thread(){
    while(1){
    	InterfaceAsservissement* interfaceAsservissement=InterfaceAsservissement::Instance();
    	//Il y a quelquechose devant
    	int distanceUltraSonG = DistanceUltrason(UGAUCHE);
    	int distanceUltraSonD = DistanceUltrason(UDROITE);
        if(distanceUltraSonG < 5000
        && distanceUltraSonD < 5000){
        	        	//On stop.
  				interfaceAsservissement->stop();
       	 	int distanceUltraSon = (distanceUltraSonG+distanceUltraSonD)/2;
      	  	int xRobot =  CONVERSION_TIC_MM*interfaceAsservissement->getXRobot();
      	  	int yRobot =  CONVERSION_TIC_MM*interfaceAsservissement->getYRobot();
    	    	double angleRobot = CONVERSION_TIC_RADIAN*interfaceAsservissement->getAngleRobot();        	
    	    	//On actualise la position du robot adverse
				RobotAdverse::Instance()->setCoords(
				xRobot+cos(angleRobot)*distanceUltraSon,
				yRobot+cos(angleRobot)*distanceUltraSon);
        		//On recule
				interfaceAsservissement->reculer(200);
				//On recalcule une trajectoire.
				interfaceAsservissement->reGoTo();
		}
		usleep(1000);
    }
}

unsigned short InterfaceCapteurs::DistanceUltrason( Ultrason val ) {
    
    unsigned char msg[3];
    msg[0] = val;
    
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

PresencePion InterfaceCapteurs::EtatBras ( FinCourse val ) {
    
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
    
    return ((PresencePion) rec[0]);
}

void InterfaceCapteurs::attendreJumper()
{
	unsigned char msg[2] = {0x50, '\0'};
	unsigned char rec[1];
	int err;
	while(rec[0]!=1){
		if( (err= i2c_write(adaptateur_i2c,0X20,msg,2)) != 0){
			fprintf(stderr, "Error writing to the adapter: %s\n", linkm_error_msg(err));
			exit(1);
		}
		if( (err= i2c_read(adaptateur_i2c,0X20,rec,1)) != 0){
			fprintf(stderr, "Error reading from the adapter: %s\n", linkm_error_msg(err));
			exit(1);
		}
		usleep(500);
    }
}

/*********************************************************/
/*  FIN CAPTEURS                                         */
/*********************************************************/

void ouvrir_adaptateur_i2c ()
{
    int err;
    
    if (err = i2c_open( &adaptateur_i2c ) != 0 ) {
               fprintf(stderr, "Adaptateur pas branché : %s\n", linkm_error_msg(err));
    }
     
}
