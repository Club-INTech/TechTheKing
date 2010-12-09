#include "AStar.h"

//ce qui concerne les noeuds



Noeud::Noeud(int x,int y,int cout1,int cout2, Noeud* parent){
	m_x=x;
	m_y=y;
	m_cout1=cout1;
	m_cout2=cout2;
	m_cout3=cout1+cout2;
	m_parent=parent;
}

bool Noeud::operator>(Noeud noeud2) const{
	if(m_cout3<noeud2.m_cout3)
		return true;
	return false;
}

int Noeud::getCout1() const{
	return m_cout1;
}

int Noeud::getCout2() const{
	return m_cout2;
}

int Noeud::getCout3() const{
	return m_cout3;
}

Noeud* Noeud::getParent() const{
	return m_parent;
}

void Noeud::setCout1(int cout1){
	m_cout1=cout1;
}

void Noeud::setCout2(int cout2){
	m_cout2=cout2;
}

void Noeud::setCout3(int cout3){
	m_cout3=cout3;
}

void Noeud::setParent(Noeud* noeud){
	m_parent=noeud;
}

//ce qui concerne AStar

vector <Point> AStar::getChemin(){
	return m_chemin;
}


AStar::AStar(int precision, Noeud depart, Noeud arrivee, vector<Obstacle*>listeObstacles){
	m_precision=precision;
	m_depart=depart;
	m_arrivee=arrivee;
	m_listeObstacles=listeObstacles;
	trouverChemin();
};


void AStar::ajouterCasesAdjacentes(Noeud noeud){
	for(int i=noeud.getX()-m_precision;i<=noeud.getX()+m_precision;i+=m_precision){
		if(i<0 || i>3000)
			continue; //Si on est en dehors de la carte, on ignore...
		for (int j=noeud.getY()-m_precision;j<=noeud.getY()+m_precision;j+=m_precision){
			if(j<0 || j>2100)
				continue; //si on est en dehors de la carte, on ignore...
			if(i==noeud.getX() && j==noeud.getY())
				continue; //on est à l'étape actuelle, on ignore...
			if(contientPoint(noeud,m_listeObstacles))
				continue; // si le point est dans un obstacle, on ignore...
			Noeud tmp(i,j);
			if(estDansListe(m_listeFermee,tmp)==-1){ //si le a déjà été étudié, on ne fait rien, sinon...
				Noeud* parent = new Noeud;
				(*parent)=noeud;
				tmp.setCout1(tmp.rayon(m_depart));
				tmp.setCout2(tmp.rayon(m_arrivee));
				tmp.setCout3(tmp.getCout1()+tmp.getCout2());
				tmp.setParent(parent);
				int indiceCommun = estDansListe(m_listeOuverte,tmp);
				if(indiceCommun>-1){//si le noeud est déjà présent dans la liste ouverte...
					if(tmp>m_listeOuverte[indiceCommun]){	//si le noeud actuel est meilleur, on le remplace
						m_listeOuverte[indiceCommun]=tmp;
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

Noeud AStar::trouverMeilleurNoeud(){
	Noeud meilleurNoeud=m_listeOuverte[0];
	for(int i=0;i<m_listeOuverte.size();i++){
		if(m_listeOuverte[i]>meilleurNoeud)
			meilleurNoeud=m_listeOuverte[i];
	}
	return meilleurNoeud;
}

/*
 * transfert un noeud de la liste ouverte vers la liste fermee
 */

void AStar::transfererNoeud(Noeud noeud){
	m_listeFermee.push_back(noeud);
	int indiceNoeud = estDansListe(m_listeOuverte,noeud);
	if(indiceNoeud==-1)
		cerr << "le noeud n'apparait pas dans la liste ouverte" << endl;
	else
		m_listeOuverte.erase(m_listeOuverte.begin()+indiceNoeud);
}


void AStar::remonterChemin(){
	Noeud noeudCourant=m_arrivee;
	while(!(*(noeudCourant.getParent())==m_depart)){
		m_chemin.push_back(noeudCourant);
		noeudCourant=*(noeudCourant.getParent());
		cout<<noeudCourant<<endl;
	}
}

/*
 * C'est la boucle principale de recherche de AStar
 */

void AStar::trouverChemin(){
	Noeud courant(m_depart.getX(),m_depart.getY(),0,m_depart.rayon(m_arrivee)); //initialisation du noeud courant..
	cout<<courant.getCout2()<<endl;
	m_listeOuverte.push_back(courant);
	ajouterCasesAdjacentes(courant);
	transfererNoeud(courant);
	while(courant.getCout2()>m_precision && !(m_listeOuverte.empty())){
		courant = trouverMeilleurNoeud();
		transfererNoeud(courant);
		ajouterCasesAdjacentes(courant);
	}
	if(courant.getCout2()<=m_precision){
		m_arrivee.setParent(&courant);
		remonterChemin();
	}
	else{
		cout<<"pas de chemin"<<endl;
	}
}

/*
 * cette fonction teste la présence d'un élement dans la liste. Si il est présent, elle renvoie son indice, sinon, elle renvoie -1
 */

int estDansListe(const vector<Noeud>& listeNoeuds, Point point){
	for(int i=0;i<listeNoeuds.size();i++){
		if(listeNoeuds[i]==point){
			return i;
		}
	}
	return -1;
}


bool contientPoint(Point pointDonne,const vector<Obstacle*> listeObstacles){
	for(int i=0;i<listeObstacles.size();i++){
		if(listeObstacles[i]->contientPoint(pointDonne))
			return true;
	}
		return false;
}