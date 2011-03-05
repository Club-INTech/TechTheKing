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
volatile uint8_t indT=0;	//code l'élément de temps[] à remplir
volatile uint32_t milAP=0;	//micros après top
volatile uint32_t milAV=0;	//micros avant top

//pour l'asservissement en vitesse du moteur
//static uint16_t timerAsserv=10000;	//période d'asservissement
//uint32_t asservAv = 0;	//compteur de temps pour l'asservissement
//static int16_t kP = 1000;	//coefficient de conversion entre la période de consigne et la période de commande (dépend du nombre de pas/tr du moteur et du nombre de commandes par pas).

//pour la commande du moteur
uint32_t periode = 80000;	//periode de commande du moteur, en microsecondes
uint16_t incPeriode = 200;	//periode d'incrémentation de periode pour le démarrage, en microseconde
uint32_t incTemps = 0;	//stocke le temps écoulé depuis le début pour le démarrage
uint32_t micTemps = 0;	//stocke le temps écoulé depuis le début pour les commutions du moteur
uint8_t ind = 0;	//code la position dans le chronogramme de commande du moteur
uint8_t pas[12][4] = {
	{1,0,1,0},
	{0,0,0,0},
	{1,0,1,0},
	{0,1,1,0},
	{0,0,0,0},
	{0,1,1,0},
	{0,1,0,1},
	{0,0,0,0},
	{0,1,0,1},
	{1,0,0,1},
	{0,0,0,0},
	{1,0,0,1}
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

	//on initialise les ports de commande du moteur
	sbi(DDRB,pinMot11);
	sbi(DDRB,pinMot12);
	sbi(DDRB,pinMot21);
	sbi(DDRB,pinMot22);
	//sbi(DDRB,pinMot3);
	//on initialise les interruptions du top tour sur front descendant
	sbi(EICRA,ISC01);
	cbi(EICRA,ISC00);
	sbi(EIMSK,INT0);//pour activer les interruptions sur INT0
	//on initialise la pin du sélecteur de sens en input avec pull-up
	cbi(DDRC,pinSens);//input
	sbi(portSens,pinSens);//pull-up

	//on calcule la valeur de la consigne en microsecondes en fonction de la fréquence de rotation demandée
	//consigne=1000000/freqD;

	//et on y va ;)
	printlnString("Go");

	//démarrage : 
	printlnString("demarrage");
	//while (periode > 5556) {
	//while (periode > 5000) {
	while(42) {
		//calcul de la valeur de periode à utiliser ici (en gros pour le démarrage, on se contente pour l'instant d'une simple rampe) : 
		if (micros() > incTemps + incPeriode) {
			incTemps = micros();
			if (periode > 12000) {
				periode-=2;
			}
			else if (periode > 6000) {
				periode--;
			}
		}
		//commutation des bobines du moteur : 
		if (micros() > micTemps + periode) {
			//printUShort(rbi(PINC,pinSens));
			//printString("\t");
			printUInt(periode);
			printString("\t");
			printUInt(ind);
			printString("\t");
			//printUInt(periode);
			//printString("\t");
			printlnULong(temps[1]);
			//printString("\t");
			//printlnInt(1000000/temps[1]);
			micTemps = micros();
			// et on commute
			commuter(pas[ind]);
			if (ind<11) 		ind++;
			else 			ind = 0;
		}
	}
	periode=5000;
	while (1) {
		//commutation des bobines du moteur : 
		if (micros() > micTemps + periode) {
			//printUShort(rbi(PINC,pinSens));
			//printString("\t");
			printUInt(periode);
			printString("\t");
			printUInt(ind);
			printString("\t");
			//printUInt(periode);
			//printString("\t");
			printlnULong(temps[1]);
			//printString("\t");
			//printlnInt(1000000/temps[1]);
			micTemps = micros();
			// et on commute
			commuter(pas[ind]);
			if (ind<11) 		ind++;
			else 			ind = 0;
		}
	}
	return 0;
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
	if (indT<1) 
		indT++;
	else 
		indT=0;
	sei();
}

/**
 * fonction de commutation
 */
void commuter(uint8_t pas_commuter[4]) {
	if (pas_commuter[0] == 1) {
		sbi(portMot11,pinMot11);
	}
	else {
		cbi(portMot11,pinMot11);
	}
	if (pas_commuter[1] == 1) {
		sbi(portMot12,pinMot12);
	}
	else {
		cbi(portMot12,pinMot12);
	}
	if (pas_commuter[2] == 1) {
		sbi(portMot21,pinMot21);
	}
	else {
		cbi(portMot21,pinMot21);
	}
	if (pas_commuter[3] == 1) {
		sbi(portMot22,pinMot22);
	}
	else {
		cbi(portMot22,pinMot22);
	}
}
