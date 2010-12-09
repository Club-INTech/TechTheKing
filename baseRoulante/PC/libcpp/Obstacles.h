#ifndef __OBSTACLE
#define OBSTACLE

#define TAILLE_ROBOT

#include "Point"

/*
 * définition de la classe abstraite Obstacle
 */

class Obstacle : public Point{
	public:
		virtual bool contientPoint(const Point& pointDonne);
	
}

class cercleObstacle : public Obstacle{
	public:
		bool contientPoint(const Point& pointDonne);
	private:
		m_rayon;
}


class rectangleObstacle : public Obstacle{
	public:
		bool contientPoint(const Point& pointDonne);
	private:
		m_demiCoteX;
		m_demiCoteY;
}

#endif