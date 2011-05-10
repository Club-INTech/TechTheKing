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


typedef enum{positif,negatif}SensDeplacement;
typedef enum{pince,independants}ModeBras;
typedef enum{bas,haut}ModeAimant;

std::string exec(char* cmd);
class InterfaceAsservissement;
std::vector<char> getTtyUSB();


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
	int getXRobot();
	int getYRobot();
	static InterfaceAsservissement* m_instance;
    AStar m_pathfinding;
    unsigned int vitesseMax;
    SerialStream m_liaisonSerie;
};

// Interface passive : capteurs. A priori, pas besoin de méthode publique autre que ouvrirThread.
class InterfaceCapteurs : public Thread {
public:
    InterfaceCapteurs();
private:
    inline void traiterAbsenceObstacle();
    inline void traiterPresenceObstacle();
    void thread();
private:
};



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
private:
    unsigned char pourcentageHauteurConversion(unsigned char pourcentage); // D'un pourcentage à une valeur entre 0 et 255
    unsigned int pourcentageAngleConversion(unsigned char pourcentage); // D'un pourcentage à une valeur entre 0 et 1023 à envoyer via i2c
    template <class T>  std::stack<unsigned char> decToBin(T dec);

private:
    bool m_modePince;
};

#endif
