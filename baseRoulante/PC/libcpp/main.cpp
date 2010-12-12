#include "Point.h"
#include "AStar.h"
#include "Consigne.h"
#include <Magick++.h>

using namespace Magick;

void debugGraphique(vector<Point> listePoints, vector<Obstacle*> listeObstacles){
	Image image( Geometry(3000,2100), Color("white") );
	
	image.fillColor("red");
	image.draw( DrawableRectangle(0,0, 400,400) );
	bool caseBleue=true;
	for(unsigned j=0;j<=1750;j+=350){
		for(unsigned i=450;i<=2200;i+=350){
			if(caseBleue==true){
				image.fillColor("blue");
				caseBleue=false;
			}
			else{
				image.fillColor("red");
				caseBleue=true;
			}
			image.draw( DrawableRectangle(i,j, i+350,j+350) );
		}
		if(caseBleue==true){
			image.fillColor("blue");
			caseBleue=false;
		}
		else{
			image.fillColor("red");
			caseBleue=true;
		}
	}
	image.fillColor("blue");
	image.draw( DrawableRectangle(2600,0, 3000,400) );
	
	/* Les lignes noires */
	image.fillColor("black");
	image.draw( DrawableRectangle(400,0, 450,2100) );
	image.draw( DrawableRectangle(2550,0, 2600,2100) );
	
	/* Les parties vertes */
	image.fillColor("green");
	image.draw( DrawableRectangle(0,400, 400,2100) );
	image.draw( DrawableRectangle(2600,400, 30000,2100) );
	
	/* Les cases speciales */
	image.fillColor("black");
	image.draw(DrawableEllipse(975,525, 50, 50, 0, 360));
	image.draw(DrawableEllipse(2025,525, 50, 50, 0, 360));
	image.draw(DrawableEllipse(975,1225, 50, 50, 0, 360));
	image.draw(DrawableEllipse(2025,1225, 50, 50, 0, 360));
	image.draw(DrawableEllipse(1325,1925, 50, 50, 0, 360));
	image.draw(DrawableEllipse(1675,1925, 50, 50, 0, 360));
	
	
	/* Affiche la courbe */
	image.strokeColor("Dark Magenta");
	image.strokeWidth(10);
	
	for(unsigned int i=0;i<listePoints.size()-1;i++)
		image.draw(DrawableLine(listePoints[i].getX(),2100-listePoints[i].getY(),listePoints[i+1].getX(),2100-listePoints[i+1].getY()));
	Geometry echelle(1000,700);
	image.resize(echelle);
	image.display();
}
int main(){
	
	/* Points de départ et d'arrivée */
	Noeud point1(CASE23);
	Noeud point2(CASE64);
	
	/* Obstacles */
	cercleObstacle pion1(CASE24);
	cercleObstacle pion2(CASE34);
	cercleObstacle pion3(CASE32);
	cercleObstacle pion4(CASE43);
	cercleObstacle pion5(CASE44);
	cercleObstacle pion6(CASE54);
	
	std::vector<Obstacle*> listeObstacle;
	
	listeObstacle.push_back(&pion1);
	listeObstacle.push_back(&pion2);
	listeObstacle.push_back(&pion3);
	listeObstacle.push_back(&pion4);
	listeObstacle.push_back(&pion5);
	listeObstacle.push_back(&pion6);
	
	/* Pathfinding */
	std::vector<Point> listePoints;
	AStar test(14,point1,point2,listeObstacle);
	listePoints=lissageBezier(test.getChemin(),100);
	vector<Consigne> listeConsignes = convertirEnConsignes(lissageBezier(test.getChemin(),100));
	
	/* Debug Graphique */
	debugGraphique(listePoints,listeObstacle);
	
	/* Envoi en série puis attente */
	SerialStream my_serial_stream;
	my_serial_stream.Open( "/dev/ttyUSB0" ) ;
	my_serial_stream.SetBaudRate( SerialStreamBuf::BAUD_57600 ) ;
	my_serial_stream.SetCharSize( SerialStreamBuf::CHAR_SIZE_8 ) ;
	my_serial_stream.SetNumOfStopBits(1) ;
	sleep(2);
	my_serial_stream << "e" << endl;
	SerialStream& liaisonSerie = my_serial_stream;
	for(unsigned int i=0;i<listeConsignes.size();i++)
		listeConsignes[i].transfertSerie(liaisonSerie);
	my_serial_stream << "e" << endl;
	while(1);

	
}
