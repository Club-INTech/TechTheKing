#include "Point.h"
#include "AStar.h"
#include "Consigne.h"

int main(){
	Noeud point1(0,0);
	Noeud point2(2000,0);
	cercleObstacle pion(500,500,100);
// 	cercleObstacle pion2(700,500,100);
// 	cercleObstacle pion3(400,300,100);
	std::vector<Obstacle*> listeObstacle;
	listeObstacle.push_back(&pion);
// 	std::vector<Point> listePoints;
// 	listePoints.push_back(point1);
// 	listePoints.push_back(point2);
// 	cout << lissageBezier(listePoints,100) << endl;
	AStar test(10,point1,point2,listeObstacle);
convertirEnConsignes(lissageBezier(test.getChemin(),100));
}
