#include "Consigne.h"
#include <time.h>

string formaterInt(int entierDonne) {
	ostringstream fluxDeSortie;
	fluxDeSortie << entierDonne;
	string resultat=fluxDeSortie.str(); //l'entier est converti en string.
	
	/*
	 * format "presque" compréhensible par les microcontrolleurs.
	 * il manque en effet un caractère pour spécifier le sens de marche du moteur (1 : avance, 0 : recule)
	 */
	resultat.insert(0,7-resultat.size(),'0');
	
	return resultat;
}	


Consigne::Consigne(int rayon,int angle){
	m_rayon=rayon;
	m_angle=angle;
}

void Consigne::transfertSerie(SerialPort& interfaceDeTransfert){
	usleep(1000);
	if(m_rayon>=0)
		interfaceDeTransfert.Write("g1" + formaterInt(m_rayon));
	else
		interfaceDeTransfert.Write("g0" + formaterInt(-m_rayon));
	if(m_angle>=0)
		interfaceDeTransfert.Write("q1" + formaterInt(m_angle));
	else
		interfaceDeTransfert.Write("q0" + formaterInt(-m_angle));
}

void Consigne::print()const{
	if(m_rayon>=0)
		cout << "g1" + formaterInt(m_rayon) << endl;
	else
		cout << "g0" + formaterInt(-m_rayon) << endl;
	if(m_angle>=0)
		cout << "q1" + formaterInt(m_angle) << endl;
	else
		cout << "q0" + formaterInt(-m_angle) << endl;
}

/*
 * accesseurs classiques
 */

void Consigne::setRayon(int rayon){
	m_rayon=rayon;
}

void Consigne::setAngle(int angle){
	m_angle=angle;
}

int Consigne::getRayon() const{
	return m_rayon;
}

int Consigne::getAngle() const{
	return m_angle;
}
/*
 * Opérateurs
 */

ostream &operator<<(ostream &out, Consigne consigne){
	consigne.print();
	return out;
}

ostream &operator<<(ostream &out, vector<Consigne> listeConsignes){
	for(vector<Consigne>::iterator it=listeConsignes.begin();it!=listeConsignes.end();it++)
		it->print();
	return out;
}

void ListeConsignes::transfertSerie(vector<Consigne>& listeConsignes, SerialPort& liaisonSerie){
	for(vector<Consigne>::iterator it=listeConsignes.begin();it!=listeConsignes.end();it++)
	{
		it->transfertSerie(liaisonSerie);	
	}
}
