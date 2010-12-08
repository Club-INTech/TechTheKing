#include "Point.h"

int main(){
	Point point1(0,0);
	Point point2(1000,1000);
	vector<Point> listePoints;
	listePoints.push_back(point1);
	listePoints.push_back(point2);
	convertirEnConsignes(lissageBezier(listePoints,100));
}
