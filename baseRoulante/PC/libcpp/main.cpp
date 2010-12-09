#include "Point.h"
#include "AStar.h"

int main(){
	Noeud point1(0,0);
	Noeud point2(100,0);
	cercleObstacle pion(500,500,100);
	std::vector<Obstacle*> listeObstacle;
	listeObstacle.push_back(&pion);
	cout << point2 << endl;
	AStar test(10,point1,point2,listeObstacle);
}
