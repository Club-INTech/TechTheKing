#include <vector>
#include <iostream>

using namespace std;

class Point{
	public:
		
		/*
		 * coordonnées initialisées à 0 si pas de précision
		 */
		
		Point(int x=0,int y=0);
		
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
		
		Point operator+(const Point& Point2);
		
		Point operator-(const Point& Point2);
		
		bool operator==(const Point& Point2);
		
		void print();
		
		
	private:
		
		int m_x; // abscisse du point
		
		int m_y; //ordonnée du point
};

/*
 * fonction permettant le lissage d'une liste de points en l'utilisant comme liste de points de controles
 */

vector<Point> lissageBezier(const vector<Point>& pointsDeControle,int nbPointsBezier);