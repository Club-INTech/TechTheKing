CC=avr-g++
CFLAGS=-Wall -O2
MMCU=atmega328p
FREQ=16000000

all: recepteur.hex

recepteur.hex: recepteur.out
	avr-objcopy -O ihex -R .eeprom recepteur.out recepteur.hex

recepteur.out: main.o usart.o
	$(CC) $(CFLAGS) -mmcu=$(MMCU) -DF_CPU=$(FREQ) main.o usart.o -o recepteur.out

main.o: main.cpp
	$(CC) $(CFLAGS) -mmcu=$(MMCU) -DF_CPU=$(FREQ) main.cpp -c

usart.o: ../../../../../common-AVR/usart.c
	$(CC) $(CFLAGS) -mmcu=$(MMCU) -DF_CPU=$(FREQ) ../../../../../common-AVR/usart.c -c

clean:
	rm -rf *.o *.hex *.out

upload: all
	sudo avrdude -P /dev/ttyUSB0 -c stk500v1 -p m328p -b 57600 -D -U flash:w:recepteur.hex

