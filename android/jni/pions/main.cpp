#include <list>

#include "param.h"
#include "android.h"
#include "matrix.h"
#include "traitement.h"
#include "segmentation.h"
#include "detectionEllipse.h"
#include "fonctions.h"

int main(int argc, char** argv)
{
    // Chargement de l'image
    IplImage *img = cvLoadImage(argv[1],1);
    
    // Filtre HSV
    IplImage *imgHSV = nvgHSV(img);
    
    // Extraction composante verte
    IplImage *imgv = extractionComposante(imgHSV);
    
    // Initialisation des paramètres
    float pi[NBR_CLASSES], moy[NBR_CLASSES], var[NBR_CLASSES];
    init_param(pi,moy,var);

    // Lancement EM
    uchar *pImg  = (uchar*)imgv->imageData;
    em(pImg,pi,moy,var,NBR_ITERATIONS_MAX);

    // Segmentation
    IplImage *imgBin = cvCreateImage(cvGetSize(imgv), imgv->depth, 1);
    segmentation(imgv,imgBin,pi,moy,var);
    
    // Ouverture
    IplImage *imgOp = cvCreateImage(cvGetSize(imgv), imgv->depth, 1);
	ouverture(imgBin,imgOp,3,3);
	
	// Détection des centres d'ellipse
	std::list<Point> listeCentres = detectionEllipse(imgOp);
	
	// Affichage des centres
	IplImage *imgCentres = cvCloneImage(img);
	afficherListeCentres(imgCentres,listeCentres);
	
	// Transformation des coordonnées (homographie)
	CvMat* homogMatrix = initHomogMatrix(3) ;
	std::list<Point> listeCentresTable = homogListe(listeCentres, homogMatrix,3);
	
	// Affichage des pions
	IplImage *table = cvLoadImage("img/table.png",1);
	afficherPions(table,listeCentresTable);
	
	// Image initiale redressée
	IplImage *imgTransformee = transformImg(img,homogMatrix);
	
	// Affichage de la liste des centres
	// ! Ne pas inverser avant afficherPions !
	inverserXY(listeCentresTable);
	print_liste(listeCentresTable);
	
	cvNamedWindow("Filtre", 0);
	cvShowImage("Filtre", imgHSV);
		
	cvNamedWindow("Vert", 0);
	cvShowImage("Vert", imgv);
		
	cvNamedWindow("Segmentation", 0);
	cvShowImage("Segmentation", imgBin);
		
	cvNamedWindow("Ouverture", 0);
	cvShowImage("Ouverture", imgOp);
	
	cvNamedWindow("Table", 0);
	cvShowImage("Table", table);
	
	cvNamedWindow("Centres", 0);
	cvShowImage("Centres", imgCentres);
	
	cvNamedWindow("Homographie", 0);
	cvShowImage("Homographie", imgTransformee);

	cvWaitKey(0);

    return 0;
}
