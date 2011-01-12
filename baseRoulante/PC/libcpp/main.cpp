#include "Point.h"
#include "AStar.h"
#include "Consigne.h"
#include "Obstacles.h"
#include <Magick++.h>
#include "Debug.h"

int main(){
	
	/* Points de départ et d'arrivée */
	Noeud point1(CASE11);
	Noeud point2(CASE46);
	
	ListeObstacles::refreshPositions("PositionsPions.dat");

	ListeObstacles::setCouleursAuto();
	
	/* Pathfinding */
	std::vector<Point> listePoints;
	AStar test(50,point1,point2);
	listePoints=ListePoints::lissageBezier(test.getChemin(),200);
	vector<Consigne> listeConsignes = ListePoints::convertirEnConsignes(listePoints);

	/* Debug Graphique */
// 	Debug::debugGraphique(listePoints);
	
	/*debug consignes */
// 	Debug::debugConsignes(listeConsignes);
	
	/* Envoi en série puis attente */
// 	SerialStream my_serial_stream;
// 	my_serial_stream.Open( "/dev/ttyUSB0" ) ;
// 	my_serial_stream.SetBaudRate( SerialStreamBuf::BAUD_57600 ) ;
// 	my_serial_stream.SetCharSize( SerialStreamBuf::CHAR_SIZE_8 ) ;
// 	my_serial_stream.SetNumOfStopBits(1) ;
// 	sleep(2);
// 	my_serial_stream << "e" << endl;
// 	SerialStream& liaisonSerie = my_serial_stream;
// 	for(unsigned int i=0;i<listeConsignes.size();i++)
// 		listeConsignes[i].transfertSerie(liaisonSerie);
// 	my_serial_stream << "e" << endl;
// 	while(1);
	
}
