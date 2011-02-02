#ifndef INTERFACES_H
#define INTERFACES_H

#include <iostream>
#include "Point.h"
#include "AStar.h"
#include <SerialStream.h>
#include "Singleton.h"


typedef enum{Positif,Negatif}SensDeplacement;
typedef enum{Indefini, Asservissement='0', Capteur='1', Actionneurs='2'}TypeCarte;



class Interface {
protected:
	Interface();
	std::string detectionSerieUsb();
protected:
	TypeCarte m_idCarte;
	SerialStream m_liaisonSerie;
};

class InterfaceAsservissement : public Interface,public Singleton<InterfaceAsservissement>{
public:
	InterfaceAsservissement();
	void goTo(Point depart, Point arrivee);
	void avancer(unsigned int distance, SensDeplacement sens);
	void tourner(unsigned int angle, SensDeplacement sens);
private:
	unsigned int vitesseMax;
	AStar m_pathfinding;
};

#endif