#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include <vector>

void proba_post(uchar *pImg, std::vector < Matrix<float> > &proba, float pi[], float moy[], float var[]);
void em(uchar *img, float pi[], float moy[], float var[], int n_iter);
void init_param(float pi[], float moy[], float var[]);
void segmentation(IplImage *img, IplImage *imgBin, float pi[], float moy[], float var[]);

#endif
