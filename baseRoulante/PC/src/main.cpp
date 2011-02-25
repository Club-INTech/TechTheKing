#include "config.h"
#include "Point.h"
#include "AStar.h"
#include "Consigne.h"
#include "Obstacles.h"
#include "Debug.h"
#include "Interfaces.h"
#include "Server.h"


std::vector <Obstacle*> listeObstacles ;
bool isObstacleDetected;

int main(){
    
    Socket::creer(57373);
    Socket::instance()->ouvrirThread();
	while(1){
		sleep(2);
	}
	
    /* Points de départ et d'arrivée */
    Point point1(CASE11);
    Point point2(CASE66);
    AStar test(50);
    std::vector<Point> listePoints;
    listePoints=ListePoints::lissageBezier(test.getChemin(point1,point2),200);
    vector<Consigne> listeConsignes = ListePoints::convertirEnConsignes(listePoints);
		
#ifdef DEBUG_GRAPHIQUE
    Debug::debugGraphique(listePoints);
#endif
		
//  Debug::debugConsignes(listeConsignes);
    
    
    /*Detection Interface Asservissement */
    //InterfaceAsservissement test;
    
    /* Envoi en série puis attente */
//  InterfaceAsservissement::creer();
//  detectionSerieUsb(InterfaceAsservissement::instance());
//  SerialStream& liaisonSerie = my_serial_stream;
//  for(unsigned int i=0;i<listeConsignes.size();i++)
//      listeConsignes[i].transfertSerie(liaisonSerie);
//  my_serial_stream << "e" << endl;
//  while(1);
    
}
