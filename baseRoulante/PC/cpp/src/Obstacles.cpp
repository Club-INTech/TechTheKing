#include "Obstacles.h"
#include "Constantes.h"


std::vector< std::pair<Obstacle*,int> > listeObstacles ;

Obstacle::Obstacle(double x,double y,Couleur couleur){
    m_x=x;
    m_y=y;
    m_couleur=couleur;
}


Couleur Obstacle::couleurPlusProche() {
    return NEUTRE;
}

CercleObstacle::CercleObstacle(double x,double y,Couleur couleur,int rayon) : Obstacle(x,y,couleur),m_rayon(rayon)
{
}

#ifdef DEBUG_GRAPHIQUE
void CercleObstacle::draw(Magick::Image* image){
    image->fillColor("yellow");
    image->draw(Magick::DrawableEllipse(m_x*900/3000,630-m_y*630/2100, m_rayon*0.3, m_rayon*0.3, 0, 360));
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
    image->draw(Magick::DrawableRectangle((m_x-m_demiCoteX)*900/3000,
		630-(m_y-m_demiCoteY)*630/2100,
		(m_x+m_demiCoteX)*900/3000,
		630-(m_y+m_demiCoteY)*630/2100));
}
#endif

bool RectangleObstacle::contientCercle(int centreX, int centreY, int rayonDonne) {
    return (m_x - m_demiCoteX - rayonDonne) < centreX
			&& centreX < (m_x + m_demiCoteX + rayonDonne)
			&& (m_y - m_demiCoteY - rayonDonne) < centreY 
			&& centreY < (m_y + m_demiCoteY + rayonDonne);
}

RobotAdverse* RobotAdverse::m_instance = NULL;

RobotAdverse::RobotAdverse() : CercleObstacle(0,0,NOIR,TAILLE_ROBOT_ADVERSE){	
}

void RobotAdverse::setCoords(int x,int y){
	m_x=x;
	m_y=y;
	#ifdef DEBUG
	std::cout<< "Robot adverse maintenant en (" << x << "," << y << ")" << std::endl;
	#endif
}

RobotAdverse* RobotAdverse::Instance(){
    if(m_instance==NULL){
       #ifdef DEBUG
         cout<<"Création du robot adverse"<<endl;
       #endif
       m_instance= new RobotAdverse();
    }
    else{
      #ifdef DEBUG
         cout<<"Récupération du robot adverse déjà créé. " <<endl;
      #endif
    }
    return m_instance;
}


Obstacle* ListeObstacles::contientCercle(int centreX, int centreY, int rayon,Couleur couleur){
    vector<Obstacle*> tmp; // contient la liste des correspondances.
    Obstacle* min=NULL;
    for(unsigned int i=0;i<listeObstacles.size();i++){
        if((listeObstacles[i].first)->contientCercle(centreX,centreY,rayon) && (listeObstacles[i].first)->getCouleur() == couleur)
            tmp.push_back((listeObstacles[i].first));
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
        (listeObstacles[i].first)->setCouleur((listeObstacles[i].first)->couleurPlusProche());
    }
}

void ListeObstacles::initialisation(){
    listeObstacles.push_back(std::make_pair<Obstacle*,int>(new RectangleObstacle(200,1689,200+TAILLE_ROBOT,11),0));
    listeObstacles.push_back(std::make_pair<Obstacle*,int>(new RectangleObstacle(2800,1689,200+TAILLE_ROBOT,11),0));
    listeObstacles.push_back(std::make_pair<Obstacle*,int>(new RectangleObstacle(800,60,350,60),0));
    listeObstacles.push_back(std::make_pair<Obstacle*,int>(new RectangleObstacle(461,185,11,65+TAILLE_ROBOT),0));
    listeObstacles.push_back(std::make_pair<Obstacle*,int>(new RectangleObstacle(1139,185,11,65+TAILLE_ROBOT),0));
    listeObstacles.push_back(std::make_pair<Obstacle*,int>(new RectangleObstacle(2200,60,350,60),0));
    listeObstacles.push_back(std::make_pair<Obstacle*,int>(new RectangleObstacle(1861,185,11,65+TAILLE_ROBOT),0));
    listeObstacles.push_back(std::make_pair<Obstacle*,int>(new RectangleObstacle(2539,185,11,65+TAILLE_ROBOT),0));
    listeObstacles.push_back(std::make_pair<Obstacle*,int>(RobotAdverse::Instance(),0));
}

void ListeObstacles::refreshPositions(const char nomFichier[]){
    listeObstacles.clear();
    ifstream fichierObstacles(nomFichier, ios::in);
    if(fichierObstacles)
    {
        double x,y;
        int note;
        while(fichierObstacles >> x >> y >> note){
            CercleObstacle* pion = new CercleObstacle(x,y);
            listeObstacles.push_back(std::make_pair(pion,note));
        }
    }
        else
            cerr<<"ouverture du fichier impossible"<<endl;
}


