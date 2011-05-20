#include "traitement.h"
#include "cv.h"
#include <android/log.h>
#define LOG_TAG "DETECTION_ELLIPSES"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)


/*
void binarisation_nvg(IplImage *img, IplImage* &imgBin, int seuil)
{
	// Transformation en niveau de gris
	IplImage *imgGris ;
	imgGris = cvCreateImage(cvGetSize(img), img->depth, 1);

	cvCvtColor(img,imgGris,CV_BGR2GRAY);
	
	// Binarisation
	imgBin = cvCloneImage(imgGris);

	cvThreshold(imgGris, imgBin, seuil, 255, CV_THRESH_BINARY);
}


void ouverture(IplImage *imgBin, int cfg_morph_cols, int cfg_morph_rows)
{
	IplImage *temp = cvCloneImage(imgBin);
	IplConvKernel *noyau = cvCreateStructuringElementEx(cfg_morph_cols, cfg_morph_rows, cfg_morph_cols/2, cfg_morph_rows/2, CV_SHAPE_ELLIPSE);
	cvMorphologyEx(imgBin, imgBin, temp, noyau, CV_MOP_OPEN, 3);
}
*/

IplImage* binariseHSV(const IplImage *imageRGB,int teinteMin,int teinteMax,int sat, int morphRows, int morphCols){
	float fR, fG, fB;
	float fH, fS, fV;
	const float FLOAT_TO_BYTE = 255.0f;
	const float BYTE_TO_FLOAT = 1.0f / FLOAT_TO_BYTE;

	// Create a blank HSV image
	IplImage *imageHSV = cvCreateImage(cvGetSize(imageRGB), 8, 3);
	if (!imageHSV || imageRGB->depth != 8 || imageRGB->nChannels != 3) {
		//printf("ERROR in convertImageRGBtoHSV()! Bad input image.\n");
		exit(1);
	}

	int h = imageRGB->height;		// Pixel height.
	int w = imageRGB->width;		// Pixel width.
	int rowSizeRGB = imageRGB->widthStep;	// Size of row in bytes, including extra padding.
	char *imRGB = imageRGB->imageData;	// Pointer to the start of the image pixels.
	int rowSizeHSV = imageHSV->widthStep;	// Size of row in bytes, including extra padding.
	char *imHSV = imageHSV->imageData;	// Pointer to the start of the image pixels.
	for (int y=0; y<h; y++) {
		for (int x=0; x<w; x++) {
			// Get the RGB pixel components. NOTE that OpenCV stores RGB pixels in B,G,R order.
			uchar *pRGB = (uchar*)(imRGB + y*rowSizeRGB + x*3);
			int bB = *(uchar*)(pRGB+0);	// Blue component
			int bG = *(uchar*)(pRGB+1);	// Green component
			int bR = *(uchar*)(pRGB+2);	// Red component

			// Convert from 8-bit integers to floats.
			fR = bR * BYTE_TO_FLOAT;
			fG = bG * BYTE_TO_FLOAT;
			fB = bB * BYTE_TO_FLOAT;

			// Convert from RGB to HSV, using float ranges 0.0 to 1.0.
			float fDelta;
			float fMin, fMax;
			int iMax;
			// Get the min and max, but use integer comparisons for slight speedup.
			if (bB < bG) {
				if (bB < bR) {
					fMin = fB;
					if (bR > bG) {
						iMax = bR;
						fMax = fR;
					}
					else {
						iMax = bG;
						fMax = fG;
					}
				}
				else {
					fMin = fR;
					fMax = fG;
					iMax = bG;
				}
			}
			else {
				if (bG < bR) {
					fMin = fG;
					if (bB > bR) {
						fMax = fB;
						iMax = bB;
					}
					else {
						fMax = fR;
						iMax = bR;
					}
				}
				else {
					fMin = fR;
					fMax = fB;
					iMax = bB;
				}
			}
			fDelta = fMax - fMin;
			fV = fMax;				// Value (Brightness).
			if (iMax != 0) {			// Make sure its not pure black.
				fS = fDelta / fMax;		// Saturation.
				float ANGLE_TO_UNIT = 1.0f / (6.0f * fDelta);	// Make the Hues between 0.0 to 1.0 instead of 6.0
				if (iMax == bR) {		// between yellow and magenta.
					fH = (fG - fB) * ANGLE_TO_UNIT;
				}
				else if (iMax == bG) {		// between cyan and yellow.
					fH = (2.0f/6.0f) + ( fB - fR ) * ANGLE_TO_UNIT;
				}
				else {				// between magenta and cyan.
					fH = (4.0f/6.0f) + ( fR - fG ) * ANGLE_TO_UNIT;
				}
				// Wrap outlier Hues around the circle.
				if (fH < 0.0f)
					fH += 1.0f;
				if (fH >= 1.0f)
					fH -= 1.0f;
			}
			else {
				// color is pure Black.
				fS = 0;
				fH = 0;	// undefined hue
			}

			// Convert from floats to 8-bit integers.
			int bH = (int)(0.5f + fH * 255.0f);
			int bS = (int)(0.5f + fS * 255.0f);
			int bV = (int)(0.5f + fV * 255.0f);

			// Clip the values to make sure it fits within the 8bits.
			if (bH > 255)
				bH = 255;
			if (bH < 0)
				bH = 0;
			if (bS > 255)
				bS = 255;
			if (bS < 0)
				bS = 0;
			if (bV > 255)
				bV = 255;
			if (bV < 0)
				bV = 0;

			// Set the HSV pixel components.
			uchar *pHSV = (uchar*)(imHSV + y*rowSizeHSV + x*3);
			int v = (bH <= teinteMax && bH >= teinteMin && bS > sat) ? 255 : 0;
			*(pHSV+0) = v;		// H component
			*(pHSV+1) = v;		// S component
			*(pHSV+2) = v;		// V component

			/*
			*(pHSV+0) = bH;		// H component
			*(pHSV+1) = bS;		// S component
			*(pHSV+2) = bV;		// V component
			*/
		}
	}

	// Create kernels for the morphological operation
	IplConvKernel *kernel;
	kernel = cvCreateStructuringElementEx(morphRows, morphCols, morphRows/2, morphCols/2, CV_SHAPE_ELLIPSE);
	cvErode(imageHSV, imageHSV, kernel, 1);

	return imageHSV;
}

