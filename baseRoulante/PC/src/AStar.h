#ifndef ASTAR_H
#define ASTAR_H

#include "Point.h"
#include "Obstacles.h"
#include <algorithm>
#include <list>

/*!
 *\def RAYON_DE_DETECTION
 *
 * le périmetre dans lequel le robot se déplacera pour aller bouger un pion à l'adversaire...
 */
#define RAYON_DE_DETECTION 350

/*!
 * \def EMPIETEMENT
 * l'empietement du robot sur le pion adverse, lorsque celui ci n'est pas sur le chemin le plus court
 */
#define EMPIETEMENT 100

/*!
 * \class Noeud
 *
 * La classe Noeud, indispensable pour tout algorithme de pathfinding.
 */
class Noeud : public Point{
	
	
	public:

		/*!
		 * \brief Constructeur
		 * \param cout1 : Distance au point de départ
		 * \param cout2 : Distance au point d'arrivee
		 */
		Noeud(int x=0,int y=0, double cout1=0,double cout2=0);

		/*!
		 * \brief Destructeur
		 */
		~Noeud();
		
		/*!
		 *\brief Accesseurs
		 */
		double getCout1();
		double getCout2();
		double getCout3();	
		Noeud* getParent();
		bool getCollision(){ return m_collision ; };
		
		void setCout1(double cout1);
		void setCout2(double cout2);
		void setCout3(double cout3);
		void setCollision(bool etat) { m_collision = etat ;};
		
		void setDistancePionAdverse(double distance);
		void setParent(Noeud* parent);

		/*!
		 * \brief Opérateur de comparaison entre deux noeuds
		 * Sert pour déterminer le "meilleur" chemin.
		 */
		bool operator>(Noeud noeudDonne);
		void operator=(Point pointDonne);
		
	private:
		
		double m_cout1; /*!< Distance du noeud au point de départ.*/
		double m_cout2; /*!< Distance du noeud au point d'arrivee.*/
		double m_cout3; /*!< Somme des deux couts précédents.*/

		double m_distancePionAdverse; /*!< La distance qui sépare un noeud d'un pion adverse. Nécéssaire pour l'attraction de la trajectoire par ce point.*/

		bool m_collision; /*!< si on emp^eche toute collision avec les obstacles, ça va planter à cause des imprécisions. Il faut juste les éviter*/
		
		Noeud* m_parent; /*!< Le parent du noeud. Necessaire pour retrouver son chemin*/		
};


/*!
 * \class AStar
 * \brief Tout ce qui concerne l'algorithme A*....
 */
class AStar {
	
	private:
		
		/*!
		 * \brief trouverChemin
		 * C'est la boucle principale de recherche.
		 */
		void trouverChemin();


		/*!
		 * \brief trouverMeilleurNoeud
		 * Boucle, Etape 1 : Cherche le meilleur noeud de la liste ouverte pour continuer l'algorithme au meilleur endroit.
		 */
		list<Noeud*>::iterator trouverMeilleurNoeud();

		/*!
		 * \brief ajouterCasesAdjacentes
		 * Boucle, Etape 2 : Le principe de l'algorithme A* est d 'ajouter les cases de recherche au fur et à mesure
		 */
		void ajouterCasesAdjacentes(Noeud* noeud);

		/*!
		 * \brief transfererNoeud
		 * Boucle, Etape 3 : Transfert d'un noeud de la liste ouverte vers la liste fermée (le noeud a été étudié.)
		 */
		void transfererNoeud(Noeud* noeud);

		/*!
		 * \brief remonterChemin
		 * Après la boucle, on doit remonter le chemin de parent en parent
		 */
		void remonterChemin();
	public:
		
		AStar(int precision=50);
		void setPrecision(int precision);
		vector<Point> getChemin(Point depart, Point arrivee);
		
	private:
		
		list<Noeud*> m_listeOuverte;
		
		list<Noeud*> m_listeFermee;
		
		int m_precision;
		
		Noeud m_depart;
		
		Noeud m_arrivee;
		
		vector<Point> m_chemin;
		
};

/* Point est noeud est il dans une liste? */

list<Noeud*>::iterator estDansListe(list<Noeud*>& listeNoeuds, int x, int y);

#endif
