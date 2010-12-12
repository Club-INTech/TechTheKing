#include "Consigne.h"
	
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

void Consigne::transfertSerie(SerialStream interfaceDeTransfert){
	if(m_angle>=0)
		interfaceDeTransfert << "g1" + formaterInt(m_angle) << endl;
	else
		interfaceDeTransfert << "g0" + formaterInt(-m_angle) << endl;
	if(m_rayon>=0)
		interfaceDeTransfert << "q1" + formaterInt(m_rayon) << endl;
	else
		interfaceDeTransfert << "q0" + formaterInt(-m_rayon) << endl;
}

void Consigne::print()const{
	if(m_angle>=0)
		cout << "g1" + formaterInt(m_angle) << endl;
	else
		cout << "g0" + formaterInt(-m_angle) << endl;
	if(m_rayon>=0)
		cout << "q1" + formaterInt(m_rayon) << endl;
	else
		cout << "q0" + formaterInt(-m_rayon) << endl;
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
	for(unsigned int i=0;i<listeConsignes.size();i++)
		listeConsignes[i].print();
	return out;
}

void print(const vector<Consigne>& listeConsignes){
	for(unsigned i=0;i<listeConsignes.size();i++){
		listeConsignes[i].print();
	}
}
