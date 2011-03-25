#include "main.h"

/*
 * définissons quelques variables globales ou define (selon les 
 * besoins) qu'on va utiliser pour le moteur.
 */
//pour la consigne en vitesse de rotation du moteur : 
//#define freqD 10		//fréquence demandée en tr/s
//static uint32_t consigne;	//période pour la consigne de commande

//pour la mesure des periodes de rotation du moteur
volatile uint32_t temps[2]={65535,65535};	//contient les périodes de rotation mesurées
//volatile uint8_t indT=0;	//code l'élément de temps[] à remplir
volatile uint32_t milAP=0;	//micros après top
volatile uint32_t milAV=0;	//micros avant top

//pour l'asservissement en vitesse du moteur
//static uint16_t timerAsserv=10000;	//période d'asservissement
//uint32_t asservAv = 0;	//compteur de temps pour l'asservissement
//static int16_t kP = 1000;	//coefficient de conversion entre la période de consigne et la période de commande (dépend du nombre de pas/tr du moteur et du nombre de commandes par pas).

//pour la commande du moteur
uint32_t periode = 50000;	//periode de commande du moteur, en microsecondes
uint16_t incPeriode = 5000;	//periode d'incrémentation de periode pour le démarrage, en microseconde
uint32_t incTemps = 0;	//stocke le temps écoulé depuis le début pour le démarrage
uint32_t micTemps = 0;	//stocke le temps écoulé depuis le début pour les commutions du moteur
uint8_t ind = 0;	//code la position dans le chronogramme de commande du moteur
uint8_t pas[4][2] = {
	{1,0},
	{1,1},
	{0,1},
	{0,0}
};

/**
 * et on y va pour la fonction main
 */
int main() {
	//on initialise la transmission série (pour le debug)
	uart_init();
	printlnString("Start");

	//on initialise la notion temporelle sur l'AVR
	temps_init();
	//on initialise l'alimentation des lasers (par pwm)
	//lasers_init();

	//on initialise les ports de commande du moteur en sortie
	sbi(DDRB,pinMotE);
	sbi(DDRB,pinMot1);
	sbi(DDRB,pinMot2);
	//on initialise les interruptions du top tour sur front descendant
	sbi(EICRA,ISC11);
	cbi(EICRA,ISC10);
	sbi(EIMSK,INT1);//pour activer les interruptions sur INT1
	//on initialise la pin du sélecteur de sens en input avec pull-up
	cbi(DDRC,pinSens);//input
	sbi(portSens,pinSens);//pull-up

	//on calcule la valeur de la consigne en microsecondes en fonction de la fréquence de rotation demandée
	//consigne=1000000/freqD;

	//et on y va ;)
	printlnString("Go");

	//démarrage : 
	printlnString("demarrage");
	while(1) {
		//calcul de la valeur de periode à utiliser ici (en gros pour le démarrage, on se contente pour l'instant d'une simple rampe) : 
		if (micros() > incTemps + incPeriode) {
			incTemps = micros();
			if (periode > 7700) {
				periode-=4;
			}
		}
		//commutation des bobines du moteur : 
		if (micros() > micTemps + periode) {
			//printUShort(rbi(PINC,pinSens));
			//printString("\t");
			printULong(periode);
			//printString("\t");
			//printUShort(ind);
			//printString("\t");
			//printUInt(periode);
			//printString("\t");
			//printULong(temps[1]);
			//printString("\t");
			//printlnInt(1000000/temps[1]);

			// et on commute
			micTemps = micros();
			commuter(pas[ind]);
			if (ind<3) 		ind++;
			else 			ind = 0;
			printlnString("|");
		}
	}
	return 0;
}

/**
 * fonction de commutation
 */
void commuter ( uint8_t pas_commuter[2] ) {
	if (pas_commuter[0] == 1) 
		sbi(portMot1,pinMot1);
	else 
		cbi(portMot1,pinMot1);
	if (pas_commuter[1] == 1) 
		sbi(portMot2,pinMot2);
	else 
		cbi(portMot2,pinMot2);

	return;
}

/**
 * fonction du top-tour
 */
ISR(INT0_vect) {
	cli();
	milAP=micros();
	temps[0]=temps[1];
	temps[1]=(milAP-milAV);
	milAV=milAP;
	sei();
}
