#ifndef __ASTAR
#define __ASTAR

#include "Point.h"
#include "Obstacles.h"

/*
 * la classe noeud, indispensable pour AStar
 */

class Noeud : public Point{
	public:
		
		Noeud(int x=0,int y=0, int cout1=0,int cout2=0,int cout3=0);
		
		bool operator>(Noeud noeudDonne) const; //comparaison des deux noeuds via leur cout3.
		
	private:
		
		int m_cout1;
		int m_cout2;
		int m_cout3;
		
		Point m_parent;
};


/*
 * Tout ce qui concerne l'algorithme A*....
 */

class AStar {
	private:
		void ajouterCasesAdjacentes(Point point);
		Noeud trouverMeilleurNoeud();
		void remonterChemin();
	public:
		AStar(int precision=10, Noeud depart, Noeud arrivee, vector<Obstacle>listeObstacles);
		void trouverChemin();
		vector<Point> getChemin();
	private:
		vector<Noeud> m_listeOuverte;
		vector<Noeud> m_listeFermée;
		vector<Obstacle> m_listeObstacles;
		int m_precision;
		Noeud m_depart;
		Noeud m_arrivee;
		vector<Point> m_chemin;
};
		
	

#endif