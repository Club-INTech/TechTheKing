#include "main.h"

void trans_init(void) {
	sbi(EICRA,ISC01);
	sbi(EICRA,ISC00);
	sbi(EIMSK,INT0);
}

uint8_t trans_receiveByte(void) {
	cli();//on coupe les interruptions pour être tranquille
	uint8_t data=0,receive_bit=0;
	for (receive_bit=0;receive_bit<8;receive_bit++) {
		if (rbi(PIND,PORTD2) == 1) {
			sbi(data,receive_bit);
		}
		else {
			cbi(data,receive_bit);
		}
		_delay_us(PERIOD_RATE);
	}
	sei();//on réactive les interruptions pour pouvoir travailler
	return data;
}

ISR(INT0_vect) {
	//define variables
	uint8_t data;// raddress, , chk;//transmitter address
	uint8_t sync;
	sync = trans_receiveByte();
	//receive destination address
	//raddress=trans_receiveByte();
	//receive data
	data=trans_receiveByte();
	//if transmitter address match
	//if(raddress==RADDR_BYTE) {
	/*if(data==SIG1_BYTE) {
	  PORTB|=(1<<PORTB5);//LED ON
	  }
	  else if(data==SIG2_BYTE) {
	  PORTB&=~(1<<PORTB5);//LED OFF
	  }
	  else {*/
	/*if (!(data == 0 || data == 0xFF)) {
	//blink led as error
	//PORTB&=~(1<<PORTB4);//LED OFF
	cbi(PORTB,PORTB4);
	_delay_ms(800);
	//PORTB|=(1<<PORTB4);//LED ON
	sbi(PORTB,PORTB4);
	_delay_ms(10);
	USART_vSendByte(sync);
	USART_vSendByte(data);
	}*///c'est quoi tout ce bordel?
	//}
	//}
	if (data == 0x55){
		sbi(PORTB,PORTB5);
		_delay_ms(50);
		cbi(PORTB,PORTB5);
		_delay_ms(50);
	}
}

int main(void) {
	sbi(DDRB,PORTB5);
	//enable global interrupts
	sei();

	trans_init();

	while(1) {
		//nothing here interrupts are working
	}

	return 0;
}
