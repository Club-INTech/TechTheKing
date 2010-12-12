#include "Point.h"
#include "AStar.h"
#include "Consigne.h"

int main(){
	Noeud point1(CASE23);
	Noeud point2(CASE64);
	cercleObstacle pion1(CASE24);
	cercleObstacle pion2(CASE34);
	cercleObstacle pion3(CASE32);
// 	cercleObstacle pion4(CASE43);
	cercleObstacle pion5(CASE44);
	cercleObstacle pion6(CASE54);
	std::vector<Obstacle*> listeObstacle;
	listeObstacle.push_back(&pion1);
	listeObstacle.push_back(&pion2);
	listeObstacle.push_back(&pion3);
// 	listeObstacle.push_back(&pion4);
	listeObstacle.push_back(&pion5);
	listeObstacle.push_back(&pion6);
	std::vector<Point> listePoints;
	listePoints.push_back(point1);
	listePoints.push_back(point2);
	AStar test(14,point1,point2,listeObstacle);
	vector<Consigne> listeConsignes = convertirEnConsignes(lissageBezier(test.getChemin(),100));
	print(listeConsignes);
	SerialStream my_serial_stream;
	my_serial_stream.Open( "/dev/ttyUSB0" ) ;
	my_serial_stream.SetBaudRate( SerialStreamBuf::BAUD_57600 ) ;
	my_serial_stream.SetCharSize( SerialStreamBuf::CHAR_SIZE_8 ) ;
	my_serial_stream.SetNumOfStopBits(1) ;
/*	my_serial_stream.SetParity( SerialStreamBuf::PARITY_ODD ) ;
	my_serial_stream.SetFlowControl( SerialStreamBuf::FLOW_CONTROL_HARD ) */;
	sleep(2);
	my_serial_stream << "e" << endl;
	SerialStream& liaisonSerie = my_serial_stream;
	for(unsigned int i=0;i<listeConsignes.size();i++)
		listeConsignes[i].transfertSerie(liaisonSerie);
	my_serial_stream << "e" << endl;
	while(1);

	
}
