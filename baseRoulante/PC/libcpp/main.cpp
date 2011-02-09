#include "Point.h"
#include "AStar.h"
#include "Consigne.h"
#include "Obstacles.h"
#include "Debug.h"
#include "Interfaces.h"
#include "Server.h"

int main(){
    
    /* Points de départ et d'arrivée */
    Noeud point1(CASE11);
    Noeud point2(CASE66);
    Socket comTel(57373);
    comTel.onOpen();
    listeObstacles.push_back((comTel.onRead()));
    ListeObstacles::setCouleursAuto();
    
    
    std::vector<Point> listePoints;
    AStar test(50);
    listePoints=ListePoints::lissageBezier(test.getChemin(point1,point2),200);
    vector<Consigne> listeConsignes = ListePoints::convertirEnConsignes(listePoints);

    Debug::debugGraphique(listePoints);
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
