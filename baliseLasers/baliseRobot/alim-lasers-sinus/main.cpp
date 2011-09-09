#include "main.h"

/**
 * et on y va pour la fonction main
 */
int main() {
	//on initialise la transmission série (pour le debug)
	uart_init();
	printlnString("Start");

	//on initialise la notion temporelle sur l'AVR
	//temps_init();

	//on initialise l'alimentation des lasers (par pwm)
	lasers_init();//!!!ce n'est que l'initialisation des timers, ça n'allume pas les lasers pour autant!!!
	//lasers_init2();//!!!ce n'est que l'initialisation des timers, ça n'allume pas les lasers pour autant!!!

	//et on y va ;)
	printlnString("Go");

	lasers_start();

	//démarrage : 
	int l=0;
	while(42) {
		l++;
		//printString("a");
		printUInt(TCNT0);
	}
	return 0;
}

