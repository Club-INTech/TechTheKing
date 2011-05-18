#include "Interfaces.h"
#include "config.h"
#include "Debug.h"
#include "Constantes.h"
#ifdef DEBUG_GRAPHIQUE
#include <Magick++.h>
#endif
#include "Util.hpp"

using namespace std;

InterfaceAsservissement* InterfaceAsservissement::m_instance=NULL;

void InterfaceAsservissement::debugConsignes(){
	cout<<m_lastTrajectory<<endl;
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
   #ifdef DEBUG			
	cout<<"Tentative de déplacement du robot en : (x = " << arrivee.getX() << ", y = " << arrivee.getY() << ")" << endl;
   #endif
   Point depart(1000,1000);
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
		m_liaisonSerie<<"b0"+formaterInt(angleRadian*CONVERSION_RADIAN_TIC)<<endl;
	else
		m_liaisonSerie<<"b1"+formaterInt(angleRadian*CONVERSION_RADIAN_TIC)<<endl;
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
	cout<< "x : " << result<<endl;
	return result;
}

int InterfaceAsservissement::getYRobot()
{
	int result;
	m_liaisonSerie << "y" << endl ;
	m_liaisonSerie >> result;
	cout<< "y : " << result<<endl;
	return result;
}

unsigned char InterfaceActionneurs::pourcentageHauteurConversion(unsigned char pourcentage){
    return (pourcentage*2.55);
}


template <typename T>  stack<unsigned char> InterfaceActionneurs::decToBin(T dec){
    stack<unsigned char> res;
    for(int i=0;i<sizeof(T);i++){
    bitset<8> charTmp;
        for(int j=0;j<8;j++){
            charTmp.set(j,dec%2);
            dec=dec>>1;
        }
        unsigned char resTmp = (charTmp.to_ulong() << 8 * i);
        if(resTmp) res.push(resTmp);
    }
    return res;
}

InterfaceActionneurs::InterfaceActionneurs(){
}

unsigned int InterfaceActionneurs::pourcentageAngleConversion(unsigned char pourcentage){
    return(pourcentage*10.24);
}

void InterfaceCapteurs::thread(){
    while(1){
        //Tant que le capteur ne détecte pas d'obstacle
            //traiterAbsenceObstacle()
        //traiterPresenceObstacle()
    }   
}
