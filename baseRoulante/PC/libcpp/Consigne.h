#ifndef __CONSIGNE
#define __CONSIGNE

#include <SerialStream.h>
#include <string>
#include <sstream>
#include <iostream>


using namespace LibSerial ;
using namespace std ;

class Consigne{
	private:
		
		string formaterInt(int entierDonne)const; //convertir un entier en chaine de caractère compréhensibles par les microcontrolleurs
		
	public:
		
		Consigne(int rayon=0,int angle=0);
		void transfertSerie(SerialStream& interfaceDeTransfert); //charge une consigne donnée dans une interface série (actionneur, asservissement...)
		void print() const; //affiche une consigne (debug surtout)
		
		/*
		 * accesseurs classiques
		 */
		
		void setRayon(int rayon);
		void setAngle(int angle);
		int getRayon() const;
		int getAngle() const;
		
		/*
		 * Opérateurs
		 */
		
		friend ostream &operator<<(ostream &out, Consigne consigne);
		
	private:
		
		int m_rayon;
		int m_angle;
		
};

ostream &operator<<(ostream &out, vector<Consigne> listeConsignes);

void print(const vector<Consigne>& listeConsignes);


#endif