#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>

#define NX 1280
#define NY 960

#include "detectionEllipse.h"
#include "traitement.h"

IplImage *img ;
char vue ;
CvMat* homogMatrix;

int cfg_seuil = 180 ;
int cfg_morph_rows = 5 ;
int cfg_morph_cols = 5 ;

int main(int argc, char** argv)
{
	// Type de point de vue
	vue = argv[1][0] ;
	
	// Chargement de l'image
	img = cvLoadImage(argv[2],1);
	
	// Création de la matrice de projection
	homogMatrix = initHomogMatrix(img,vue) ;
	
	// Binarisation
	IplImage *imgBin ;
	binarisation_nvg(img,imgBin,cfg_seuil) ;

	
	// Suppression des pixels isolés
	IplImage *imgBinOp ;
	ouverture(imgBin,imgBinOp,cfg_morph_cols,cfg_morph_rows) ;
	
	// Détection des pions
	std::list<Point> listeCentres;
	listeCentres = detectionEllipse(imgBinOp) ;

	// Passage des centres des pions dans les coordonnées de la table
	std::list<Point> listeCentresTable = homogListe(listeCentres, homogMatrix);
	
	// Affichage des résultats
	std::cout<<listeToString(listeCentresTable);
	
	return 0 ;
}

