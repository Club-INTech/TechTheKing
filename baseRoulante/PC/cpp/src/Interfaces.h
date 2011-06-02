#ifndef INTERFACES_H
#define INTERFACES_H

#include <iostream>
#include <bitset>
#include <stack>
#include "Point.h"
#include "AStar.h"
#include <SerialPort.h>
//#include "Singleton.h"
#include "Thread.h"
#include "config.h"

enum SensDeplacement {POSITIF, NEGATIF};
enum ModeAimant {BAS, HAUT};
enum Bras {BGAUCHE = 0X41, BDROITE = 0X42};
enum Niveau {SOCLE, MILIEU, TOUR, CAPTURE};
enum Orientation {BALAYAGE, CENTRE, REPLIE, DROIT}; 

std::string exec(char* cmd);
class InterfaceAsservissement;
std::vector<char> getTtyUSB();

void setCouleurRobot(Couleur couleur);
Couleur getCouleurRobot();

class InterfaceAsservissement {
public:
    static InterfaceAsservissement* Instance();
    ~InterfaceAsservissement();
    friend void detectionSerieUsb(InterfaceAsservissement* asserv); // ne devrait pas servir si on garde l'i2c
    int getDistanceRobot();
    int getAngleRobot();
    void goTo(Point arrivee,int nbPoints);
    void pwmMaxTranslation(unsigned char valPWM);
    void pwmMaxRotation(unsigned char valPWM);
    void recalage();
    void reGoTo();
    void avancer(unsigned int distanceMm);
    void reculer(unsigned int distanceMm);
    void tourner(double angleRadian);
    void stop();
    void stopAll();
    #ifdef DEBUG_GRAPHIQUE
    void debugGraphique();
    #endif
    void debugConsignes();
    int getXRobot();
    int getYRobot();
    void setXRobot(int xMm);
    void setYRobot(int yMm);
    void setEvitement();
    void ecrireSerie(std::string msg);
    void actualiserCouleurRobot();
private:
    InterfaceAsservissement& operator=(const InterfaceAsservissement&);
    InterfaceAsservissement(std::string port, int precisionAStar);
    void recupPosition();
    void attendreArrivee();
    int readInt();
    inline void eviter();
private:
	bool m_evitement;
    Point m_lastArrivee;
    int m_lastNbPoints;
    int m_compteurImages;
    vector<Point> m_lastTrajectory;
    vector<Consigne> m_lastListeConsignes;
    static InterfaceAsservissement* m_instance;
    AStar m_pathfinding;
    unsigned int vitesseMax;
    SerialPort m_serialPort;
    boost::mutex m_evitement_mutex;
    boost::mutex  m_serial_mutex;
    std::string m_port;
};

// Interface passive : capteurs. A priori, pas besoin de méthode publique autre que ouvrirThread.

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
	boost::thread m_thread_finMatch;
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

#endif
