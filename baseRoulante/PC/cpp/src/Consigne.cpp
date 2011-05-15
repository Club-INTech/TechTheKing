#include "Consigne.h"
#include <time.h>

string Consigne::formaterInt(int entierDonne)const{
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

void Consigne::transfertSerie(SerialStream& interfaceDeTransfert){
	if(m_rayon>=0)
		interfaceDeTransfert.write(("g1" + formaterInt(m_rayon)).c_str(),9);
	else
		interfaceDeTransfert.write(("g0" + formaterInt(-m_rayon)).c_str(),9);
	if(m_angle>=0)
		interfaceDeTransfert.write(("q1" + formaterInt(m_angle)).c_str(),9);
	else
		interfaceDeTransfert.write(("q0" + formaterInt(-m_angle)).c_str(),9);
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

void ListeConsignes::transfertSerie(vector<Consigne>& listeConsignes, SerialStream& liaisonSerie){
	liaisonSerie.write("o",1);
	for(vector<Consigne>::iterator it=listeConsignes.begin();it!=listeConsignes.end();it++)
	{
		it->transfertSerie(liaisonSerie);	
	}
	liaisonSerie.write("p",1);
}
