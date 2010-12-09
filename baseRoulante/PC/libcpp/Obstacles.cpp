#include "Obstacles.h"



bool cercleObstacle::contientPoint(Point pointDonne){
	(this.rayon(pointDonne)<m_rayon+TAILLE_ROBOT)?return true:return false;
}

bool rectangleObstacle::contientPoint(Point pointDonne){
	(abs(pointDonne.m_x-m_x) < m_demiCoteX+TAILLE_ROBOT && abs(pointDonne.m_y-m_y) < m_demiCoteY+TAILLE_ROBOT)?return true:return false;
}