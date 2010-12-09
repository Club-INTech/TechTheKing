#include "Obstacles.h"

cercleObstacle::cercleObstacle(int x,int y,int rayon){
	m_x=x;
	m_y=y;
	m_rayon=rayon;
}

bool cercleObstacle::contientPoint(Point& pointDonne) const{
	if(rayon(pointDonne) < m_rayon + TAILLE_ROBOT)
		return true;
	return false;
}

bool rectangleObstacle::contientPoint(Point& pointDonne)const {
	if(fabs(pointDonne.getX()-m_x) < m_demiCoteX+TAILLE_ROBOT && fabs(pointDonne.getY()-m_y) < m_demiCoteY+TAILLE_ROBOT)
		return true;
	return false;
}

