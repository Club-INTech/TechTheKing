#ifndef __ASTAR
#define __ASTAR

#include "Point.h"
#include "Obstacles.h"
#include <algorithm>

/* le périmetre dans lequel le robot se déplacera pour aller bouger un pion à l'adversaire... */

#define RAYON_DE_DETECTION 700

/* l'empietement du robot sur le pion adverse, lorsque celui ci n'est pas sur le chemin le plus court */

#define EMPIETEMENT 400

/* la classe noeud, indispensable pour AStar */

class Noeud : public Point{
	public:
		
		Noeud(int x=0,int y=0, double cout1=0,double cout2=0);
		~Noeud();
		
		/* Opérateur de comparaison entre deux noeuds */
		
		bool operator>(Noeud noeudDonne) const;
		
		/* accesseurs */
		
		double getCout1() const;
		double getCout2() const;
		double getCout3() const;
		
		Noeud* getParent() const;
		
		void setCout1(double cout1);
		void setCout2(double cout2);
		void setCout3(double cout3);
		
		void setDistancePionAdverse(double distance);
		void setParent(Noeud* parent);
	
	private:
		
		double m_cout1;
		double m_cout2;
		double m_cout3;

		double m_distancePionAdverse;
		
		Noeud* m_parent;
};


/*
 * Tout ce qui concerne l'algorithme A*....
 */

class AStar {
	
	private:
		
		void ajouterCasesAdjacentes(Noeud noeud);
		Noeud trouverMeilleurNoeud();
		void remonterChemin();
		void transfererNoeud(Noeud noeud);
		void trouverChemin();
		
	public:
		
		AStar(int precision, Noeud depart, Noeud arrivee, vector<Obstacle*>listeObstacles);
		
		vector<Point> getChemin();
		
	private:
		
		vector<Noeud> m_listeOuverte;
		
		vector<Noeud> m_listeFermee;
		
		vector<Obstacle*> m_listeObstacles;
		
		int m_precision;
		
		Noeud m_depart;
		
		Noeud m_arrivee;
		
		vector<Point> m_chemin;
		
};

/* Point est noeud est il dans une liste? */

int estDansListe(const vector<Noeud>& listeNoeuds, Point point);

#endif