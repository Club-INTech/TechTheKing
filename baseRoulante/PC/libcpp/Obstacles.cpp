#include "Obstacles.h"

Obstacle::Obstacle(double x,double y,Couleur couleur){
	m_x=x;
	m_y=y;
	m_couleur=couleur;
}

bool Obstacle::contientCercle(Point& centreCercle, int rayon) const{
	return false;
}

Couleur Obstacle::couleurPlusProche() const {
	return NEUTRE;
}

cercleObstacle::cercleObstacle(double x,double y,Couleur couleur) : Obstacle(x,y,couleur),m_rayon(TAILLE_PION)
{

}

void cercleObstacle::draw(Magick::Image* image) const{
	image->fillColor("yellow");
	image->draw(Magick::DrawableEllipse(m_x,2100-m_y, m_rayon, m_rayon, 0, 360));
}

bool cercleObstacle::contientCercle(Point& centreCercle, int rayonDonne) const{
	if(rayon(centreCercle) < (m_rayon + rayonDonne))
		return true;
	return false;
}

void cercleObstacle::deplacer(double newX, double newY) {
	m_x=newX;
	m_y=newY;
}

/* Determination automatique de la couleur d'un pion : si il est à nous, à l'adversaire ou à personne */
Couleur cercleObstacle::couleurPlusProche() const{
	Couleur couleurCase=ROUGE;
	for(unsigned j=175;j<=1925;j+=350)
	{
		for(unsigned i=625;i<=2375;i+=350)
		{
			if(couleurCase==BLEU)
			{
				if(fabs(m_x - i)<TOLERANCE_X && fabs(m_y - j)<TOLERANCE_Y){
					return BLEU;
				}
				couleurCase=ROUGE;
			}
			else{
				if(fabs(m_x - i)<TOLERANCE_X && fabs(m_y - j)<TOLERANCE_Y){
					return ROUGE;
				}
				couleurCase=BLEU;
			}
		}
		if(couleurCase==BLEU)
		{
			couleurCase=ROUGE;
		}
		else
		{
			couleurCase=BLEU;
		}
	}
	return NEUTRE;
}
				
		
		
	

bool rectangleObstacle::contientCercle(Point& centreCercle, int rayon)const {
	if(fabs(centreCercle.getX()-m_x) < m_demiCoteX+rayon && fabs(centreCercle.getY()-m_y) < m_demiCoteY+rayon)
		return true;
	return false;
}


/* cette fonction regarde si un cercle est inclu dans un obstacle d'une couleur donnée. 
 * Renvoie l'obstacle le plus proche trouvé, ou Null si il n'y en a pas...
 */

Obstacle* contientCercle(Point centreCercle,int rayon, const vector<Obstacle*> listeObstacles,Couleur couleur){
	vector<Obstacle*> tmp; // contient la liste des correspondances.
	Obstacle* min=NULL;
	for(unsigned int i=0;i<listeObstacles.size();i++){
		if(listeObstacles[i]->contientCercle(centreCercle,rayon) && listeObstacles[i]->getCouleur() == couleur)
			tmp.push_back(listeObstacles[i]);
	}
	if(tmp.size()>0)
		min=tmp[0];
	for(unsigned int i=0;i<tmp.size();i++){
		if(tmp[i]->rayon(centreCercle)<min->rayon(centreCercle))
			min=tmp[i];
	}
		return min;
}

void setCouleursAuto(vector<Obstacle*> listeObstacles){
	for(unsigned int i=0;i<listeObstacles.size();i++)
		listeObstacles[i]->setCouleur(listeObstacles[i]->couleurPlusProche());
}