#include "Obstacles.h"


std::vector <Obstacle*> listeObstacles ;


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
				
		
rectangleObstacle::rectangleObstacle(double x,double y,int demiCoteX,int demiCoteY) : Obstacle(x,y,NOIR),m_demiCoteX(demiCoteX),m_demiCoteY(demiCoteY)
{
	
}

void rectangleObstacle::draw(Magick::Image* image) const{
	image->fillColor("Dark Orange");
	image->draw(Magick::DrawableRectangle(m_x-m_demiCoteX,2100-(m_y-m_demiCoteY),m_x+m_demiCoteX,2100-(m_y+m_demiCoteY)));
}
	

bool rectangleObstacle::contientCercle(Point& centreCercle, int rayon)const {
	return(rayon > fabs( (m_y+m_demiCoteY-centreCercle.getY()) )
		|| rayon > fabs( (m_y-m_demiCoteY-centreCercle.getY()) )
		|| rayon > fabs( (m_x-m_demiCoteX-centreCercle.getX()) )
		|| rayon > fabs( (m_y+m_demiCoteX-centreCercle.getX()) )
	);
}

Obstacle* ListeObstacles::contientCercle(Point centreCercle,int rayon,Couleur couleur){
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

void ListeObstacles::setCouleursAuto(){
	for(unsigned int i=0;i<listeObstacles.size();i++)
		listeObstacles[i]->setCouleur(listeObstacles[i]->couleurPlusProche());
}

void ListeObstacles::refreshPositions(const char nomFichier[]){
	listeObstacles.clear();

	listeObstacles.push_back(new rectangleObstacle(200,1689,200,11));
	listeObstacles.push_back(new rectangleObstacle(2800,1689,200,11));
	listeObstacles.push_back(new rectangleObstacle(800,60,350,60));
	listeObstacles.push_back(new rectangleObstacle(461,185,11,65));
	listeObstacles.push_back(new rectangleObstacle(1139,185,11,65));
	listeObstacles.push_back(new rectangleObstacle(2200,60,350,60));
	listeObstacles.push_back(new rectangleObstacle(1861,185,11,65));
	listeObstacles.push_back(new rectangleObstacle(2539,185,11,65));

	ifstream fichierObstacles(nomFichier, ios::in);
	if(fichierObstacles)
	{
		double x,y;
		while(fichierObstacles >> x >> y){
			cercleObstacle* pion = new cercleObstacle(x,y);
			listeObstacles.push_back(pion);
		}
	}
		else
			cerr<<"ouverture du fichier impossible"<<endl;
}