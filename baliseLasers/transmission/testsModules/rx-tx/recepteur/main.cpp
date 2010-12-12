#include "main.h"

void USART_Init(void) {
	//Set baud rate
	UBRR0L=(uint8_t)UBRRVAL;		//low byte
	UBRR0H=(UBRRVAL>>8);	//high byte
	//Set data frame format: asynchronous mode,no parity, 1 stop bit, 8 bit size
	UCSR0C=(0<<UMSEL01)|(0<<UMSEL00)|(0<<UPM01)|(0<<UPM00)|
		(0<<USBS0)|(1<<UCSZ01)|(1<<UCSZ00);	
	//Enable Transmitter and Receiver and Interrupt on receive complete
	UCSR0B=(1<<RXEN0)|(1<<RXCIE0)|(1<<TXEN0);
	//enable global interrupts
}

uint8_t USART_vReceiveByte(void) {
	// Wait until a byte has been received
	while((UCSR0A&(1<<RXC0)) == 0);
	// Return received data
	return UDR0;
}


void USART_vSendByte(uint8_t u8Data)
{
	// Wait if a byte is being transmitted
	while((UCSR0A&(1<<UDRE0)) == 0);
	// Transmit data
	UDR0 = u8Data;  
}


ISR(USART_RX_vect) {
	//define variables
	uint8_t raddress, data, chk;//transmitter address
	uint8_t sync;
	sync = USART_vReceiveByte();
	//receive destination address
	//raddress=USART_vReceiveByte();
	//receive data
	data=USART_vReceiveByte();
	//receive checksum
	//chk=USART_vReceiveByte();
	//compare received checksum with calculated
	//if(chk==(raddress+data)) {//if match perform operations
	//if transmitter address match
	//if(raddress==RADDR_BYTE) {
	if(data==LEDON_BYTE) {
		PORTB|=(1<<PORTB5);//LED ON
	}
	else if(data==LEDOFF_BYTE) {
		PORTB&=~(1<<PORTB5);//LED OFF
	}
	else {
		if (!(data == 0 || data == 0xFF)) {
			//blink led as error
			//PORTB&=~(1<<PORTB4);//LED OFF
			cbi(PORTB,PORTB4);
			_delay_ms(800);
			//PORTB|=(1<<PORTB4);//LED ON
			sbi(PORTB,PORTB4);
			_delay_ms(10);
			USART_vSendByte(sync);
			USART_vSendByte(data);
		}
	}
	//}
	//}
}

int main(void) {
	PORTB&=~(1<<PORTB5);//LED OFF
	DDRB=(1<<PORTB5);//define port B pin 5 as output : arduino pin13;
	PORTB&=~(1<<PORTB4);//LED OFF
	DDRB=(1<<PORTB4);//define port B pin 4 as output : arduino pin12;
	//enable global interrupts
	sei();

	USART_Init();

	while(1){ 
		//PORTB|=(1<<PORTB4);//LED OFF
		cbi(PORTB,PORTB4);
		_delay_ms(1000);
		//PORTB&=~(1<<PORTB4);//LED ON
		//sbi(PORTB,PORTB4);	
		_delay_ms(1000);
		//nothing here interrupts are working
	}

	return 0;
}
