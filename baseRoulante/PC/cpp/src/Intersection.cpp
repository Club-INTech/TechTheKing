#include <vector>
#include "Obstacles.h"

#define TOLERANCE_DISTANCE 10

bool comparerPions(std::vector<Obstacle*>::iterator a, std::vector<Obstacle*>::iterator b);
void printVector(std::vector<Obstacle*> v);
std::vector< std::pair<Obstacle*,int> > fusionResultats(std::vector<Obstacle*> t1, std::vector<Obstacle*> t2, std::vector<Obstacle*> t3, int niveau);
void ajouterPion(std::vector< std::pair<Obstacle*,int> > &v, std::vector<Obstacle*>::iterator p);

// Intersection des résultats (au moins 2 téléphones nécessaires)
std::vector< std::pair<Obstacle*,int> > fusionResultats(std::vector<Obstacle*> t1, std::vector<Obstacle*> t2, std::vector<Obstacle*> t3, int niveau)
{
	std::vector<Obstacle*>::iterator it;
	std::vector< std::pair<Obstacle*,int> > resultatFusion;
	
	// Une double boucle pour 3 instructions c'est mal
	for(it=t1.begin();it!=t1.end();it++)
	{
		ajouterPion(resultatFusion,it);
	}
	
	for(it=t2.begin();it!=t2.end();it++)
	{
		ajouterPion(resultatFusion,it);
	}
	
	for(it=t3.begin();it!=t3.end();it++)
	{
		ajouterPion(resultatFusion,it);
	}
	
	// Tri des résultats en fonction du niveau
	for(it=resultatFusion.begin();it!=resultatFusion.end();it++)
	{
		if (it.second < niveau) resultatFusion.erase(it);
	}
	
	return resultatFusion;
}

// True si les 2 pions sont les mêmes, false sinon 
bool comparerPions(std::vector<Obstacle*>::iterator a, std::vector<Obstacle*>::iterator b)
{
	double d = (*a)->rayon(**b);
	return (d < TOLERANCE_DISTANCE);
}

// Vérifie si un pion est déjà présent, l'ajoute sinon
void ajouterPion(std::vector< std::pair<Obstacle*,int> > &v, std::vector<Obstacle*>::iterator p)
{
	std::vector<Obstacle*>::iterator it;
	bool present = false;
	
	for(it=v.begin();it!=v.end();it++)
	{
		// Si pion déjà présent, on augmente le poids
		if (comparerPions(it,p))
		{
			present = true;
			
			// Calcul du barycentre
			(*(it.first))->setX( ( it.second * (*(it.first))->getX() + (*p)->getX() ) / (it.second + 1) );
			(*(it.first))->setY( ( it.second * (*(it.first))->getY() + (*p)->getY() ) / (it.second + 1) );
			
			// Incrémentation du poids
			it.second += 1;
		}
	}
	
	// Ajout du pion s'il n'est pas déjà présent
	if (!present)
	{
		// new CercleObstacle((*p)->getX(),(*p)->getY()) ?
		v.push_back(std::make_pair<Obstacle*,int>(new CercleObstacle((*p)->getX(),(*p)->getY()),1));
	}
}

void printVector(std::vector<Obstacle*> v)
{	
	for(std::vector<Obstacle*>::iterator it=v.begin();it!=v.end();it++)
	{
		std::cout << (*it)->getX() << " " << (*it)->getY() << std::endl;
	}
}
