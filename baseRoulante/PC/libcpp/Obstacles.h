#ifndef __OBSTACLE
#define OBSTACLE

#define TAILLE_ROBOT 150

#include "Point.h"

using namespace std;

/* définition de la classe abstraite Obstacle */

class Obstacle : public Point{
	public:
		virtual bool contientPoint(Point& pointDonne) const  { };
	
};

/* obstacles particuliers */

class cercleObstacle : public Obstacle{
	public:
		cercleObstacle(int x=0,int y=0,int rayon=150);
		bool contientPoint(Point& pointDonne) const ;
	private:
		int m_rayon;
};


class rectangleObstacle : public Obstacle{
	public:
		bool contientPoint(Point& pointDonne) const ;
	private:
		int m_demiCoteX;
		int m_demiCoteY;
};

#endif