#ifndef CONSTANTES_H
#define CONSTANTES_H

extern double CONVERSION_RADIAN_TIC;
extern double CONVERSION_TIC_RADIAN;
extern double CONVERSION_RADIAN_ANGLE;
extern double CONVERSION_TIC_MM;
extern double CONVERSION_MM_TIC;
extern double TAILLE_ROBOT; /*!< Le robot est assimilé à un cercle de rayon TAILLE_ROBOT*/
extern double MARGE_SECURITE_PION;
extern double TAILLE_PION; /*!< Le pion est un cercle de rayon TAILLE_PION*/
extern double TOLERANCE_X; /*!< Le pion est considéré dans une case si son abscisse est à moins de TOLERANCE_X du centre*/
extern double TOLERANCE_Y; /*!< Le pion est considéré dans une case si son ordonnée est à moins de TOLERANCE_Y du centre*/

/*!
 * \enum Couleur
 * \brief les constantes des couleurs;
 */
enum Couleur
{
    ROUGE, /*!< Le pion est au robot rouge. */
    BLEU, /*!< Le pion est au robot bleu. */
    NEUTRE, /*!< Le pion n'est à personne.*/
    NOIR /*!<L'obstacle est une planche de bois*/
};

extern Couleur COULEUR_ROBOT;
extern Couleur COULEUR_ADVERSE;

#endif
