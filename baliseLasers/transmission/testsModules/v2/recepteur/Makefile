CC=avr-g++
CFLAGS=-Wall -O2
MMCU=atmega328p
FREQ=16000000

all: recepteur.hex

recepteur.hex: recepteur.out
	avr-objcopy -O ihex -R .eeprom recepteur.out recepteur.hex

recepteur.out: main.o usart.o temps.o
	$(CC) $(CFLAGS) -mmcu=$(MMCU) -DF_CPU=$(FREQ) main.o usart.o temps.o -o recepteur.out

main.o: main.cpp main.h
	$(CC) $(CFLAGS) -mmcu=$(MMCU) -DF_CPU=$(FREQ) main.cpp -c

temps.o: ./lib/temps.cpp ./lib/temps.h
	$(CC) $(CFLAGS) -mmcu=$(MMCU) -DF_CPU=$(FREQ) ./lib/temps.cpp -c

usart.o: ../../../../../common-AVR/usart.c ../../../../../common-AVR/usart.h
	$(CC) $(CFLAGS) -mmcu=$(MMCU) -DF_CPU=$(FREQ) ../../../../../common-AVR/usart.c -c

clean:
	rm -rf *.o *.hex *.out

upload: all
	sudo avrdude -P /dev/ttyUSB0 -c stk500v1 -p m328p -b 57600 -D -U flash:w:recepteur.hex


upload2: all
	sudo avrdude -c usbasp -p m328p -U flash:w:recepteur.hex

