#include "main.h"

/*
 * définissons quelques variables globales ou define (selon les 
 * besoins) qu'on va utiliser pour le moteur.
 */
//pour la consigne en vitesse de rotation du moteur : 
#define freqD 10 //fréquence demandée en tr/s
static uint32_t consigne;//période pour la consigne de commande
//pour la mesure des periodes de rotation du moteur
volatile uint32_t temps[2]={65535,65535};//contient les périodes de rotation mesurées
volatile uint8_t indT=0;//code l'élément de temps[] à remplir
volatile uint32_t milAP=0;//micros après top
volatile uint32_t milAV=0;//micros avant top
//pour l'asservissement en vitesse du moteur
//static uint16_t timerAsserv=10000;//période d'asservissement
//uint32_t asservAv = 0;//compteur de temps pour l'asservissement
//static int16_t kP = 1000;//coefficient de conversion entre la période de consigne et la période de commande (dépend du nombre de pas/tr du moteur et du nombre de commandes par pas).
//pour la commande du moteur
uint16_t periode = 40000;//periode de commande du moteur, en microsecondes
uint16_t incPeriode = 5000; //periode d'incrémentation de periode pour le démarrage, en microseconde
uint32_t incTemps = 0;//stoque le temps écoulé depuis le début pour le démarrage
uint32_t micTemps = 0;//stoque le temps écoulé depuis le début pour les commutions du moteur
uint8_t ind = 0;//code la position dans le chronogramme de commande du moteur

/**
 * et on y va pour la fonction main
 */
int main() {
	//on initialise la notion temporelle sur l'AVR
	temps_init();
	//on initialise la transmission série (pour le debug)
	uart_init();
	//on initialise l'alimentation des lasers (par pwm)
	lasers_init();
	//on initialise les ports de commande du moteur
	sbi(DDRB,pinMot1);
	sbi(DDRB,pinMot2);
	sbi(DDRB,pinMot3);
	//on initialise les interruptions du top tour sur front descendant
	sbi(EICRA,ISC01);
	cbi(EICRA,ISC00);
	sbi(EIMSK,INT0);//pour activer les interruptions sur INT0
	//on initialise la pin du sélecteur de sens en input avec pull-up
	cbi(DDRC,pinSens);//input
	sbi(portSens,pinSens);//pull-up

	//on calcule la valeur de la consigne en microsecondes en fonction de la fréquence de rotation demandée
	consigne=1000000/freqD;

	//et on y va ;)
	printlnString("Go");

	//démarrage : 
	printlnString("demarrage");
	while (periode > 5556) {
		//calcul de la valeur de periode à utiliser ici (en gros pour le démarrage, on se contente pour l'instant d'une simple rampe) : 
		if (micros() > incTemps + incPeriode) {
			incTemps = micros();
			if (periode > 10000) {
				periode--;
			}
			else if (periode > 2778) {
				periode--;
			}
		}
		//commutation des bobines du moteur : 
		if (micros() > micTemps + periode) {
			printUShort(rbi(PINC,pinSens));
			printString("\t");
			printUInt(periode);
			printString("d");
			printULong(temps[1]);
			printString("\t");
			printlnInt(1000000/temps[1]);
			micTemps = micros();
			if (ind==0)		cbi(portMot3,pinMot3);
			else if (ind==1)	sbi(portMot2,pinMot2);
			else if (ind==2)	cbi(portMot1,pinMot1);
			else if (ind==3)	sbi(portMot3,pinMot3);
			else if (ind==4)	cbi(portMot2,pinMot2);
			else			sbi(portMot1,pinMot1);
			if (ind<5) 		ind++;
			else 			ind = 0;
		}
	}
	periode=5556;
	while (1) {
		//commutation des bobines du moteur : 
		if (micros() > micTemps + periode) {
			printUShort(rbi(PINC,pinSens));
			printString("\t");
			printUInt(periode);
			printString("d");
			printULong(temps[1]);
			printString("\t");
			printlnInt(1000000/temps[1]);
			micTemps = micros();
			if (ind==0)		cbi(portMot3,pinMot3);
			else if (ind==1)	sbi(portMot2,pinMot2);
			else if (ind==2)	cbi(portMot1,pinMot1);
			else if (ind==3)	sbi(portMot3,pinMot3);
			else if (ind==4)	cbi(portMot2,pinMot2);
			else			sbi(portMot1,pinMot1);
			if (ind<5) 		ind++;
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
