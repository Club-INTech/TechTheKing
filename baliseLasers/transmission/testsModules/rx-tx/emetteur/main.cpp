#include "main.h"

void USART_Init(void)
{
	//Set baud rate
	UBRR0L=(uint8_t)UBRRVAL;		//low byte
	UBRR0H=(UBRRVAL>>8);	//high byte
	//Set data frame format: asynchronous mode,no parity, 1 stop bit, 8 bit size
	UCSR0C=(0<<UMSEL01)|(0<<UMSEL00)|(0<<UPM01)|(0<<UPM00)|
		(0<<USBS0)|(1<<UCSZ01)|(1<<UCSZ00);	
	//Enable Transmitter and Receiver and Interrupt on receive complete
	UCSR0B=(1<<TXEN0);
}

void USART_vSendByte(uint8_t u8Data)
{
	// Wait if a byte is being transmitted
	while((UCSR0A&(1<<UDRE0)) == 0);
	// Transmit data
	UDR0 = u8Data;  
}

void Send_Packet(uint8_t addr, uint8_t cmd)
{
	USART_vSendByte(SYNC_BYTE);//send synchro byte	
	//USART_vSendByte(addr);//send receiver address
	USART_vSendByte(cmd);//send increment command
	//USART_vSendByte((addr+cmd));//send checksum
}

void delayms(uint8_t t)//delay in ms
{
	uint8_t i;
	for(i=0;i<t;i++)
		_delay_ms(1);
}

int main(void)
{
	PORTB|=(1<<PORTB5);//LED OFF
	DDRB=(1<<PORTB5);//define port B pin 5 as output : arduino pin13;

	USART_Init();

	while(1)
	{//endless transmission
		//send command to switch led ON
		Send_Packet(RADDR_BYTE, LEDON_BYTE);
		//PORTB&=~(1<<PORTB5);//LED ON	
		sbi(PORTB,PORTB5);
		//delayms(100);
		_delay_ms(100);
		//send command to switch led ON
		Send_Packet(RADDR_BYTE, LEDOFF_BYTE);
		//PORTB|=(1<<PORTB5);//LED OFF
		cbi(PORTB,PORTB5);
		//delayms(100);
		_delay_ms(100);
	}
	return 0;
}
