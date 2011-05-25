#ifndef TRAITEMENT_H
#define TRAITEMENT_H

#include "detectionEllipse.h"

IplImage* binariseHSV(const IplImage *imageRGB,int teinteMin,int teinteMax,int sat, int morphRows, int morphCols);

void binarisation_nvg(IplImage *img, IplImage* &imgBin, int seuil);

void ouverture(IplImage *imgBin, int cfg_morph_cols, int cfg_morph_rows);

CvMat* initHomogMatrix(IplImage *img, char vue);

float correctionTranchePion(float y);

IplImage* transformImg(IplImage *img, CvMat* homogMatrix);

std::list<Point> homogListe(std::list<Point> listeCentres, CvMat* homogMatrix);

IplImage* extractionComposante(IplImage *img, char couleur = 'v');

#endif
