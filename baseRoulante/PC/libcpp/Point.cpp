#include "Point.h"

/*
 * constructeur
 */

Point::Point(int x,int y){
	m_x=x;
	m_y=y;
}

/*
 * différents opérateurs
 */

Point Point::operator+(const Point& Point2){
	int x=m_x;
	int y=m_y;
	x+=Point2.m_x;
	y+=Point2.m_y;
	Point resultat(x,y);
	return resultat;
}

Point Point::operator-(const Point& Point2){
	int x=m_x;
	int y=m_y;
	x-=Point2.m_x;
	y-=Point2.m_y;
	Point resultat(x,y);
	return resultat;
}

bool Point::operator==(const Point& Point2){
	if(m_x==Point2.m_x && m_y==Point2.m_y)
		return true;
	else
		return false;
}

void Point::print(){
	cout << "x= " << m_x << " ; y= " << m_y << endl;
}

vector<Point> lissageBezier(const vector<Point>& pointsDeControle,int nbPointsBezier){
	vector<Point> resultat;
	double t;
	for(int k=0;k<=nbPointsBezier;k++){
		t=(double)k/nbPointsBezier;
		Point pointBezier;
		vector<Point> listeBarycentres(pointsDeControle);
		int longueur=pointsDeControle.size();
		for(int i=0;i<longueur-1;i++){
			for(int j=0;j<longueur-1-i;j++){
				listeBarycentres[j]=listeBarycentres[j]*(1-t)+listeBarycentres[j+1]*t;
			}
		}
		resultat.push_back(listeBarycentres[0]);
		listeBarycentres[0].print();
	}
	return resultat;
}
		