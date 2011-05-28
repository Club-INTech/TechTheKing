#ifndef DETECTION_ELLIPSE_H
#define DETECTION_ELLIPSE_H

#include <list>
#include <sstream>
#include <stack>
#include <string>
#include <iostream>

struct Point{
	int x,y;
};

typedef int Image[NX][NY];
void afficherImage(Image image);
void insererMotifs(Image image, Point coteHautGauche, Point coteBasDroit);
bool estDansListe(Point point,std::list<Point>& listePoints);
bool estInterieurMotif(Point& point, Image image);
Point trouverBarycentre(std::list<Point>& contour);
std::list<Point> trouverContour(Image image, int extremiteTrouveeX, int extremiteTrouveeY);
void annulerEllipse(Image image, Point centre);
std::list<Point> trouverCentresMotifs(Image image);
void afficherMatrice(CvMat* m);
std::list<Point> detectionEllipse(IplImage *img);
std::string intToString(int x);
void inverserXY(std::list<Point> &liste);
std::string listeToString(const std::list<Point> &liste);

#endif
