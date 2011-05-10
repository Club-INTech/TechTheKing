%module libChessUp


%{
    #include "Interfaces.h"
    #include "Thread.h"
    #include "Singleton.h"
    #include "Server.h"
%}

%inline %{
    std::vector <Obstacle*> listeObstacles ;
%}

%rename(__add__) Point::operator+;
%rename(__mul__) Point::operator*;
%rename(__sub__) Point::operator-;
%rename(print) operator<<;

typedef enum {positif,negatif} SensDeplacement;
typedef enum{pince,independants} ModeBras;
typedef enum{bas,haut} ModeAimant;

std::string exec(char* cmd);
class InterfaceAsservissement;
std::vector<char> getTtyUSB();

namespace ListeObstacles{

/*!
 * \brief contientCercle
 *
 * Existe-t-il dans la liste d'obstacles un obstacle de la couleur donnée contenant le cercle donné?
 *
 * \return Si il existe, l'obstacle en question le plus proche du centre du cercle. NULL sinon.
 */
Obstacle* contientCercle(int centreX,int centreY,int rayon, Couleur couleur);

/*!
 * \brief setCouleursAuto
 *
 * Parcourt la liste d'obstacles et attribue à chacun la couleur de la case la plus proche.
 *
 */
void setCouleursAuto();

/*!
 * \brief refreshPositions
 *
 * Recharge les coordonnees de la liste d'obstacles depuis le fichier .dat spécifié.
 *
 */
void refreshPositions(const char nomFichier[]);

/*!
 * \brief
 * 
 * Ajout des planches de bois
 * 
 */
void initialisation();
}

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
      Point operator*(float k);
      template<typename T> Point operator/(T k);
      Point operator+(Point Point2);
      Point operator-(Point Point2);
      bool operator==(Point Point2);
      bool operator!=(Point Point2);
      friend ostream &operator<<(ostream &out, Point point);
};

class Thread{
    public:
        void ouvrirThread();
        void fermerThread();
        virtual ~Thread();
    protected:
        Thread();
        virtual void thread()=0;
    protected:
        boost::thread* m_thread;
        boost::mutex m_mutex;
};

#define TAILLE_BUFFER 256
    
class Socket : public Thread{
    public:
        static Socket* Instance(int port);
        ~Socket();
        void onOpen();
        void onClose();
        void request();
};


class InterfaceAsservissement {
public:
   static InterfaceAsservissement* Instance(int precisionAStar=50);
    friend void detectionSerieUsb(InterfaceAsservissement* asserv); // ne devrait pas servir si on garde l'i2c
    void goTo(Point arrivee,int nbPoints);
    void avancer(unsigned int distance, SensDeplacement sens);
    void tourner(unsigned int angle, SensDeplacement sens);
    
private:
    InterfaceAsservissement& operator=(const InterfaceAsservissement&);
    InterfaceAsservissement(const InterfaceAsservissement&){};
   InterfaceAsservissement(int precisionAStar);
    void recupPosition();
private:
   static InterfaceAsservissement* m_instance;
    AStar m_pathfinding;
    unsigned int vitesseMax;
    SerialStream m_liaisonSerie;
};
class InterfaceCapteurs : public Thread{
public:
    InterfaceCapteurs();
};
template <class T> 
class InterfaceActionneurs {
public:
    InterfaceActionneurs();
    void hauteurBrasGauche(unsigned char pourcentageHauteur);
    void hauteurBrasDroit(unsigned char pourcentageHauteur);
    void angleBrasGauche(unsigned char pourcentageAngle);
    void angleBrasDroit(unsigned char pourcentageAngle);
    void positionAimantGauche(ModeAimant mode);
    void positionAimantDroit(ModeAimant mode);
    void setMode(ModeBras mode);
};


