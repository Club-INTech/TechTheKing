#ifndef __ASTAR
#define __ASTAR

#include "Point.h"
#include "Obstacles.h"
#include <algorithm>

/*
 * la classe noeud, indispensable pour AStar
 */

class Noeud : public Point{
	public:
		
		Noeud(int x=0,int y=0, int cout1=0,int cout2=0, Noeud* parent=NULL);
		
		bool operator>(Noeud noeudDonne) const; //comparaison des deux noeuds via leur cout3.
		
		/*
		 * accesseurs
		 */
		
		int getCout1() const;
		int getCout2() const;
		int getCout3() const;
		Noeud* getParent() const;
		void setCout1(int cout1);
		void setCout2(int cout2);
		void setCout3(int cout3);
		void setParent(Noeud* parent);
	private:
		
		int m_cout1;
		int m_cout2;
		int m_cout3;
		
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
	public:
		AStar(int precision, Noeud depart, Noeud arrivee, vector<Obstacle*>listeObstacles);
		void trouverChemin();
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

int estDansListe(const vector<Noeud>& listeNoeuds, Point point);


bool contientPoint(Point pointDonne,const vector<Obstacle*> listeObstacles);

#endif