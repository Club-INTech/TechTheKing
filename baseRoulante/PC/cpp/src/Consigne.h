#ifndef CONSIGNE_H
#define CONSIGNE_H

/*!
 * \file Consigne.h
 * \brief La gestion des consignes
 * \author Philippe Tillet.
 * \version 1.0
 */


#include <SerialStream.h>
#include <string>
#include <sstream>
#include <iostream>


using namespace LibSerial ;
using namespace std ;

/*!
 * \class Consigne
 * \brief La gestion des consignes.
 * 
 * Le role principal de cette classe est de transférer une liste de consigne vers une des cartes du robot.
 */
class Consigne{
	
		
	public:

		/*!
		 * \brief Constructeur de la classe Consigne
		 */
		Consigne(int rayon=0,int angle=0);

		/*!
		 * \brief transfertSerie
		 *
		 * Transfert les deux paramètres de la consigne (rayon et angle) vers l'interface spécifiée.
		 */
		void transfertSerie(SerialStream& interfaceDeTransfert);

		/*!
		 * \brief print
		 *
		 * Affichage d'une consigne sur la sortie standart.
		 */
		void print() const;
		
		/*!
		 * \brief Accesseurs
		 */
		void setRayon(int rayon);
		void setAngle(int angle);
		int getRayon() const;
		int getAngle() const;

		/*!
		 * \brief Opérateurs
		 *	
		 * Il n'y en a qu'un : l'opérateur de flux sortant.
		 */		
		friend ostream &operator<<(ostream &out, Consigne consigne);
		
	private:
		
		int m_rayon;
		int m_angle;
		
};
 
 /*!
 * \brief Opérateur de flux sortant pour une liste de consignes.
 *
 * Se sert de la fonction print qui affiche le résultat sur la sortie standart
 */
ostream &operator<<(ostream &out, vector<Consigne> listeConsignes);

/*!
 * \brief formaterInt
 *
 * Converti un entier en chaine de caractère compréhensibles par les microcontroleurs.
 */
string formaterInt(int entierDonne);

namespace ListeConsignes{
void transfertSerie(vector<Consigne>& listeConsignes,SerialStream& interfaceDeTransfert);
}


#endif
