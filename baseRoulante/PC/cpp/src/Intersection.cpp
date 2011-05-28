#include <vector>
#include "Obstacles.h"

#define TOLERANCE_DISTANCE 10

bool comparerPions(std::vector<Obstacle*>::iterator a, std::vector<Obstacle*>::iterator b);
void printVector(std::vector<Obstacle*> v);
std::vector< std::pair<Obstacle*,int> > fusionResultats(std::vector<Obstacle*> t1, std::vector<Obstacle*> t2, std::vector<Obstacle*> t3);

// Intersection des résultats (au moins 2 téléphones nécessaires)
std::vector< std::pair<Obstacle*,int> > fusionResultats(std::vector<Obstacle*> t1, std::vector<Obstacle*> t2, std::vector<Obstacle*> t3)
{
	std::vector<Obstacle*>::iterator it1,it2,it3;
	int n;
	double x,y;
	std::vector< std::pair<Obstacle*,int> > resultatFusion;
	
	for(it1=t1.begin();it1!=t1.end();it1++)
	{
		n = 1;
		x = (*it1)->getX();
		y = (*it1)->getY();
		
		for(it2=t2.begin();it2!=t2.end();it2++)
		{
			if (comparerPions(it1,it2))
			{
				n++;
				x += (*it2)->getX();
				y += (*it2)->getY();
				t2.erase(it2);
			}
		}
		for(it3=t3.begin();it3!=t3.end();it3++)
		{
			if (comparerPions(it1,it3))
			{
				n++;
				x += (*it3)->getX();
				y += (*it3)->getY();
				t3.erase(it3);
			}
		}
		
		if (n > 1)
		{
			x = x/n;
			y = y/n;
			resultatFusion.push_back(std::make_pair<Obstacle*,int>(new CercleObstacle(x,y),n));
		}
	}
	
	return resultatFusion;
}

// True si les 2 pions sont les mêmes, false sinon 
bool comparerPions(std::vector<Obstacle*>::iterator a, std::vector<Obstacle*>::iterator b)
{
	double d = (*a)->rayon(**b);
	return (d < TOLERANCE_DISTANCE);
}

void printVector(std::vector<Obstacle*> v)
{	
	for(std::vector<Obstacle*>::iterator it=v.begin();it!=v.end();it++)
	{
		std::cout << (*it)->getX() << " " << (*it)->getY() << std::endl;
	}
}
