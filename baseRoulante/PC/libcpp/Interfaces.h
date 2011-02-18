#ifndef INTERFACES_H
#define INTERFACES_H

#include <iostream>
#include "Point.h"
#include "AStar.h"
#include <SerialPort.h>
#include "Singleton.h"


typedef enum{Positif,Negatif}SensDeplacement;

std::string exec(char* cmd);
class InterfaceAsservissement;
std::vector<char> getTtyUSB();

class Interface {
public:
	friend void detectionSerieUsb(InterfaceAsservissement* asserv); 
protected:
	Interface();
	virtual ~Interface(){};
protected:
	std::string m_idCarte;
	SerialStream m_liaisonSerie;
};

class InterfaceAsservissement : public Interface{
public:
	static InterfaceAsservissement* instance();
	static void creer(int precisionAStar);
	void goTo(Point depart, Point arrivee,int nbPoints);
	void avancer(unsigned int distance, SensDeplacement sens);
	void tourner(unsigned int angle, SensDeplacement sens);
private:
	InterfaceAsservissement(int precisionAStar);
	static InterfaceAsservissement* m_instance;
private:
	AStar m_pathfinding;
	unsigned int vitesseMax;
};

#endif
