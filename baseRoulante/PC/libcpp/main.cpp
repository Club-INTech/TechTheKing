#include "Point.h"
#include "AStar.h"
#include "Consigne.h"

int main(){
	Noeud point1(0,0);
	Noeud point2(2000,1300);
	cercleObstacle pion1(0,500,100);
	cercleObstacle pion2(1000,500,100);
	cercleObstacle pion3(2000,950,100);
	cercleObstacle pion4(1200,300,100);
	cercleObstacle pion5(1500,500,100);
	std::vector<Obstacle*> listeObstacle;
	listeObstacle.push_back(&pion1);
	listeObstacle.push_back(&pion2);
	listeObstacle.push_back(&pion3);
	listeObstacle.push_back(&pion4);
	listeObstacle.push_back(&pion5);
	std::vector<Point> listePoints;
	listePoints.push_back(point1);
	listePoints.push_back(point2);
	AStar test(14,point1,point2,listeObstacle);
	print(convertirEnConsignes(lissageBezier(test.getChemin(),100)));
}
