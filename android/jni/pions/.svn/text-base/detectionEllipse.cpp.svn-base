#include "param.h"
#include "android.h"
#include "detectionEllipse.h"


void insererMotifs(Image image, Point coteHautGauche, Point coteBasDroit){
	for(int i=coteHautGauche.x;i<coteBasDroit.x;i++)
		for(int j=coteHautGauche.y;j<coteBasDroit.y;j++)
			image[i][j]=1;
}

bool estDansListe(Point point,std::list<Point>& listePoints){
	for(std::list<Point>::iterator it=listePoints.begin();it!=listePoints.end();it++){
		if(it->x==point.x && it->y==point.y)
			return true;
	}
	return false;
}

bool estInterieurMotif(Point& point, Image image){
	for(int i=point.x-1;i<=point.x+1;i++){
		if(i<0 || i >= NX)
			continue;
		for(int j=point.y-1;j<=point.y+1;j++){
			if(j<0 || j >= NY)
				continue;
			if(image[i][j]==0)
				return false;
		}
	}
	return true;
}

Point trouverBarycentre(std::list<Point>& contour){
	Point barycentre;
	int sommeX=0;
	int sommeY=0;
	int tailleContour=contour.size();
	for(std::list<Point>::iterator it=contour.begin();it!=contour.end();it++){
		sommeX+=it->x;
		sommeY+=it->y;
	}
	
	barycentre.x = sommeX/tailleContour;
	barycentre.y = sommeY/tailleContour;
	
	return barycentre;
}

std::list<Point> trouverContour(Image image, int extremiteTrouveeX, int extremiteTrouveeY){
	std::list<Point> contour;
	std::list<Point> listeOuverte;
	int i,j;
	Point courant = {extremiteTrouveeX,extremiteTrouveeY};
	Point tmp;
	listeOuverte.push_back(courant);
	while( !listeOuverte.empty() ){
		courant=listeOuverte.back();
		contour.push_back(courant);
		listeOuverte.pop_back();
		/* On insère dans la liste des pixels à étudiers les autres noeuds du contour */
		for(i=courant.x-1; i<=courant.x+1;i++){
			if(i<0 || i >= NX)
				continue;
			for(j=courant.y-1; j<=courant.y+1;j++){
				if(j<0 || j>=NY || (i==courant.x && j==courant.y))
					continue;
				if(image[i][j]==1){
					tmp=(Point){i,j};
					/* il y a un 1 a proximité du noeud actuel. Existe-t-il un 0 autour de lui ?*/
					/* nécéssité de reparcourir les alentours du noeud courant */
					if(!estDansListe(tmp,contour) &&  !estDansListe(tmp,listeOuverte) &&  !estInterieurMotif(tmp,image)){
						listeOuverte.push_back(tmp);
					}
				}
			}	
		}
	}
	return contour;
}

void annulerEllipse(Image image, Point centre){
	if (centre.x < 0 || centre.x >= NX || centre.y < 0 || centre.y>=NY)
		return;
	
	std::stack<Point> pile;
	pile.push(centre);
	while (!pile.empty())
	{
		Point courant = pile.top();
		pile.pop();
		int x = courant.x;
		int y = courant.y;
		if (courant.x < 0 || courant.x >= NX || courant.y < 0 || courant.y>=NY)
			continue;
		if (image[x][y]==1)
		{
			image[x][y]=0;
			pile.push((Point){x - 1, y});
			pile.push((Point){x + 1, y});
			pile.push((Point){x , y - 1});
			pile.push((Point){x , y + 1});
			
			// Problème des pions non remplis
			//pile.push((Point){x - 20, y});
			//pile.push((Point){x + 20, y});
			//pile.push((Point){x , y - 20});
			//pile.push((Point){x , y + 20});
		}
	}
}

std::list<Point> trouverCentresMotifs(Image image){
	Point barycentreCourant;
	std::list<Point> listeCentres;
	std::list<Point> contour;
	std::ostringstream buf;
	for(int i=0;i<NX;++i){
		for(int j=0;j<NY;++j){
			if(image[i][j]==1){
					contour = trouverContour(image, i, j);
					barycentreCourant = trouverBarycentre(contour);
					annulerEllipse(image,barycentreCourant);
					//if(contour.size()>TOLERANCE_CONTOUR){
						listeCentres.push_back(barycentreCourant);
					//}
				}
			}
		}
	return listeCentres;
}

void inverserXY(std::list<Point> &liste)
{
	for(std::list<Point>::iterator it=liste.begin();it!=liste.end();it++)
	{
		int t = it->x;
		it->x = it->y;
		it->y = t;
	}
}

std::list<Point> detectionEllipse(IplImage *img){
	// Exporte l'image vers une matrice
	// A améliorer . Ou pas.

	Image image;

	LOGI("Conversion Image en Matrice binaire");

	for(int i=0;i<NX;i++){
		for(int j=0;j<NY;j++){
			CvScalar pixel = cvGet2D(img,i,j);
			image[i][j] = (pixel.val[0] == 0) ? 0 : 1 ;
		}
	}

	LOGI("Trouver Centre Motifs");
	
	std::list<Point> liste = trouverCentresMotifs(image);
	
	inverserXY(liste);

	return liste;
}

std::string listeToString(const std::list<Point> &liste)
{
	std::ostringstream oss;
	std::list<Point>::const_iterator lit;
	for(lit=liste.begin();lit!=liste.end();++lit)
	{
		oss << "x" << lit->x << "y" << lit->y;
	}
	return oss.str();
}
