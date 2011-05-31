#include "param.h"
#include "android.h"
#include "traitement.h"

// Calcul de la matrice d'homographie
CvMat* initHomogMatrix(int vue)
{
	// Matrice de l'homographie
	CvMat* homogMatrix = cvCreateMat(3,3,CV_32FC1);
	
	// Coordonnées des sommets
	CvPoint2D32f srcQuad[4], dstQuad[4];
	
	// Coordonnées sources des sommets sur l'image
	switch(vue)
	{
	
		// Point de vue 1 by jb (ca marchera donc pas)
		case 1 :
			//Gauche
			srcQuad[0].x = 25;
			srcQuad[0].y = 197;

			//Haut
			srcQuad[1].x = 129;
			srcQuad[1].y = 94;

			//Droite
			srcQuad[2].x = 461;
			srcQuad[2].y = 176;

			//Bas
			srcQuad[3].x = 173;
			srcQuad[3].y = 424;
			
			// Cordonnées destination
			// Gauche
			dstQuad[0].x = NY*1400/2100; //105cm
			dstQuad[0].y = NX*1500/3000; //140cm
			
			// Haut
			dstQuad[1].x = 0;
			dstQuad[1].y = NX-1;
			
			// Droit
			dstQuad[2].x = NY*350/2100; //80cm
			dstQuad[2].y = NX*800/3000; //35cm
			
			// Bas
			dstQuad[3].x = NY*1750/2100; // 45cm
			dstQuad[3].y = NX*450/3000; // 175cm
		
		break ;
	
		// Point de vue 3
		case 3 :
			
			// Haut gauche
			srcQuad[0].x = 457; //dst Top left
			srcQuad[0].y = 550;
				
			// Haut droite
			srcQuad[1].x = 6; //dst Top right
			srcQuad[1].y = 202;
				
			// Bas gauche
			srcQuad[2].x = 441; //dst Bottom left
			srcQuad[2].y = 226;
				
			// Bas droit
			srcQuad[3].x = 345; //dst Bot right
			srcQuad[3].y = 126;
			
			// Cordonnées destination
			// Haut gauche
			dstQuad[0].y = NY*450/2100;
			dstQuad[0].x = NX*120/3000;
			
			// Haut droite
			dstQuad[1].x = NY*1750/2100;
			dstQuad[1].y = NX*800/3000;
			
			// Bas gauche
			dstQuad[2].x = NY*700/2100;
			dstQuad[2].y = NX*1500/3000;
			
			// Bas droite
			dstQuad[3].x = NY-1;
			dstQuad[3].y = NX-1;
		
		break ;
		
		// Point de vue 5
		case 5 :
			
			// Haut gauche
			srcQuad[0].x = -8; //dst Top left
			srcQuad[0].y = 40;
				
			// Haut droite
			srcQuad[1].x = 488; //dst Top right
			srcQuad[1].y = 40;
				
			// Bas gauche
			srcQuad[2].x = -2; //dst Bottom left
			srcQuad[2].y = 273;
				
			// Bas droit
			srcQuad[3].x = 468; //dst Bot right
			srcQuad[3].y = 282;
			
			// Cordonnées destination
			// Haut gauche
			dstQuad[0].x = 0;
			dstQuad[0].y = 0;
			
			// Haut droite
			dstQuad[1].x = NY - 1;
			dstQuad[1].y = 0;
			
			// Bas gauche
			dstQuad[2].x = NY*700/2100;
			dstQuad[2].y = NX-NX*800/3000;
			
			// Bas droite
			dstQuad[3].x = NY*1400/2100;
			dstQuad[3].y = NX-NX*800/3000;
		
		break ;
	}
	
	// Création de la matrice
	cvGetPerspectiveTransform(srcQuad,dstQuad,homogMatrix);
	
	return homogMatrix ;	
}


