%module Obstacles


%{
    #include "Obstacles.h"
    #include "Point.h"
    #include "Contrats.h"
%}

%rename(__different__) Point::operator!=;
%rename(__print__) operator<<;

%inline %{
    std::vector <Obstacle*> listeObstacles ;
%}

#include "Contrats.h"

class Point{
    public:
        Point(double x=0,double y=0);

        void print();

        void round();
        
        double rayon(Point Point2);

        double angle(Point Point2);

        void setX(double x);

        void setY(double y);

        double getX();

        double getY();
        
        template<typename T> Point operator*(T k){
            Point resultat(m_x*k,m_y*k);
            return resultat;
        }

        template<typename T> Point operator/(T k){
            Point resultat(m_x/k,m_y/k);
            return resultat;
        }

        Point operator+(Point Point2);
        
        Point operator-(Point Point2);

        bool operator==(Point Point2);
        bool operator!=(Point Point2);

        friend ostream &operator<<(ostream &out, Point point);
        

        
        
    protected:
        
        double m_x; /*!< Abscisse du point*/
        double m_y; /*!< Ordonnée du point*/
        BEGIN_INVARIANT_BLOCK(Point)
        INVARIANT(0<=m_x, "Abscisse du point positive");
        INVARIANT(m_x<=3000, "Abscisse du point inférieure à 3000");
        INVARIANT(0<=m_y, "Ordonnée du point positive");
        INVARIANT(m_y<=2100, "Ordonnée du point inférieure à 2100");
        END_INVARIANT_BLOCK;
};


ostream &operator<<(ostream &out, vector<Point> listePoints);


namespace ListePoints{

vector<Point> lissageBezier(const vector<Point>& pointsDeControle,int nbPointsBezier) ;

vector<Consigne> convertirEnConsignes(vector<Point>& listePoints) ;

int estDansListe(vector<Point>& listePoints, Point point);
}

typedef enum
{
    ROUGE, /*!< Le pion est au robot rouge. */
    BLEU, /*!< Le pion est au robot bleu. */
    NEUTRE, /*!< Le pion n'est à personne.*/
    NOIR /*!<L'obstacle est une planche de bois*/
}
Couleur;

class Obstacle : public Point{
    
    public:

        Obstacle(double x=0,double y=0,Couleur couleur=NEUTRE);

        virtual bool contientCercle(int centreX, int centreY, int rayon) = 0;

#ifdef DEBUG_GRAPHIQUE

        virtual void draw(Magick::Image* image) = 0;
#endif
        
        void deplacer(double newX,double newY) { m_x=newX; m_y=newY; };

        virtual Couleur couleurPlusProche();

        virtual Couleur getCouleur()  { return m_couleur; };
        
        void setCouleur(Couleur couleur){ m_couleur=couleur; };
        
    protected:
        
        Couleur m_couleur;
        
};

class CercleObstacle : public Obstacle{
    
    public:
        
        CercleObstacle(double x,double y,Couleur m_couleur=NEUTRE);

        
#ifdef DEBUG_GRAPHIQUE
        void draw(Magick::Image* image);
#endif
        
        
        bool contientCercle(int centreX, int centreY, int rayon);
        
        Couleur couleurPlusProche();
        
    private:
        
        int m_rayon;
};


class RectangleObstacle : public Obstacle{
    
    public:
        
        RectangleObstacle(double x,double y,int demiCoteX,int demiCoteY);

#ifdef DEBUG_GRAPHIQUE
        void draw(Magick::Image* image);
#endif

        bool contientCercle(int centreX, int centreY, int rayon);
        
        Couleur couleurPlusProche(){return NOIR;};
        
    private:
        
        int m_demiCoteX;
        int m_demiCoteY;
};

namespace ListeObstacles{

Obstacle* contientCercle(int centreX,int centreY,int rayon, Couleur couleur);

void setCouleursAuto();

void refreshPositions(const char nomFichier[]);

void initialisation();
}

