#include "Interfaces.h"
#include "config.h"
#include "Debug.h"
#include "i2cLib.h"
#include "Constantes.h"
#ifdef DEBUG_GRAPHIQUE
#include <Magick++.h>
#endif
#include "Util.hpp"

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
                case '1':
                    asserv->m_liaisonSerie.Open(stringTmp);
                    cout<<"Actionneurs : ok"<<endl;
                    break;
            }
        }
    }
}


void InterfaceAsservissement::goTo(Point arrivee,int nbPoints){
	int xDepart = getXRobot();
	int yDepart = getYRobot();
   #ifdef DEBUG			
	cout<<"Tentative de déplacement du robot de" << "( " << xDepart << ", " << yDepart << ")"
	 << "en : " << "(" << arrivee.getX() << ", " << arrivee.getY() << ")" << endl;
   #endif
   Point depart(xDepart,yDepart);
   vector<Point> listePointsTmp=m_pathfinding.getChemin(depart,arrivee);
   m_lastTrajectory=ListePoints::lissageBezier(listePointsTmp,nbPoints);
   m_lastListeConsignes=ListePoints::convertirEnConsignes(m_lastTrajectory,getDistanceRobot()); 
   ListeConsignes::transfertSerie(m_lastListeConsignes,m_liaisonSerie);
   unsigned char result;
   /*
   while(result != 'f'){
		m_liaisonSerie >> result;
   }*/
}

void InterfaceAsservissement::avancer(unsigned int distanceMm){
	m_liaisonSerie<<"b1"+formaterInt(distanceMm*CONVERSION_MM_TIC)<<endl;
}

void InterfaceAsservissement::reculer(unsigned int distanceMm){
	m_liaisonSerie<<"b0"+formaterInt(distanceMm*CONVERSION_MM_TIC)<<endl;
}

void InterfaceAsservissement::tourner(int angleRadian){
	if(angleRadian>0)
		m_liaisonSerie<<"a0"+formaterInt(angleRadian*CONVERSION_RADIAN_TIC)<<endl;
	else
		m_liaisonSerie<<"a1"+formaterInt(angleRadian*CONVERSION_RADIAN_TIC)<<endl;
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

int InterfaceAsservissement::getXRobot()
{
	int result;
	m_liaisonSerie << "x" << endl ;
	m_liaisonSerie >> result;
	return result;
}

int InterfaceAsservissement::getYRobot()
{
	int result;
	m_liaisonSerie << "y" << endl ;
	m_liaisonSerie >> result;
	return result;
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
    unsigned int tics = pourcentageHauteurConversion(pourcentageHauteur);
    unsigned char message[] = {0X41, (unsigned char) tics, (unsigned char) (tics >> 8), '\0'};
    
    i2c_write(adaptateur_i2c, 0X10, message, 4);
}


void InterfaceActionneurs::hauteurBrasDroit(unsigned char pourcentageHauteur)
{
    unsigned int tics = pourcentageHauteurConversion(pourcentageHauteur);
    unsigned char message[] = {0X42, (unsigned char) tics, (unsigned char) (tics >> 8), '\0'};
    
    i2c_write(adaptateur_i2c, 0X10, message, 4);
}


void InterfaceActionneurs::hauteurDeuxBras(unsigned char pourcentageHauteur)
{
    unsigned int tics = pourcentageHauteurConversion(pourcentageHauteur);
    unsigned char message[] = {0X4B, (unsigned char) tics, (unsigned char) (tics >> 8), '\0'};
    
    i2c_write(adaptateur_i2c, 0X10, message, 4);
}


void InterfaceActionneurs::angleBrasGauche(unsigned char pourcentageAngle)
{
    unsigned int angle = pourcentageAngleConversion(pourcentageAngle);
    unsigned char message[] = {0X11, (unsigned char) angle, (unsigned char) (angle >> 8), '\0'};
    
    i2c_write(adaptateur_i2c, 0X10, message, 4);
}


void InterfaceActionneurs::angleBrasDroit(unsigned char pourcentageAngle)
{
    unsigned int angle = pourcentageAngleConversion(pourcentageAngle);
    unsigned char message[] = {0X12, (unsigned char) angle, (unsigned char) (angle >> 8), '\0'};
    
    i2c_write(adaptateur_i2c, 0X10, message, 4);
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
    
    i2c_write(adaptateur_i2c, 0X10, message, 2);
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
    
    i2c_write(adaptateur_i2c, 0X10, message, 2);
}

void InterfaceActionneurs::recalage(void)
{
    i2c_write(adaptateur_i2c, 0XA1, message, 2);
}

unsigned int InterfaceActionneurs::pourcentageHauteurConversion(unsigned char pourcentage)
{
    return (pourcentage*90);
}


unsigned int InterfaceActionneurs::pourcentageAngleConversion(unsigned char pourcentage)
{
    return(pourcentage*6+200);
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
        //Tant que le capteur ne détecte pas d'obstacle
            //traiterAbsenceObstacle()
        //traiterPresenceObstacle()
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
