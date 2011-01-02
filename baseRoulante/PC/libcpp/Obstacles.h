#ifndef OBSTACLES_H
#define OBSTACLES_H

/*!
 * \file Obstacles.h
 * \brief La gestion des obstacles du décor : aussi bien les pions que les morceaux de bois sur la table.
 * \author Philippe Tillet.
 * \version 1.0
 */


#define TAILLE_ROBOT 140 /*!< Le robot est assimilé à un cercle de rayon TAILLE_ROBOT*/
#define MARGE_SECURITE_PION 100
#define TAILLE_PION 100 /*!< Le pion est un cercle de rayon TAILLE_PION*/

#define TOLERANCE_X 150 /*!< Le pion est considéré dans une case si son abscisse est à moins de TOLERANCE_X du centre*/
#define TOLERANCE_Y 150 /*!< Le pion est considéré dans une case si son ordonnée est à moins de TOLERANCE_Y du centre*/

#include "Point.h"
#include <Magick++.h>

#define COULEUR_ROBOT BLEU
#define	COULEUR_ADVERSE ROUGE

using namespace std;

/*!
 * \enum Couleur
 * \brief les constantes des couleurs;
 */
typedef enum
{
	ROUGE, /*!< Le pion est au robot rouge. */
	BLEU, /*!< Le pion est au robot bleu. */
	NEUTRE, /*!< Le pion n'est à personne.*/
	NOIR /*!<L'obstacle est une planche de bois*/
}
Couleur;

/*!
 * \class Obstacle
 * \brief Classe représentant un obstacle
 *
 * La classe abstraite obstacle de laquelle découleront les obstacles circulaires (pions cette année) et rectangulaires(morceaux de bois cette année).
 */

class Obstacle : public Point{
	
	public:

		/*!
		 *  \brief Constructeur
		 *
		 *  Constructeur de la classe Obstacle
		 *
		 *  \param x : Abscisse du centre de masse de l'obstacle.
		 *  \param y : Ordonnée du centre de masse de l'obstacle.
		 *  \param couleur : A qui est le pion.
		 */
		Obstacle(double x=0,double y=0,Couleur couleur=NEUTRE);

		/*!
		 * \brief contientCercle
		 *
		 * L'obstacle est il en collision avec un cercle?
		 *
		 * \return true si le cercle donné chevauche l'obstacle.
		 */
		virtual bool contientCercle(Point& centreCercle, int rayon) const ;

		/*!
		 * \brief draw
		 *
		 * Dessine l'e pion obstacle dans une image ImageMagick.
		 */
		virtual void draw(Magick::Image* image) const  { };

		/*!
		 * \brief deplacer
		 *
		 * Déplace un obstacle en (newX,newY)
		 */
		void deplacer(double newX,double newY) { m_x=newX; m_y=newY; };

		/*!
		 * \brief couleurPlusProche
		 *
		 * L'obstacle est-il à quelqu'un? Si oui, à qui?
		 *
		 * \return la couleur du robot à qui l'obstacle est, ou neutre si il n'est à personne.
		 */
		virtual Couleur couleurPlusProche() const;

		/*!
		 * \brief Accesseurs
		 */
		virtual Couleur getCouleur() const  { return m_couleur; };
		void setCouleur(Couleur couleur){ m_couleur=couleur; };
		
	protected:
		
		Couleur m_couleur;
		
};

/*!
 * \class cercleObstacle
 *
 * \brief classe représentant un obstacle circulaire.
 *
 * Cette classe correspond à priori uniquement aux pions cette année.
 */

class cercleObstacle : public Obstacle{
	
	public:
		
		cercleObstacle(double x,double y,Couleur m_couleur=NEUTRE);
		
		void draw(Magick::Image* image) const  ;
		
		bool contientCercle(Point& centreCercle,int rayon) const ;
		
		Couleur couleurPlusProche() const;
		
	private:
		
		int m_rayon;
};

/*!
 * \class rectangleObstacle
 *
 * \brief classe représentant un obstacle rectangulaire.
 *
 * Cette classe correspond à priori uniquement aux planches de bois cette annee.
 */

class rectangleObstacle : public Obstacle{
	
	public:
		
		rectangleObstacle(double x,double y,int demiCoteX,int demiCoteY);
		
		void draw(Magick::Image* image) const  ;

		bool contientCercle(Point& centreCercle,int rayon) const ;
		
		Couleur couleurPlusProche() const {return NOIR;};
		
	private:
		
		int m_demiCoteX;
		int m_demiCoteY;
};

extern std::vector <Obstacle*> listeObstacles ; /*!< La liste d'obstacles dans la mémoire du robot*/

/*!
 * \namespace ListeObstacles
 *
 * Toutes les fonctions concernant à la liste d'obstacles. (elle est unique)
 */
namespace ListeObstacles{

/*!
 * \brief contientCercle
 *
 * Existe-t-il dans la liste d'obstacles un obstacle de la couleur donnée contenant le cercle donné?
 *
 * \return Si il existe, l'obstacle en question le plus proche du centre du cercle. NULL sinon.
 */
Obstacle* contientCercle(Point centreCercle,int rayon, Couleur couleur);

/*!
 * \brief setCouleursAuto
 *
 * Parcourt la liste d'obstacles et attribue à chacun la couleur de la case la plus proche.
 *
 */
void setCouleursAuto();

/*!
 * \brief refreshPositions
 *
 * Recharge les coordonnees de la liste d'obstacles depuis le fichier .dat spécifié.
 *
 */
void refreshPositions(const char nomFichier[]);
}



#endif