// Application de l'homographie à la liste de points détectés
std::list<Point> homogListe(std::list<Point> listeCentres, CvMat* homogMatrix, int vue)
{
	std::list<Point> listeCentresHomog;
	
	std::list<Point>::const_iterator
	lit (listeCentres.begin()),
	lend(listeCentres.end());
	for(;lit!=lend;++lit)
	{
		float x_centre = lit->x ;
		float y_centre = lit->y ;
		
		// Correction de la hauteur de la tranche
		y_centre += correctionTranchePion(y_centre,vue) ;		
		
		// Application de l'homographie
		float x = cvmGet(homogMatrix,0,0)*x_centre + cvmGet(homogMatrix,0,1)*y_centre + cvmGet(homogMatrix,0,2) ;
		float y = cvmGet(homogMatrix,1,0)*x_centre + cvmGet(homogMatrix,1,1)*y_centre + cvmGet(homogMatrix,1,2) ;
		float z = cvmGet(homogMatrix,2,0)*x_centre + cvmGet(homogMatrix,2,1)*y_centre + cvmGet(homogMatrix,2,2) ;
	
		// Mise à l'échelle de l'image
		x = x/z * 3000 / NX ;
		y = y/z * 2100 / NY ;

		Point tmp;
		tmp.x = x;
		tmp.y = y;
		listeCentresHomog.push_back(tmp);		
	}
	
	return listeCentresHomog ;
}

float correctionTranchePion(float y, int vue)
{
	switch(vue)
	{
		case 3 :
			return 0.12*y-5.4 ;
		break;
		
		case 5 :
			return 0.10*y+10.6 ;
		break;
		
		default:
			return 0;
		break;
	}
}


void ouverture(IplImage *imgBin, IplImage *imgBinOp, int cfg_morph_cols, int cfg_morph_rows)
{
	IplImage *temp = cvCloneImage(imgBin);
	IplConvKernel *noyau = cvCreateStructuringElementEx(cfg_morph_cols, cfg_morph_rows, cfg_morph_cols/2, cfg_morph_rows/2, CV_SHAPE_ELLIPSE);
	cvMorphologyEx(imgBin, imgBinOp, temp, noyau, CV_MOP_OPEN, 3);
}

// Extraction de la composante verte
IplImage* extractionComposante(IplImage *img)
{
    IplImage *green = cvCreateImage(cvSize(NY,NX), img->depth, 1);
    uchar *pGreen = (uchar*)green->imageData;
	uchar *pImg  = (uchar*)img->imageData;
	
    int x, y;
    for(x=0; x<NX; x++)
    {
		for(y=0; y<NY; y++)
		{
			uchar *pG = (uchar*)(pGreen + y + x*NY);
			*pG = pImg[x*3*NY + 3*y + 1];
		}
	}

    return green;
}

IplImage* nvgHSV(const IplImage *imageRGB)
{
	float fR, fG, fB;
	float fH, fS, fV;
	const float FLOAT_TO_BYTE = 255.0f;
	const float BYTE_TO_FLOAT = 1.0f / FLOAT_TO_BYTE;

	// Create a blank HSV image
	IplImage *imageHSV = cvCreateImage(cvGetSize(imageRGB), 8, 3);

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
			*(pHSV+0) = (bH > 20 && bH < 60 && bV > 50 && bS > 70) ? bB : 0;		// H component
			*(pHSV+1) = (bH > 20 && bH < 60 && bV > 50 && bS > 70) ? bG : 0;		// S component
			*(pHSV+2) = (bH > 20 && bH < 60 && bV > 50 && bS > 70) ? bR : 0;		// V component
		}
	}
	return imageHSV;
}


IplImage* transformImg(IplImage *img, CvMat* homogMatrix)
{
	// Homographie de l'image
	IplImage *imgTransform;
	imgTransform = cvCloneImage(img);
	imgTransform->origin = img->origin;
	cvZero(imgTransform);
	
	// Création de l'image vue du dessus
	cvWarpPerspective(img,imgTransform,homogMatrix);
	
	return imgTransform;
}


void afficherPions(IplImage *img, std::list<Point> listeCentres)
{
	int x,y;
	std::list<Point>::const_iterator
	lit (listeCentres.begin()),
	lend(listeCentres.end());
	for(;lit!=lend;++lit)
	{
		// Mise à l'échelle de l'image
		x = lit->x * 900 / 3000;
		y = lit->y * 630 / 2100;
		
		// Affichage des cercles
		cvCircle(img, cvPoint(y,630-x), 30, CV_RGB(250,250,80), -1);	
	}
}

void afficherListeCentres(IplImage *img, std::list<Point> listeCentres)
{
	std::list<Point>::const_iterator
	lit (listeCentres.begin()),
	lend(listeCentres.end());
	for(;lit!=lend;++lit)
	{
		cvCircle(img, cvPoint(lit->x,lit->y), 5, CV_RGB(250,0,0), -1);	
	}
}

