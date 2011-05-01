#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>

#define NX 1280
#define NY 960

#include "detectionEllipse.cpp"
#include "traitement.cpp"
#include "affichage.cpp"

IplImage *img ;
char vue ;
CvMat* homogMatrix;

int cfg_seuil = 180 ;
int cfg_morph_rows = 5 ;
int cfg_morph_cols = 5 ;



void detection()
{		
	// Binarisation
	IplImage *imgBin ;
	binarisation_nvg(img,imgBin,cfg_seuil) ;
	cvShowImage( "Binarisation", imgBin);

	// Suppression des pixels isolés
	IplImage *imgBinOp ;
	ouverture(imgBin,imgBinOp,cfg_morph_cols,cfg_morph_rows) ;
	cvShowImage( "Ouverture", imgBinOp);
	
	// Détection des pions
	std::list<Point> listeCentres;
	listeCentres = detectionEllipse(imgBinOp) ;

	
	// Passage des centres des pions dans les coordonnées de la table
	std::list<Point> listeCentresTable = homogListe(listeCentres, homogMatrix) ;
	
	// Affichage des centres sur l'image d'origine
	IplImage *imgCentre = cvCloneImage(imgBinOp);
	imgCentre->origin = img->origin;
	afficherListeCentre(imgCentre,listeCentres);
	//cvZero(imgCentre);
	cvShowImage( "Image", imgCentre);
	
	// Chargement de l'image de la table
	IplImage *imgTable;
	imgTable = cvLoadImage("img/table.png",1);
	
	// Affichage des pions sur la table
	afficherPions(imgTable, listeCentresTable) ;
	cout<<listeToString(listeCentres)<<endl;
	cout<<listeToString(listeCentresTable)<<endl;
	
	// Lancement de la fenêtre de résultat
	cvShowImage( "Resultat", imgTable);
}

void refreshImg(int v)
{
	detection() ;
}

int main(int argc, char** argv)
{
	// Type de point de vue
	vue = argv[1][0] ;
	
	// Chargement de l'image
	img = cvLoadImage(argv[2],1);
	
	// Création de la matrice de projection
	homogMatrix = initHomogMatrix(img,vue) ;
	
	// Lancement des fenêtres
	initFenetre();
	
	// Affichage de la transformation
	IplImage *imgTransform ;
	imgTransform = transformImg(img,homogMatrix);
	cvShowImage("Transformation", imgTransform);
	
	// Lancement de la détection avec affichage
	detection() ;
	//cvReleaseMat(&homogMatrix);
	
	// Création des trackbar pour chaque paramètre
	cvCreateTrackbar("Seuil de binarisation","Binarisation",&cfg_seuil,255,refreshImg);
	cvCreateTrackbar("Ouverture X","Ouverture",&cfg_morph_cols,50,refreshImg);
	cvCreateTrackbar("Ouverture Y","Ouverture",&cfg_morph_rows,25,refreshImg);
	
	cvWaitKey(0);
	
	return 0 ;
}

