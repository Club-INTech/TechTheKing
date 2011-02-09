#include "temps.h"

volatile uint32_t cnt_ovf_timer1=0;	//compteur d'interruptions timer1

void temps_init() {
	cbi(TCCR1B,CS12);	//|choix d'une clock 
	cbi(TCCR1B,CS11);	//|sans prescaler pour 
	sbi(TCCR1B,CS10);	//|plus de précision

	sbi(TIMSK1,TOIE1);	//pour activer les interruptions sur overflow
}

uint32_t micros() {
	uint32_t microseconds = 0;
	microseconds = (cnt_ovf_timer1*65536+TCNT1H*256+TCNT1L)/(F_CPU/1000000);
	return microseconds;
}

ISR(TIMER1_OVF_vect) {
	cnt_ovf_timer1++;
}

