#include "AStar.h"


//ce qui concerne les noeuds



Noeud::Noeud(int x,int y,int cout1,int cout2,int cout3){
	Point(x,y);
	m_cout1=cout1;
	m_cout1=cout2;
	m_cout1=cout3;
}

bool Noeud::operator>(Noeud noeud2) const{
	(m_cout3>noeud2.m_cout3)?true:false;
}


//ce qui concerne AStar


AStar::AStar(int precision=10, Noeud depart, Noeud arrivee, vector<Obstacle>listeObstacles){
	m_precision=precision;
	m_depart=depart;
	m_arrivee=arrivee;
	m_listeObstacles=listeObstacles;
};


void AStar::ajouterCasesAdjacentes(Point point){
	Noeud tmp; // création d'un noeud temporaire
	for(int i=point.m_x-m_precision;i<=point.m_x+m_precision;i+=m_precision){
		(i<0 || i>3000)?continue; //Si on est en dehors de la carte, on ignore...
		for (int j=point.m_y-m_precision;j<=point.m_y+m_precision;j+=m_precision){
			(j<0 || j>2100)?continue; //si on est en dehors de la carte, on ignore...
			(i=point.m_x && j=point.m_y)?continue; //on est à l'étape actuelle, on ignore...
			(m_listeObstacles.contientPoint(point))?continue; // si le point est dans un obstacle, on ignore...
			Noeud noeudCourant(i,j);
			if(!estDansListe(m_listeFermee,noeudCourant)){ //si le a déjà été étudié, on ne fait rien, sinon...
				tmp.cout1=noeudCourant.distance(m_depart);
				tmp.cout2=noeudCourant.distance(m_arrivee);
				tmp.cout3=tmp.cout1+tmp.cout2;
				tmp.parent=point;
				if(estDansListe(m_listeOuverte,noeudCourant)) //si le noeud est déjà présent dans la liste ouverte...
				
			
}

Noeud AStar::trouverMeilleurNoeud(){
	Noeud meilleurNoeud=m_listeOuverte[0];
	for(int i=0;i<m_listeOuverte.size();i++)
		(m_listeOuverte[i]>meilleurNoeud)?meilleurNoeud=m_listeOuverte[i];
	return meilleurNoeud;
}

void AStar::remonterChemin(){
	
}