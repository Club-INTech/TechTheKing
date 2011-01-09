#include "AStar.h"

/*ce qui concerne les noeuds*/

Noeud::Noeud(int x,int y,double cout1,double cout2){
	m_x=x;
	m_y=y;

	m_cout1=cout1;
	m_cout2=cout2;
	m_cout3=cout1+cout2;
	
	m_distancePionAdverse=0;

	m_collision=false;
	
	m_parent=this;
}

bool Noeud::operator>(Noeud noeud2) const{
	if(m_collision==true)
		return false;
	if(m_distancePionAdverse > TAILLE_PION+TAILLE_ROBOT-EMPIETEMENT && m_distancePionAdverse!=noeud2.m_distancePionAdverse)
		return  (m_distancePionAdverse<noeud2.m_distancePionAdverse);
	return(m_cout3<noeud2.m_cout3);
}

double Noeud::getCout1() const{
	return m_cout1;
}

double Noeud::getCout2() const{
	return m_cout2;
}

double Noeud::getCout3() const{
	return m_cout3;
}


Noeud* Noeud::getParent() const{
	return m_parent;
}

void Noeud::setCout1(double cout1){
	m_cout1=cout1;
}

void Noeud::setCout2(double cout2){
	m_cout2=cout2;
}

void Noeud::setCout3(double cout3){
	m_cout3=cout3;
}

void Noeud::setDistancePionAdverse(double distance){
	m_distancePionAdverse=distance;
}

void Noeud::setParent(Noeud* noeud){
	m_parent=noeud;
}

Noeud::~Noeud(){
}



/* ce qui concerne AStar */



vector <Point> AStar::getChemin(){
	return m_chemin;
}


AStar::AStar(int precision, Noeud depart, Noeud arrivee){
	m_precision=precision;
	m_depart=depart;
	m_arrivee=arrivee;

	trouverChemin();
};


void AStar::ajouterCasesAdjacentes(Noeud noeud){
	for(int i=noeud.getX()-m_precision;i<=noeud.getX()+m_precision;i+=m_precision){
		if(i>3000 || i < - m_precision)
			continue;
		for (int j=noeud.getY()-m_precision;j<=noeud.getY()+m_precision;j+=m_precision){
			if(j>2100 || j < - m_precision)
				continue;
			 //on est à l'étape actuelle, on ignore...
			if(i==noeud.getX() && j==noeud.getY())
				continue;
			Noeud tmp(i,j,tmp.rayon(m_depart),tmp.rayon(m_arrivee));
			 // si le point est dans un obstacle de la couleur du robot ou sur une planche de bois, on ignore pour ne pas le déplacer ou se bloquer
			if(ListeObstacles::contientCercle(noeud,TAILLE_ROBOT+m_precision+MARGE_SECURITE_PION,COULEUR_ROBOT)!=NULL
				|| ListeObstacles::contientCercle(noeud,TAILLE_ROBOT,NOIR)!=NULL
				){
				tmp.setCollision(true);
				}
			if(estDansListe(m_listeFermee,tmp)==m_listeFermee.end()){ //si le a déjà été étudié, on ne fait rien, sinon...
				Obstacle* estSurPionAdverse=ListeObstacles::contientCercle(tmp,RAYON_DE_DETECTION,COULEUR_ADVERSE);
				if(estSurPionAdverse!=NULL){
					/*si pousser le pion adverse nous fait pousser le notre aussi*/
					if(ListeObstacles::contientCercle(*estSurPionAdverse,TAILLE_PION+50,COULEUR_ROBOT)!=NULL)
						tmp.setCollision(true);
					else
					{
						tmp.setDistancePionAdverse(tmp.rayon(*estSurPionAdverse));
					}

				}
				tmp.setParent(new Noeud(noeud));
				list<Noeud>::iterator noeudCommun = estDansListe(m_listeOuverte,tmp);
				if(noeudCommun!=m_listeOuverte.end()){//si le noeud est déjà présent dans la liste ouverte...
					if(tmp>*noeudCommun){	//si le noeud actuel est meilleur, on le remplace
						m_listeOuverte.erase(noeudCommun);
						m_listeOuverte.push_back(tmp);
					}
				}
				else
					m_listeOuverte.push_back(tmp); //si le noeud n'était pas dans la liste ouverte, on le rajoute...
			}
		}
	}
}
					
			
/*
 * trouve le meilleur noeud dans la liste ouverte
 */

