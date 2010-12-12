#include "main.h"

void trans_init() {
	sbi(OUT_DDR,OUT_BIT);
}

void trans_sendByte(uint8_t data) {
	uint8_t send_bit=0;
	for (send_bit=0;send_bit<8;send_bit++) {
		if ( ((data & (1 << send_bit)) >> send_bit) ) {
			sbi(OUT_PORT,OUT_BIT);
		}
		else {
			cbi(OUT_PORT,OUT_BIT);
		}
		_delay_us(PERIOD_RATE);
	}
}

void trans_sendPacket(uint8_t addr, uint8_t sig) {
	trans_sendByte(SYNC_BYTE);//send synchro byte	
	//trans_sendByte(addr);//send receiver address
	trans_sendByte(sig);//send increment command
}

int main() {
	trans_init();

	while(1)
	{//endless transmission
		/*
		//send signal 1
		Send_Packet(RADDR_BYTE, SIG1_BYTE);
		_delay_ms(100);
		//send signal 2
		Send_Packet(RADDR_BYTE, SIG2_BYTE);
		_delay_ms(100);*/
		trans_sendByte(0x55);//pour lui faire envoyer : 01010101...
		//trans_sendByte(0x33);//pour lui faire envoyer : 00110011...
		//trans_sendByte(0x11);//pour lui faire envoyer : 00010001...
		//trans_sendByte(0x77);//pour lui faire envoyer : 01110111...
	}
	return 0;
}
