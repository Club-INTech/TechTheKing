#ifndef POINT_H
#define POINT_H

#include <vector>
#include <iostream>
#include "Consigne.h"
#include <cmath>
#include "Contrats.h"
#include <SerialStream.h>

/*!
 * \file Point.h
 * \brief La gestion des points.
 * \author Philippe Tillet.
 * \version 1.0
 */



/*!define des endroits remarquables */

#define CASE_DEPART_ROUGE 200,1900
#define CASE_DEPART_BLEU 2800,1900

#define CASE11 625,175
#define CASE12 975,175
#define CASE13 1325,175
#define CASE14 1675,175
#define CASE15 2025,175
#define CASE16 2375,175

#define CASE21 625,525
#define CASE22 975,525
#define CASE23 1325,525
#define CASE24 1675,525
#define CASE25 2025,525
#define CASE26 2375,525

#define CASE31 625,875
#define CASE32 975,875
#define CASE33 1325,875
#define CASE34 1675,875
#define CASE35 2025,875
#define CASE36 2375,875

#define CASE41 625,1225
#define CASE42 975,1225
#define CASE43 1325,1225
#define CASE44 1675,1225
#define CASE45 2025,1225
#define CASE46 2375,1225

#define CASE51 625,1575
#define CASE52 975,1575
#define CASE53 1325,1575
#define CASE54 1675,1575
#define CASE55 2025,1575
#define CASE56 2375,1575

#define CASE61 625,1925
#define CASE62 975,1925
#define CASE63 1325,1925
#define CASE64 1675,1925
#define CASE65 2025,1925
#define CASE66 2375,1925

using namespace std;

/*!
 * \class Point
 *
 * \brief Classe représentant les points.
 */

class Point{
	public:
		
		/*!
		 * \brief Constructeur de la classe point.
		 * 
		 * coordonnées initialisées à 0 si pas de précision
		 * tout est en double pour ne pas cumuler d'erreur lorsque l'on fait des opérations successives sur les points (lissage de Bézier par exemple)
		 */
		Point(double x=0,double y=0);


		Point(const Point&);
		
		/*!\brief Fonction d'affichage du point sur la sortie standart*/
		void print();

		
		/*!\brief Arrondis les coordonnées du point aux entiers les plus proches.*/
		void round();
		
		/*! \brief Calcul de la distance entre deux points */
		double rayon(Point Point2);

		/*!
		 * \brief Calcul de l'angle entre deux points.
		 * 
		 * L'angle est calculé par rapport à (Oy) et est entre -Pi et Pi.
		 */
		double angle(Point Point2);

		
		/*! \brief Accesseur setX*/
		void setX(double x);
		/*! \brief Accesseur setY*/
		void setY(double y);
		/*! \brief Accesseur getX*/
		double getX();
		/*! \brief Accesseur getY*/
		double getY();
		
		/*! \brief Opérateur de multiplication par une constante*/
		template<typename T> Point operator*(T k){
			Point resultat(m_x*k,m_y*k);
			return resultat;
		}

		/*! \brief Opérateur de division par une constante*/
		template<typename T> Point operator/(T k){
			Point resultat(m_x/k,m_y/k);
			return resultat;
		}

		/*! \brief Opérateur d'addition de deux points*/
		Point operator+(Point Point2);
		
		/*! \brief Opérateur de soustraction de deux points*/
		Point operator-(Point Point2);

		/*! \brief Opérateur d'égalité entre deux points*/
		bool operator==(Point Point2);
		bool operator!=(Point Point2);

		/*! \brief Opérateur de flux sortant*/
		friend ostream &operator<<(ostream &out, Point point);
		

		
		
	protected:
		
		double m_x; /*!< Abscisse du point*/
		double m_y; /*!< Ordonnée du point*/
		BEGIN_INVARIANT_BLOCK(Point)
		INVARIANT(0<=m_x, "Abscisse du point positive");
		INVARIANT(m_x<=3000, "Abscisse du point inférieure à 3000");
		INVARIANT(0<=m_y, "Ordonnée du point positive");
		INVARIANT(m_y<=2100, "Ordonnée du point inférieure à 2100");
		END_INVARIANT_BLOCK;
};


/*! \brief Operateur de flux sortant pour une liste de points. */
ostream &operator<<(ostream &out, vector<Point> listePoints);


/*!
 * \namespace ListePoints
 *
 * Toutes les fonctions concernant les listes de points.
 */

namespace ListePoints{
/*!
 *\brief Lissage de Bézier.
 *
 * fonction permettant le lissage d'une liste de points en l'utilisant comme liste de points de controles.
 *
 * \param nbPointsBezier nombre de points souhaités sur la courbe.
 * \return la liste de points lissée
 */
vector<Point> lissageBezier(const vector<Point>& pointsDeControle,int nbPointsBezier) ;

/*!
 *\brief Conversion en consignes.
 *
 * permet de convertir une liste de points en liste de consignes
 *\return la liste de consignes.
 */
vector<Consigne> convertirEnConsignes(vector<Point>& listePoints,int dephasageAngle);

/*!
 * \brief Est dans liste ?
 *
 * Verifie l'existence d'un point dans une liste donnée
 * 
 *\return l'indice (entre 0 et n-1) de la correspondance si elle existe. -1 sinon.
  */
int estDansListe(vector<Point>& listePoints, Point point);
}

#endif
