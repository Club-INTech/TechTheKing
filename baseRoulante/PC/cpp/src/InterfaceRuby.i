%module libChessUp


%{
    #include "Interfaces.h"
    #include "Thread.h"
    #include "Socket.h"
    #include "i2cLib.h"
    #include "hiddata.h"
%}

%include "../config.h"

%rename(__add__) Point::operator+;
%rename(__mul__) Point::operator*;
%rename(__sub__) Point::operator-;
%rename(print) operator<<;

enum Couleur {ROUGE, BLEU, NEUTRE, NOIR};

%inline %{
extern std::vector< std::pair<Obstacle*,int> > listeObstacles;
extern int RAYON_DE_DETECTION;
extern int EMPIETEMENT;
extern Couleur COULEUR_ROBOT;
extern Couleur COULEUR_ADVERSE;
extern double CONVERSION_RADIAN_TIC;
extern double CONVERSION_TIC_RADIAN;
extern double CONVERSION_RADIAN_ANGLE;
extern double CONVERSION_TIC_MM;
extern double CONVERSION_MM_TIC;
extern double TAILLE_ROBOT;
extern double MARGE_SECURITE_PION;
extern double TAILLE_PION;
extern double TOLERANCE_X;
extern double TOLERANCE_Y;
%}

enum SensDeplacement {POSITIF, NEGATIF};
enum ModeAimant {BAS, HAUT};
enum Bras {BGAUCHE = 0X41, BDROITE = 0X42};
enum Niveau {SOCLE, MILIEU, TOUR, CAPTURE};
enum Orientation {BALAYAGE, CENTRE, REPLIE, DROIT}; 
typedef struct usbDevice Adaptator;

std::string exec(char* cmd);
class InterfaceAsservissement;
std::vector<char> getTtyUSB();

namespace ListeObstacles {
    Obstacle* contientCercle(int centreX,int centreY,int rayon, Couleur couleur);
    void setCouleursAuto();
    void refreshPositions(const char nomFichier[]);
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
        Thread();
        void ouvrirThread();
        void fermerThread();
        virtual ~Thread();
    protected:
        virtual void thread()=0;
    protected:
        boost::thread* m_thread;
        boost::mutex m_mutex;
};

#define TAILLE_BUFFER 256
    
class Socket{
        Socket(int port);
    public:
        static Socket* Instance(int port);
        ~Socket();
        void onOpen();
        void getAllPions();
};

class InterfaceAsservissement {
public:
    static InterfaceAsservissement* Instance(int precisionAStar=50);
    ~InterfaceAsservissement();
    friend void detectionSerieUsb(InterfaceAsservissement* asserv); // ne devrait pas servir si on garde l'i2c
    int getDistanceRobot();
    int getAngleRobot();
    void goTo(Point arrivee,int nbPoints);
    void pwmMaxRotation(unsigned char valPWM);
    void pwmMaxTranslation(unsigned char valPWM);
    void recalage();
    void reGoTo();
    void avancer(unsigned int distanceMm);
    void reculer(unsigned int distanceMm);
    void tourner(double angleRadian);
    void stop();
    #ifdef DEBUG_GRAPHIQUE
    void debugGraphique();
    #endif
    void debugConsignes();
    int getXRobot();
    int getYRobot();
    void setXRobot(int xMm);
    void setYRobot(int yMm);

    
private:
    InterfaceAsservissement& operator=(const InterfaceAsservissement&);
    InterfaceAsservissement(const InterfaceAsservissement&){};
    InterfaceAsservissement(int precisionAStar);
    void recupPosition();
};

class InterfaceCapteurs : public Thread {
public:
	static InterfaceCapteurs* Instance();
    ~InterfaceCapteurs();
    unsigned short DistanceUltrason( void );
    unsigned short distanceDernierObstacle ( void );
    bool EtatBras ( Bras val );
    char LecteurCB ( void );
    void gestionJumper();
    void gestionFinMatch();
    bool EtatJumper ( void );
private:
	InterfaceCapteurs();
    inline void traiterAbsenceObstacle();
    inline void traiterPresenceObstacle();
    void thread();
private:
	unsigned short m_distanceDernierObstacle;
	static InterfaceCapteurs* m_instance;
	boost::mutex m_ultrason_mutex;
};


class InterfaceActionneurs{
    
    public:
        InterfaceActionneurs();
        ~InterfaceActionneurs();
        void hauteurBrasGauche(Niveau Hauteur);
        void hauteurBrasDroit(Niveau Hauteur);
        void hauteurDeuxBras(Niveau Hauteur);
        void angleBrasGauche(Orientation Angle);
        void angleBrasDroit(Orientation Angle);
        void positionAimantGauche(ModeAimant mode);
        void positionAimantDroit(ModeAimant mode);
        void arret(void);
};

void ouvrir_adaptateur_i2c ();
