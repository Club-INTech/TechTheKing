/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include <string.h>
#include <jni.h>
#include <android/log.h>
#include "pions/param.h"
#include "pions/android.h"
#include "pions/detectionEllipse.h"
#include "pions/fonctions.h"
#include "pions/loi_normale.h"
#include "pions/matrix.h"
#include "pions/traitement.h"
#include "pions/segmentation.h"
#include "cv.h"
#include "cxcore.h"
#include "bmpfmt.h"
#define ANDROID_LOG_VERBOSE ANDROID_LOG_DEBUG
#define LOGV(...) __android_log_print(ANDROID_LOG_SILENT, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#ifdef __cplusplus
extern "C" {
#endif
IplImage* pImage = NULL;
IplImage* loadPixels(int* pixels, int width, int height);
IplImage* getIplImageFromIntArray(JNIEnv* env, jintArray array_data,
		jint width, jint height);

// TODO: cvRelease
JNIEXPORT jstring JNICALL Java_intech_detection_PictureTaker_trouverPions(JNIEnv* env, jobject thiz,jint vue){

	// Filtre HSV
	LOGI("Filtrage HSV");
    IplImage *imgHSV = nvgHSV(pImage);
    
    // Extraction composante verte
    LOGI("Extraction composante verte");
    IplImage *imgv = extractionComposante(imgHSV);
    
    // Initialisation des paramètres
    float pi[NBR_CLASSES], moy[NBR_CLASSES], var[NBR_CLASSES];
    init_param(pi,moy,var);

    // Lancement EM
    LOGI("Lancement EM");
    uchar *pImg  = (uchar*)imgv->imageData;
    em(pImg,pi,moy,var,NBR_ITERATIONS_MAX);

    // Segmentation
    LOGI("Segmentation");
    IplImage *imgBin = cvCreateImage(cvGetSize(imgv), imgv->depth, 1);
    segmentation(imgv,imgBin,pi,moy,var);
    
    // Ouverture
    LOGI("Ouverture");
    IplImage *imgOp = cvCreateImage(cvGetSize(imgv), imgv->depth, 1);
	ouverture(imgBin,imgOp,3,3);
	
	// Détection des centres d'ellipse
	LOGI("Détection des centres");
	std::list<Point> listeCentres = detectionEllipse(imgOp);
	
	// Passage des centres des pions dans les coordonnées de la table
	if(!listeCentres.empty()){

			for(std::list<Point>::iterator it=listeCentres.begin() ; it != listeCentres.end() ; ++it){
			CvPoint center;
			center.x = cvRound(it->x);
			center.y = cvRound(it->y);
			int radius = cvRound(5);
			cvCircle( pImage, center, radius, CV_RGB(80,0,80), 3, CV_AA, 0 );
			}
	}
	
	LOGI("Transformation des coordonnées");
	// Transformation des coordonnées (homographie)
	CvMat* homogMatrix = initHomogMatrix(vue) ;
	std::list<Point> listeCentresTable = homogListe(listeCentres, homogMatrix,vue);
	
	// Affichage de la liste des centres
	inverserXY(listeCentresTable);

	// Affichage des résultats
	return ((env)->NewStringUTF(listeToString(listeCentresTable).c_str()));
}

JNIEXPORT jboolean JNICALL Java_intech_detection_PictureTaker_setSourceImage(
		JNIEnv* env, jobject thiz, jintArray photo_data, jint width,
		jint height) {
	if (pImage != NULL) {
		cvReleaseImage(&pImage);
		pImage = NULL;
	}
	pImage = getIplImageFromIntArray(env, photo_data, width, height);
	if (pImage == NULL) {
		return 0;
	}
	LOGI("Load Image Done.");
	return 1;
}
JNIEXPORT jbooleanArray JNICALL Java_intech_detection_DebugCamera_getSourceImage(
		JNIEnv* env, jobject thiz) {
	if (pImage == NULL) {
		LOGE("No source image.");
		return 0;
	}
	cvFlip(pImage);
	int width = pImage->width;
	int height = pImage->height;
	int rowStep = pImage->widthStep;
	int headerSize = 54;
	int imageSize = rowStep * height;
	int fileSize = headerSize + imageSize;
	unsigned char* image = new unsigned char[fileSize];
	struct bmpfile_header* fileHeader = (struct bmpfile_header*) (image);
	fileHeader->magic[0] = 'B';
	fileHeader->magic[1] = 'M';
	fileHeader->filesz = fileSize;
	fileHeader->creator1 = 0;
	fileHeader->creator2 = 0;
	fileHeader->bmp_offset = 54;
	struct bmp_dib_v3_header_t* imageHeader =
			(struct bmp_dib_v3_header_t*) (image + 14);
	imageHeader->header_sz = 40;
	imageHeader->width = width;
	imageHeader->height = height;
	imageHeader->nplanes = 1;
	imageHeader->bitspp = 24;
	imageHeader->compress_type = 0;
	imageHeader->bmp_bytesz = imageSize;
	imageHeader->hres = 0;
	imageHeader->vres = 0;
	imageHeader->ncolors = 0;
	imageHeader->nimpcolors = 0;
	memcpy(image + 54, pImage->imageData, imageSize);
	jbooleanArray bytes = env->NewBooleanArray(fileSize);
	if (bytes == 0) {
		LOGE("Error in creating the image.");
		delete[] image;
		return 0;
	}
	env->SetBooleanArrayRegion(bytes, 0, fileSize, (jboolean*) image);
	delete[] image;
	return bytes;
}

IplImage* loadPixels(int* pixels, int width, int height) {
	int x, y;
	IplImage *img = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
	unsigned char* base = (unsigned char*) (img->imageData);
	unsigned char* ptr;
	for (y = 0; y < height; y++) {
		ptr = base + y * img->widthStep;
		for (x = 0; x < width; x++) {
			// blue
			ptr[3 * x] = pixels[x + y * width] & 0xFF;
			// green
			ptr[3 * x + 1] = pixels[x + y * width] >> 8 & 0xFF;
			// blue
			ptr[3 * x + 2] = pixels[x + y * width] >> 16 & 0xFF;
		}
	}
	return img;
}
IplImage* getIplImageFromIntArray(JNIEnv* env, jintArray array_data,
		jint width, jint height) {
	int *pixels = env->GetIntArrayElements(array_data, 0);
	if (pixels == 0) {
		LOGE("Error getting int array of pixels.");
		return 0;
	}
	IplImage *image = loadPixels(pixels, width, height);
	env->ReleaseIntArrayElements(array_data, pixels, 0);
	if (image == 0) {
		LOGE("Error loading pixel array.");
		return 0;
	}
	return image;
}

#ifdef __cplusplus
}
#endif