CvMat* initHomogMatrix(IplImage *img, char vue)
{
	// Matrice de l'homographie
	CvMat* homogMatrix = cvCreateMat(3,3,CV_32FC1);
	
	// Coordonn�es des sommets
	CvPoint2D32f srcQuad[4], dstQuad[4];
	
	// Coordonn�es sources des sommets sur l'image
	switch(vue)
	{
		// Point de vue A (1.jpg)
		case 'a' :
			
			// Haut gauche
			srcQuad[0].x = 133; //dst Top left
			srcQuad[0].y = 94;
				
			// Haut droite
			srcQuad[1].x = 685; //dst Top right
			srcQuad[1].y = 95;
				
			// Bas gauche
			srcQuad[2].x = -890; //dst Bottom left
			srcQuad[2].y = 610;
				
			// Bas droit
			srcQuad[3].x = 1850; //dst Bot right
			srcQuad[3].y = 610;
		
		break ;
		
		// Point de vue B (2.jpg)
		case 'b' :
		
			// Haut gauche
			srcQuad[0].x = 256; //dst Top left
			srcQuad[0].y = 3;
				
			// Haut droite
			srcQuad[1].x = 772; //dst Top right
			srcQuad[1].y = 34;
				
			// Bas gauche
			srcQuad[2].x = -940; //dst Bottom left
			srcQuad[2].y = 250;
				
			// Bas droit
			srcQuad[3].x = 642; //dst Bot right
			srcQuad[3].y = 914;
		
		break ;
		
		// Point de vue C (3.jpg)
		case 'c' :
			
			// Haut gauche
			srcQuad[0].x = -85; //dst Top left
			srcQuad[0].y = 40;
				
			// Haut droite
			srcQuad[1].x = 450; //dst Top right
			srcQuad[1].y = 23;
				
			// Bas gauche
			srcQuad[2].x = 260; //dst Bottom left
			srcQuad[2].y = 1075;
				
			// Bas droit
			srcQuad[3].x = 1650; //dst Bot right
			srcQuad[3].y = 245;
		
		break ;
		
		// Point de vue A (1.jpg)
		case '5' :
			
			// Haut gauche
			srcQuad[0].x = -40; //dst Top left
			srcQuad[0].y = 60;
				
			// Haut droite
			srcQuad[1].x = 976; //dst Top right
			srcQuad[1].y = 60;
				
			// Bas gauche
			srcQuad[2].x = -3300; //dst Bottom left
			srcQuad[2].y = 1760;
				
			// Bas droit
			srcQuad[3].x = 3900; //dst Bot right
			srcQuad[3].y = 1760;
		
		break ;
	}
	
	// Cordonn�es destination
	// Haut gauche
	dstQuad[0].x = 0;
	dstQuad[0].y = 0;
	
	// Haut droite
	dstQuad[1].x = img->width - 1;
	dstQuad[1].y = 0;
	
	// Bas gauche
	dstQuad[2].x = 0;
	dstQuad[2].y = img->height - 1;
	
	// Bas droite
	dstQuad[3].x = img->width - 1;
	dstQuad[3].y = img->height - 1;
	
	// Cr�ation de la matrice
	cvGetPerspectiveTransform(srcQuad,dstQuad,homogMatrix);
	
	return homogMatrix ;	
}

float correctionTranchePion(float y)
{
	return 0.08*y+27.9 ;
}


IplImage* transformImg(IplImage *img, CvMat* homogMatrix)
{
	// Homographie de l'image
	IplImage *imgTransform;
	imgTransform = cvCloneImage(img);
	imgTransform->origin = img->origin;
	cvZero(imgTransform);
	
	// Cr�ation de l'image vue du dessus
	cvWarpPerspective(img,imgTransform,homogMatrix);
	
	return imgTransform;
}

std::list<Point> homogListe(std::list<Point> listeCentres, CvMat* homogMatrix)
{
	std::list<Point> listeCentresHomog;
	
	std::list<Point>::const_iterator
	lit (listeCentres.begin()),
	lend(listeCentres.end());
	for(;lit!=lend;++lit)
	{
		double x_centre = lit->y ;
		double y_centre = lit->x ;
		
		// Correction de la hauteur de la tranche
		y_centre += correctionTranchePion(y_centre) ;		
		
		// Application de l'homographie
		double x = cvmGet(homogMatrix,0,0)*x_centre + cvmGet(homogMatrix,0,1)*y_centre + cvmGet(homogMatrix,0,2) ;
		double y = cvmGet(homogMatrix,1,0)*x_centre + cvmGet(homogMatrix,1,1)*y_centre + cvmGet(homogMatrix,1,2) ;
		double z = cvmGet(homogMatrix,2,0)*x_centre + cvmGet(homogMatrix,2,1)*y_centre + cvmGet(homogMatrix,2,2) ;
	
		// Mise à l'échelle de l'image
		x = x/z * 630 / NY ;
		y = y/z * 900 / NX ;

		Point tmp;
		tmp.x = x ;
		tmp.y = y ;
		listeCentresHomog.push_back(tmp);		
	}
	
	return listeCentresHomog ;
}
