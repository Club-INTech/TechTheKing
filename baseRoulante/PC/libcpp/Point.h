#ifndef __POINT
#define __POINT

/* coefficients de conversion */

#define CONVERSION_ANGLE_TICKS 1523.9085
#define CONVERSION_DISTANCE_TICKS 9.870

/*define des endroits remarquables */

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

#include <vector>
#include <iostream>
#include "Consigne.h"
#include <cmath>

using namespace std;

class Point{
	public:
		
		/*
		 * coordonnées initialisées à 0 si pas de précision
		 * tout est en double pour ne pas cumuler d'erreur lorsque l'on fait des opérations successives sur les points (lissage de Bézier par exemple)
		 */
		
		Point(double x=0,double y=0);
		
		void print();
		
		/* accesseurs */
		
		void setX(double x);
		void setY(double y);
		double getX() const;
		double getY() const;
		
		/*
		 * opérateurs classiques de multiplication, division de points par une constante,et d'ajouts, de soustraction et de comparaison de points.
		 * opérateur de flux sortant
		 */
		
		template<typename T> Point operator*(T k){
			Point resultat(m_x*k,m_y*k);
			return resultat;
		}
		
		template<typename T> Point operator/(T k){
			Point resultat(m_x/k,m_y/k);
			return resultat;
		}
		
		Point operator+(Point Point2) const;
		Point operator-(Point Point2) const;
		bool operator==(Point Point2) const;
		
		friend ostream &operator<<(ostream &out, Point point);
		
		/* Arrondis les coordonnées du points aux entiers les plus proches */
		
		void round();
		
		/* Calcul de distance et angle/(Oy) entre deux points */
		
		double rayon(Point Point2) const;
		
		double angle(Point Point2) const;
		

		
		
	protected:
		
		/* Abscisse et ordonnée du point */
		
		double m_x;
		
		double m_y;
		
};

/*Operateur pour les listes de points*/

ostream &operator<<(ostream &out, vector<Point> listePoints);

/* fonction permettant le lissage d'une liste de points en l'utilisant comme liste de points de controles*/

vector<Point> lissageBezier(const vector<Point>& pointsDeControle,int nbPointsBezier) ;

/* la fonction permettant de convertir une liste de points en liste de consignes */

vector<Consigne> convertirEnConsignes(const vector<Point>& listePoints) ;

/* vérifie l'existence d'un point dans une liste donnée */

int estDansListe(vector<Point>& listePoints, Point point);

#endif