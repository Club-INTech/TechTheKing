#ifndef INTERFACES_H
#define INTERFACES_H

#include <iostream>
#include <bitset>
#include <stack>
#include "Point.h"
#include "AStar.h"
#include <SerialPort.h>
#include "Singleton.h"
#include "Thread.h"
#include "config.h"

enum SensDeplacement {POSITIF, NEGATIF};
enum ModeAimant {BAS, HAUT};
enum Ultrason {UGAUCHE = 0X11, UDROITE = 0X12, UARRIERE = 0X13};
enum FinCourse {FCGAUCHE = 0X41, FCDROITE = 0X42};
enum PresencePion {OUI, NON};

std::string exec(char* cmd);
class InterfaceAsservissement;
std::vector<char> getTtyUSB();


class InterfaceAsservissement {
public:
	static InterfaceAsservissement* Instance(int precisionAStar=50);
	~InterfaceAsservissement();
    friend void detectionSerieUsb(InterfaceAsservissement* asserv); // ne devrait pas servir si on garde l'i2c
    int getDistanceRobot();
    int getAngleRobot();
    void goTo(Point arrivee,int nbPoints);
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

private:
    InterfaceAsservissement& operator=(const InterfaceAsservissement&);
    InterfaceAsservissement(const InterfaceAsservissement&){};
	InterfaceAsservissement(int precisionAStar);
    void recupPosition();
private:
	Point m_lastArrivee;
	int m_lastNbPoints;
	int m_compteurImages;
	vector<Point> m_lastTrajectory;
	vector<Consigne> m_lastListeConsignes;
	static InterfaceAsservissement* m_instance;
    AStar m_pathfinding;
    unsigned int vitesseMax;
    SerialStream m_liaisonSerie;
};

// Interface passive : capteurs. A priori, pas besoin de méthode publique autre que ouvrirThread.
class InterfaceCapteurs : public Thread {
public:
    InterfaceCapteurs();
    unsigned short DistanceUltrason( Ultrason val );
    PresencePion EtatBras ( FinCourse val );
private:
    inline void traiterAbsenceObstacle();
    inline void traiterPresenceObstacle();
    void thread();
private:
};




class InterfaceActionneurs {
    
    public:
        InterfaceActionneurs();
        ~InterfaceActionneurs();
        void hauteurBrasGauche(unsigned char pourcentageHauteur);
        void hauteurBrasDroit(unsigned char pourcentageHauteur);
        void hauteurDeuxBras(unsigned char pourcentageHauteur);
        void angleBrasGauche(unsigned char pourcentageAngle);
        void angleBrasDroit(unsigned char pourcentageAngle);
        void positionAimantGauche(ModeAimant mode);
        void positionAimantDroit(ModeAimant mode);
        void recalage(void);
        
    private:
        inline unsigned char pourcentageHauteurConversion(unsigned char pourcentage);
        inline unsigned char pourcentageAngleConversion(unsigned char pourcentage);
        
};

void ouvrir_adaptateur_i2c ();

#endif
