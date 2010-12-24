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
	
	/* Affiche les obstacles */
	for(unsigned int i=0;i<listeObstacles.size();i++){
		listeObstacles[i]->draw(&image);
	}
	
	/* Affiche la courbe */
	image.strokeColor("white");
	image.strokeWidth(10);
	
	for(unsigned int i=0;i<listePoints.size()-1;i++)
		image.draw(DrawableLine(listePoints[i].getX(),2100-listePoints[i].getY(),listePoints[i+1].getX(),2100-listePoints[i+1].getY()));
	Geometry echelle(1000,700);
	image.resize(echelle);
	image.display();
}
int main(){
	
	/* Points de départ et d'arrivée */
	Noeud point1(CASE11);
	Noeud point2(CASE46);
	
	/* Obstacles */
	cercleObstacle pion1(CASE54);
	cercleObstacle pion2(1150,1075,NEUTRE);
	cercleObstacle pion3(2175,875,ROUGE);
	cercleObstacle pion4(CASE33);
	cercleObstacle pion5(1200,1575,ROUGE);
	cercleObstacle pion6(1800,1225,ROUGE);
	cercleObstacle pion7(CASE31);
	cercleObstacle pion8(875,1125,ROUGE);
// 	cercleObstacle pion9(CASE46);
// 	cercleObstacle pion10(CASE51);
// 	cercleObstacle pion11(CASE53);
// 	cercleObstacle pion12(CASE55);
	
	std::vector<Obstacle*> listeObstacle;
	
	listeObstacle.push_back(&pion1);
	listeObstacle.push_back(&pion2);
	listeObstacle.push_back(&pion3);
	listeObstacle.push_back(&pion4);
	listeObstacle.push_back(&pion5);
	listeObstacle.push_back(&pion6);
	listeObstacle.push_back(&pion7);
	listeObstacle.push_back(&pion8);
// 	listeObstacle.push_back(&pion9);
// 	listeObstacle.push_back(&pion10);
// 	listeObstacle.push_back(&pion11);
// 	listeObstacle.push_back(&pion12);

	setCouleursAuto(listeObstacle);
	
	/* Pathfinding */
	std::vector<Point> listePoints;
	AStar test(20,point1,point2,listeObstacle);
	listePoints=lissageBezier(test.getChemin(),100);
	vector<Consigne> listeConsignes = convertirEnConsignes(lissageBezier(test.getChemin(),200));

	cout << pion1.couleurPlusProche()<< endl;
	cout << pion2.couleurPlusProche()<< endl;
	cout << pion3.couleurPlusProche()<< endl;
	cout << pion4.couleurPlusProche()<< endl;
	cout << pion5.couleurPlusProche()<< endl;
	cout << pion6.couleurPlusProche()<< endl;
	cout << pion7.couleurPlusProche()<< endl;
	cout << pion8.couleurPlusProche()<< endl;
	
	/* Debug Graphique */
	debugGraphique(listePoints,listeObstacle);
	
	/*debug consignes */
// 	cout << listeConsignes << endl;
	
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
