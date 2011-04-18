#include <list>
#include <stack>

using namespace std ;

struct Point{
	int x,y;
};

typedef int Image[NX][NY];

void afficherImage(Image image){
	for(int i=0;i<NX;i++){
		for(int j=0;j<NY;j++){
			std::cout << image[i][j] ;
		}
		std::cout << std::endl;
	}
}

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
		for(int j=point.y-1;j<=point.y+1;j++){
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
	Point barycentre;
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
			if(i<0 || i > NX)
				continue;
			for(j=courant.y-1; j<=courant.y+1;j++){
				if(j<0 || j>NY || (i==courant.x && j==courant.y))
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

void annulerEllipse(Image image, Point centre)
{
	if (centre.x < 0 || centre.x >= NX || centre.y < 0 || centre.y>=NY)
		return;
	
	stack<Point> pile;
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
			pile.push((Point){x - 20, y});
			pile.push((Point){x + 20, y});
			pile.push((Point){x , y - 20});
			pile.push((Point){x , y + 20});
		}
	}
}

std::list<Point> trouverCentresMotifs(Image image){
	Point barycentreCourant;
	std::list<Point> listeCentres;
	for(int i=0;i<NX;i++){
		for(int j=0;j<NY;j++){
			if(image[i][j]==1){
					std::list<Point> contour = trouverContour(image, i, j);
					barycentreCourant = trouverBarycentre(contour);
					annulerEllipse(image,barycentreCourant);
					if(contour.size()>100){
						listeCentres.push_back(barycentreCourant);
					}
				}
			}
		}
	return listeCentres;
}

void afficherListe(std::list<Point> liste)
{
	std::list<Point>::const_iterator
	lit (liste.begin()),
	lend(liste.end());
	for(;lit!=lend;++lit) std::cout << "(" << lit->x << "," << lit->y << ") " ;
	std::cout << std::endl; 
}

void afficherMatrice(CvMat* m){
	for(int i=0;i<3;i++){
		for(int j=0;j<3;j++){
			std::cout << cvmGet(m,i,j) << " " ;
		}
		std::cout << std::endl;
	}
}

std::list<Point> detectionEllipse(IplImage *img)
{
	Image image;
	for(int i=0;i<NX;i++){
		for(int j=0;j<NY;j++){
			CvScalar pixel = cvGet2D(img,i,j);
			image[i][j] = (pixel.val[0] == 0) ? 0 : 1 ;
		}
	}
	
	return trouverCentresMotifs(image);
}
