#ifndef __POINT
#define __POINT

#define CONVERSION_ANGLE_TICKS 1523.9085
#define CONVERSION_DISTANCE_TICKS 9.87020678

#include <vector>
#include <iostream>
#include "Consigne.h"
#include <cmath>

using namespace std;

class Point{
	public:
		
		/*
		 * coordonnées initialisées à 0 si pas de précision
		 */
		
		Point(int x=0,int y=0);
		
		void print();
		
		/*
		 * accesseurs
		 */
		
		void setX(int x);
		void setY(int y);
		int getX() const;
		int getY() const;
		
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
		
		/*
		 * opérateur de flux sortant pour les points
		 */
		
		friend ostream &operator<<(ostream &out, Point point);
		
		
	protected:
		
		int m_x; // abscisse du point
		
		int m_y; //ordonnée du point
};




/*
 * fonction permettant le lissage d'une liste de points en l'utilisant comme liste de points de controles
 */

vector<Point> lissageBezier(const vector<Point>& pointsDeControle,int nbPointsBezier) ;

/*
 * la fonction permettant de convertir une liste de points en liste de consignes
 */

vector<Consigne> convertirEnConsignes(const vector<Point>& listePoints) ;


#endif