#ifndef __POINT
#define __POINT

#define CONVERSION_ANGLE_TICKS 1523.9085
#define CONVERSION_DISTANCE_TICKS 4.935

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
		
		/*
		 * accesseurs
		 */
		
		void setX(double x);
		void setY(double y);
		double getX() const;
		double getY() const;
		
		/*
		 * opérateurs classiques de multiplication, division de points par une constante,et d'ajouts, de soustraction et de comparaison de points
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
		
		Point operator<<(Point Point2) const;
		
		Point operator-(Point Point2) const;
		
		bool operator==(Point Point2) const;
		
		/*
		 * Calcul de distance et angle/(Oy) entre deux points
		 */
		
		double rayon(Point Point2) const;
		
		double angle(Point Point2) const;
		
		void round();
		
		/*
		 * opérateur de flux sortant pour les points
		 */
		
		friend ostream &operator<<(ostream &out, Point point);
		
		
	protected:
		
		double m_x; // abscisse du point
		
		double m_y; //ordonnée du point
};




/*
 * fonction permettant le lissage d'une liste de points en l'utilisant comme liste de points de controles
 */
ostream &operator<<(ostream &out, vector<Point> listePoints);

vector<Point> lissageBezier(const vector<Point>& pointsDeControle,int nbPointsBezier) ;

/*
 * la fonction permettant de convertir une liste de points en liste de consignes
 */

vector<Consigne> convertirEnConsignes(const vector<Point>& listePoints) ;


int estDansListe(vector<Point>& listePoints, Point point);

#endif