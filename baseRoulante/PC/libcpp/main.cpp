#include "Point.h"
#include "AStar.h"
#include "Consigne.h"
#include "Obstacles.h"
#include <Magick++.h>

using namespace Magick;


void debugGraphique(vector<Point> listePoints){
	
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
	image.strokeColor(Color(MaxRGB,MaxRGB,MaxRGB,MaxRGB/2));
	image.strokeWidth(2*TAILLE_ROBOT); //??!! Ca marche donc c'est cool :)
	for(unsigned int i=0;i<listePoints.size()-1;i++)
		image.draw(DrawableLine(listePoints[i].getX(),2100-listePoints[i].getY(),listePoints[i+1].getX(),2100-listePoints[i+1].getY()));

	
	Geometry echelle(1000,700);
	image.resize(echelle);
	image.display();

	image.magick("png");
	image.write("cheminRobot");
	
}

int main(){
	
	/* Points de départ et d'arrivée */
	Noeud point1(CASE31);
	Noeud point2(CASE46);
	
	ListeObstacles::refreshPositions("PositionsPions.dat");

	ListeObstacles::setCouleursAuto();
	
	/* Pathfinding */
	std::vector<Point> listePoints;
	AStar test(30,point1,point2);
	listePoints=ListePoints::lissageBezier(test.getChemin(),200);
	vector<Consigne> listeConsignes = ListePoints::convertirEnConsignes(listePoints);

	/* Debug Graphique */
	debugGraphique(listePoints);
	
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
