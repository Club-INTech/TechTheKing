#ifndef LOI_NORMALE_H
#define LOI_NORMALE_H

float loi_norm_cr(float valeur);
float loi_norm(float mu, float sigma, float valeur);

#define round(x) ((x)>=0?(int)((x)+0.5):(int)((x)-0.5))

#endif
