#include <Magick++.h>
#include "Debug.h"

using namespace Magick;

void Debug::debugConsignes(vector<Consigne> listeConsignes){
	cout << listeConsignes << endl;
}

void Debug::debugGraphique(vector<Point> listePoints){
	cout<<"Conversion du chemin emprunté par le robot en graphique..."<<endl;
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
	image.strokeWidth(5); //??!! Ca marche donc c'est cool :)
	for(unsigned int i=0;i<listePoints.size()-1;i++)
		image.draw(DrawableLine(listePoints[i].getX(),2100-listePoints[i].getY(),listePoints[i+1].getX(),2100-listePoints[i+1].getY()));

	
	Geometry echelle(1000,700);
	image.resize(echelle);
	image.display();
// 	image.magick("png");
// 	image.write("cheminRobot");
// 	cout<<"chemin emprunté dans le robot écrit dans cheminRobot.png"<<endl;
}
