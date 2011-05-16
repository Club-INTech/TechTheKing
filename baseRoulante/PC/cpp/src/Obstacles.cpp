#include "Obstacles.h"


Obstacle::Obstacle(double x,double y,Couleur couleur){
    m_x=x;
    m_y=y;
    m_couleur=couleur;
}


Couleur Obstacle::couleurPlusProche() {
    return NEUTRE;
}

CercleObstacle::CercleObstacle(double x,double y,Couleur couleur) : Obstacle(x,y,couleur),m_rayon(TAILLE_PION)
{

}

#ifdef DEBUG_GRAPHIQUE
void CercleObstacle::draw(Magick::Image* image){
    image->fillColor("yellow");
    image->draw(Magick::DrawableEllipse(m_x,2100-m_y, m_rayon, m_rayon, 0, 360));
}
#endif

bool CercleObstacle::contientCercle(int centreX, int centreY, int rayonDonne){
    double dx=(centreX-m_x);
    double dy=(centreY-m_y);
    if(dx*dx+dy*dy < (m_rayon + rayonDonne)*(m_rayon+rayonDonne))
        return true;
    return false;
}

Couleur RectangleObstacle::couleurPlusProche(){
	return NOIR;
}

/* Determination automatique de la couleur d'un pion : si il est à nous, à l'adversaire ou à personne */
Couleur CercleObstacle::couleurPlusProche() {
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
                
        
RectangleObstacle::RectangleObstacle(double x,double y,int demiCoteX,int demiCoteY) : Obstacle(x,y,NOIR),m_demiCoteX(demiCoteX),m_demiCoteY(demiCoteY)
{
    
}

#ifdef DEBUG_GRAPHIQUE
void RectangleObstacle::draw(Magick::Image* image){
    image->fillColor("Dark Orange");
    image->draw(Magick::DrawableRectangle(m_x-m_demiCoteX,2100-(m_y-m_demiCoteY),m_x+m_demiCoteX,2100-(m_y+m_demiCoteY)));
}
#endif

bool RectangleObstacle::contientCercle(int centreX, int centreY, int rayonDonne) { //cercle non penché.
    double dx=(centreX-m_x);
    double dy=(centreY-m_y);
        return ( sqrt(dx*dx+dy*dy) < sqrt(m_demiCoteX*m_demiCoteX + m_demiCoteY*m_demiCoteY) + rayonDonne);
}

Obstacle* ListeObstacles::contientCercle(int centreX, int centreY, int rayon,Couleur couleur){
    vector<Obstacle*> tmp; // contient la liste des correspondances.
    Obstacle* min=NULL;
    for(unsigned int i=0;i<listeObstacles.size();i++){
        if(listeObstacles[i]->contientCercle(centreX,centreY,rayon) && listeObstacles[i]->getCouleur() == couleur)
            tmp.push_back(listeObstacles[i]);
    }
    if(tmp.size()>0)
        min=tmp[0];
    double dx;
    double dy;
    double dxs;
    double dys;
    for(unsigned int i=0;i<tmp.size();i++){
        dx=(centreX-tmp[i]->getX());
        dy=(centreY-tmp[i]->getY());
        dxs=(centreX-min->getX());
        dys=(centreY-min->getY());
        if(dx*dx+dy*dy<dxs*dxs+dys*dys)
            min=tmp[i];
    }
        return min;
}

void ListeObstacles::setCouleursAuto(){
    for(unsigned int i=0;i<listeObstacles.size();i++){
        listeObstacles[i]->setCouleur(listeObstacles[i]->couleurPlusProche());
    }
}

void ListeObstacles::initialisation(){
    listeObstacles.push_back(new RectangleObstacle(200,1689,200,11));
    listeObstacles.push_back(new RectangleObstacle(2800,1689,200,11));
    listeObstacles.push_back(new RectangleObstacle(800,60,350,60));
    listeObstacles.push_back(new RectangleObstacle(461,185,11,65));
    listeObstacles.push_back(new RectangleObstacle(1139,185,11,65));
    listeObstacles.push_back(new RectangleObstacle(2200,60,350,60));
    listeObstacles.push_back(new RectangleObstacle(1861,185,11,65));
    listeObstacles.push_back(new RectangleObstacle(2539,185,11,65));
}
    
void ListeObstacles::refreshPositions(const char nomFichier[]){
    listeObstacles.clear();
    ifstream fichierObstacles(nomFichier, ios::in);
    if(fichierObstacles)
    {
        double x,y;
        while(fichierObstacles >> x >> y){
            CercleObstacle* pion = new CercleObstacle(x,y);
            listeObstacles.push_back(pion);
        }
    }
        else
            cerr<<"ouverture du fichier impossible"<<endl;
}
