#include "param.h"
#include "android.h"
#include "matrix.h"
#include "loi_normale.h"
#include "traitement.h"
#include "fonctions.h"
#include "segmentation.h"


// Initialisation des paramètres
void init_param(float pi[], float moy[], float var[])
{
	 #if NBR_CLASSES == 3
	 
	 // Init Pi
     pi[0] = 0.333;
     pi[1] = 0.333;
     pi[2] = 0.333;

     // Init moyenne
     moy[0] = 123;
     moy[1] = 133;
     moy[2] = 200;

     // Init variance
     var[0] = 100;
     var[1] = 100;
     var[2] = 100;
	
	 #else
     
     // Init Pi
     pi[0] = 0.25;
     pi[1] = 0.25;
     pi[2] = 0.25;
     pi[3] = 0.25;

     // Init moyenne
     moy[0] = 50;
     moy[1] = 140;
     moy[2] = 180;
     moy[3] = 200;

     // Init variance
     var[0] = 100;
     var[1] = 100;
     var[2] = 100;
     var[3] = 100;
     
     #endif
     

}


// Calcul des proba a posteriori
void proba_post(uchar *pImg, std::vector < Matrix<float> > &proba, float pi[], float moy[], float var[])
{    
    float p[NBR_CLASSES];
    int x,y,c;
    float somme;

    // Pour chaque pixel de l'image
    for (x=0; x<NX; x++)
    {
        for (y=0; y<NY; y++)
        {
			if (pImg[y + x*NY] == 0) continue;
			
			//LOGI(convertString(x) + " " + convertString(y));
			
			somme = 0;
			
            for (c=0; c<NBR_CLASSES; c++)
            {
				p[c] = pi[c] * loi_norm(moy[c],var[c],(float) pImg[y + x*NY]) ;
				somme += p[c];
			}
            
            for (c=0; c<NBR_CLASSES; c++)
            {
				if (somme > 0)
				{
					proba[c].set(p[c]/somme,x,y) ;
				}
				else
				{
					proba[c].set(p[c],x,y) ;
				}
			}
        }
    }
}

void em(uchar *pImg, float pi[], float moy[], float var[], int n_iter)
{
	// On crée un tableau de 4 éléments contenant les matrices des proba a posteriori pour chaque classe
    std::vector < Matrix<float> > proba;
    Matrix<float> A(NX,NY);
    Matrix<float> B(NX,NY);
    Matrix<float> C(NX,NY);
    Matrix<float> D(NX,NY);
    Matrix<float> E(NX,NY);
    proba.push_back(A);
    proba.push_back(B);
    proba.push_back(C);
    proba.push_back(D);
    proba.push_back(E);

    float nbr_pix = NX * NY;
    int i,x,y,c;

    // Pour le nombre d'itération
    for (i=0; i<n_iter; i++)
    {
        LOGI(("Iteration " + convertString(i)).c_str());

        // Calcul des proba posteriori
        proba_post(pImg,proba,pi,moy,var);
		
		// Remise à zéro de Pi
        resetTab(pi);
        resetTab(moy);
        resetTab(var);

        // Calcul du Pi
        for (x=0; x<NX; x++)
        {
            for (y=0; y<NY; y++)
            {
				if (pImg[y + x*NY] == 0) continue;
				
				// proba[i] contient la matrice des proba a posteriori pour la classe i
                for (c=0; c<NBR_CLASSES; c++)
				{
					pi[c] += proba[c].get(x,y);
				}
            }

        }
        
        for (c=0; c<NBR_CLASSES; c++)
		{
			pi[c] /= nbr_pix;
		}
		
		print_tab("Pi",pi);

        // Calcul du moy
        for (x=0; x<NX; x++)
        {
            for (y=0; y<NY; y++)
            {
				if (pImg[y + x*NY] == 0) continue;
				
                for (c=0; c<NBR_CLASSES; c++)
				{
					moy[c] += ((float)pImg[x*NY+y])*proba[c].get(x,y);
				}
                
            }
        }
        
        for (c=0; c<NBR_CLASSES; c++)
		{
			moy[c] /= nbr_pix*pi[c];
		}

		print_tab("Moy",moy);
		
        // Calcul du var
        for (x=0; x<NX; x++)
        {
            for (y=0; y<NY; y++)
            {
				if (pImg[y + x*NY] == 0) continue;
				
                for (c=0; c<NBR_CLASSES; c++)
				{
					var[c] += (((float)pImg[x*NY+y])-moy[c])*(((float)pImg[x*NY+y])-moy[c])*proba[c].get(x,y);
				}
                
            }
        }
        
        for (c=0; c<NBR_CLASSES; c++)
		{
			var[c] /= nbr_pix*pi[c];
		}
		
		print_tab("Var",var);
        
    }
}

void segmentation(IplImage *img, IplImage *imgBin, float pi[], float moy[], float var[])
{
    CvScalar couleurImgBin;
    uchar *pImg  = (uchar*)img->imageData;
    int x,y,c;
    float p[NBR_CLASSES];

    // Pour chaque pixel de l'image
    for (x=0; x<NX; x++)
    {
        for (y=0; y<NY; y++)
        {   
			if (pImg[y + x*NY] == 0)
			{
				couleurImgBin.val[0] = 0;
			}
			else
			{ 
				for (c=0; c<NBR_CLASSES; c++)
				{
					p[c] = pi[c] * loi_norm(moy[c],var[c],(float)pImg[x*NY+y]) ;
				}

				#if AFFICHER_CLASSES == 1
				
				int classe = max(p); 
				couleurImgBin.val[0] = (max(p) == (NBR_CLASSES-1)) ? 255 : moy[classe];
				
				#else
				
				couleurImgBin.val[0] = (max(p) == (NBR_CLASSES-1)) ? 255 : 0;
				
				#endif
			}
			
            cvSet2D(imgBin,x,y,couleurImgBin);
        }
    }
}