list<Noeud>::iterator AStar::trouverMeilleurNoeud(){
	list<Noeud>::iterator it;
	list<Noeud>::iterator meilleurNoeud=m_listeOuverte.begin();
	for(it=m_listeOuverte.begin();it!=m_listeOuverte.end();it++){
		if(*it>*meilleurNoeud)
			meilleurNoeud=it;
	}
	return meilleurNoeud;
}

/*
 * transfert un noeud de la liste ouverte vers la liste fermee
 */

void AStar::transfererNoeud(Noeud noeud){
	m_listeFermee.push_back(noeud);
	list<Noeud>::iterator indiceNoeud = estDansListe(m_listeOuverte,noeud);
	if(indiceNoeud==m_listeOuverte.end())
		cerr << "le noeud n'apparait pas dans la liste ouverte" << endl;
	else
		m_listeOuverte.erase(indiceNoeud);
}


void AStar::remonterChemin(){
	
	/*
	 * on remonte le chemin dans les noeuds
	 */
	
	
	vector <Point> listePointsTmp; //liste de points temporaire (avec des points trop proches entre eux)
	
	Noeud noeudCourant=m_arrivee;
	listePointsTmp.push_back(noeudCourant);
	while(!(*(noeudCourant.getParent())==m_depart)){
		noeudCourant=*(noeudCourant.getParent());
		listePointsTmp.push_back(noeudCourant);
	}
	
	
	/*
	 * On garde pas tous les noeuds pour alléger le futur calcul de la trajectoire de bézier, et pour réduire la courbure de la courbe . (le robot n'aime pas trop les grosses courbures)
	 */
	

	
	
	unsigned int espacement = 3; //on garde un point sur trois
	
	for(unsigned int i=0;i<listePointsTmp.size();i+=espacement)
		m_chemin.push_back(listePointsTmp[i]);
	
	m_chemin.push_back(m_depart); // on ajoute le point de départ du robot

	reverse(m_chemin.begin(), m_chemin.end()); //ne pas oublier d'inverser la liste ... mieux vaut le faire maintenant qu'avant la recopie. 
}

/*
 * C'est la boucle principale de recherche de AStar
 */

void AStar::trouverChemin(){
		m_chemin.clear();
		Noeud courant(m_depart.getX(),m_depart.getY(),0,m_depart.rayon(m_arrivee)); //initialisation du noeud courant..
		m_listeOuverte.push_back(courant);
		ajouterCasesAdjacentes(courant);
		transfererNoeud(courant);
		while(courant.getCout2()>m_precision && !(m_listeOuverte.empty())){
			courant = *trouverMeilleurNoeud();
			transfererNoeud(courant);
			ajouterCasesAdjacentes(courant);
		}
		if(courant.getCout2()<=m_precision){
			m_arrivee.setParent(&courant);
			remonterChemin();
		}
		else{
			cerr<<"pas de chemin"<<endl;
			m_chemin.push_back(m_depart);
		}
		m_listeOuverte.clear();
		m_listeFermee.clear();
}

/*
 * cette fonction teste la présence d'un élement dans la liste. Si il est présent, elle renvoie son indice, sinon, elle renvoie -1
 */

list<Noeud>::iterator estDansListe(list<Noeud>& listeNoeuds, Point point){
	list<Noeud>::reverse_iterator it;
	for(it=listeNoeuds.rbegin() ; it!=listeNoeuds.rend() ;it++){
		if(*it==point){
			return (++it).base();
		}
	}
	return (++it).base();
}