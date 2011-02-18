
void initFenetre()
{
	cvNamedWindow("Resultat", 0); 
	cvNamedWindow("Binarisation", 0); 
	cvNamedWindow("Ouverture", 0); 
	cvNamedWindow("Transformation", 0); 
	cvNamedWindow("Image", 0); 
}

void afficherPions(IplImage *img, std::list<Point> listeCentres)
{
	std::list<Point>::const_iterator
	lit (listeCentres.begin()),
	lend(listeCentres.end());
	for(;lit!=lend;++lit)
	{
		cvCircle(img, cvPoint(lit->y,630-lit->x), 30, CV_RGB(250,250,80), -1);	
	}
	
}

void afficherListeCentre(IplImage *img, std::list<Point> listeCentres)
{
	std::list<Point>::const_iterator
	lit (listeCentres.begin()),
	lend(listeCentres.end());
	for(;lit!=lend;++lit)
	{
		cvRectangle(img, cvPoint(lit->y-5,lit->x-5), cvPoint(lit->y+5,lit->x+5), CV_RGB(255,0,0), 2 );
	}
}
