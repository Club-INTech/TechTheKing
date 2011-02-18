
void binarisation_nvg(IplImage *img, IplImage* &imgBin, int seuil)
{
	// Transformation en niveau de gris
	IplImage *imgGris ;
	imgGris = cvCreateImage(cvGetSize(img), img->depth, 1);
	
	int flip=0;
	if(img->origin!=IPL_ORIGIN_TL){
		flip=CV_CVTIMG_FLIP;
	}
	cvConvertImage(img, imgGris, flip);
	
	// Binarisation
	imgBin = cvCloneImage(imgGris);
	cvThreshold(imgGris, imgBin, seuil, 255, CV_THRESH_BINARY);
}


void ouverture(IplImage *imgBin, IplImage* &imgBinOp, int cfg_morph_cols, int cfg_morph_rows)
{
	IplImage *temp = cvCloneImage(imgBin);
	imgBinOp = cvCloneImage(imgBin);
	IplConvKernel *noyau = cvCreateStructuringElementEx(cfg_morph_cols, cfg_morph_rows, cfg_morph_cols/2, cfg_morph_rows/2, CV_SHAPE_ELLIPSE);
	cvMorphologyEx(imgBin, imgBinOp, temp, noyau, CV_MOP_OPEN, 3);
}

CvMat* initHomogMatrix(IplImage *img, char vue)
{
	// Matrice de l'homographie
	CvMat* homogMatrix = cvCreateMat(3,3,CV_32FC1);
	
	// Coordonnées des sommets
	CvPoint2D32f srcQuad[4], dstQuad[4];
	
	// Coordonnées sources des sommets sur l'image
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
	
	// Cordonnées destination
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
	
	// Création de la matrice
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
	
	// Création de l'image vue du dessus
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

IplImage* extractionComposante(IplImage *img, char couleur = 'v')
{
	IplImage *red = cvCreateImage(cvSize(img->width, img->height),img->depth, img->nChannels );
	IplImage *green = cvCreateImage(cvSize(img->width, img->height ),img->depth, img->nChannels );
	IplImage *blue = cvCreateImage(cvSize(img->width, img->height ),img->depth, img->nChannels );
	
	// http://www.linuxconfig.org/use-opencv-to-separate-rgb-image-into-red-green-and-blue-components
	uchar *pImg  = (uchar*)img->imageData;
	uchar *pRed   = (uchar*)red->imageData;
	uchar *pGreen = (uchar*)green->imageData;
	uchar *pBlue  = (uchar*)blue->imageData;    

	int i, j, rED, gREEN, bLUE, byte;
	for( i = 0 ; i < img->height ; i++ ) 
	{
	  for( j = 0 ; j < img->width ; j++ ) 
	  {
		  rED = pImg[i*img->widthStep + j*img->nChannels + 2];
		  gREEN = pImg[i*img->widthStep + j*img->nChannels + 1];
		  bLUE = pImg[i*img->widthStep + j*img->nChannels + 0];
		  // RED
		  pRed[i*img->widthStep + j*img->nChannels + 2] = rED;
		  // GREEN
		  pGreen[i*img->widthStep + j*img->nChannels + 1] = gREEN;
		  // BLUE
		  pBlue[i*img->widthStep + j*img->nChannels + 0] = bLUE;
	  }
	}
	
	// Passage en niveau de gris
	IplImage *imgGris ;
	imgGris = cvCreateImage(cvGetSize(green), green->depth, 1);
	
	int flip=0;
	if(green->origin!=IPL_ORIGIN_TL){
		flip=CV_CVTIMG_FLIP;
	}
	cvConvertImage(green, imgGris, flip);
	
	return imgGris;
}