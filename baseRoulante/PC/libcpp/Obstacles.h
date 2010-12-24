#ifndef __OBSTACLE
#define OBSTACLE

#define TAILLE_ROBOT 200
#define TAILLE_PION 100

#define TOLERANCE_X 150
#define TOLERANCE_Y 150

#include "Point.h"
#include <Magick++.h>

#define COULEUR_ROBOT BLEU
#define	COULEUR_ADVERSE ROUGE

using namespace std;

typedef enum{ROUGE,BLEU,NEUTRE,NOIR} Couleur;


/* définition de la classe abstraite Obstacle */

class Obstacle : public Point{
	public:
		Obstacle(double x=0,double y=0,Couleur couleur=NEUTRE);
		virtual bool contientCercle(Point& pointDonne, int rayon) const ;
		virtual void draw(Magick::Image* image) const  { };
		virtual void deplacer(double newX,double newY) { };
		virtual Couleur getCouleur() const  { return m_couleur; };
		virtual Couleur couleurPlusProche() const;

		void setCouleur(Couleur couleur){ m_couleur=couleur; };
		
	protected:
		Couleur m_couleur;
		
};

/* obstacles particuliers */

class cercleObstacle : public Obstacle{
	public:
		cercleObstacle(double x=0,double y=0,Couleur m_couleur=NEUTRE);
		void draw(Magick::Image* image) const  ;
		bool contientCercle(Point& centreCercle,int rayon) const ;
		Couleur couleurPlusProche() const;
		void deplacer(double newX,double newY);
		
	private:
		int m_rayon;
};


class rectangleObstacle : public Obstacle{
	public:
		rectangleObstacle(int x=0,int y=0,int m_couleur=NOIR);
		void draw(Magick::Image image) const  ;
		Couleur couleurPlusProche() const;
		bool contientCercle(Point& centreCercle,int rayon) const ;
	private:
		int m_demiCoteX;
		int m_demiCoteY;
};


/* vérifie si un cercle est inclu dans un obstacle de la couleur donnée
 * si oui renvoie l'obstacle correspondant
 * sinon renvoie null
 */

Obstacle* contientCercle(Point centreCercle,int rayon, const vector<Obstacle*> listeObstacles, Couleur couleur);

/* parcourt la liste des obstacles avec leurs coordonnées connues et leur attribue la couleur correspondante */

void setCouleursAuto(vector<Obstacle*> listeObstacles);

#endif