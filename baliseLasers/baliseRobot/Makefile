CC=avr-g++
CFLAGS=-Wall -O2
MMCU=atmega328p
FREQ=16000000

all: baliseRobot.hex

baliseRobot.hex: baliseRobot.out
	avr-objcopy -O ihex -R .eeprom baliseRobot.out baliseRobot.hex

baliseRobot.out: main.o usart.o temps.o lasers.o
	$(CC) $(CFLAGS) -mmcu=$(MMCU) -DF_CPU=$(FREQ) main.o usart.o temps.o lasers.o -o baliseRobot.out

main.o: main.cpp
	$(CC) $(CFLAGS) -mmcu=$(MMCU) -DF_CPU=$(FREQ) main.cpp -c

temps.o : lib/temps.cpp
	$(CC) $(CFLAGS) -mmcu=$(MMCU) -DF_CPU=$(FREQ) lib/temps.cpp -c

lasers.o : lib/lasers.cpp
	$(CC) $(CFLAGS) -mmcu=$(MMCU) -DF_CPU=$(FREQ) lib/lasers.cpp -c

usart.o: ../../baseRoulante/AVR/common/usart.c
	$(CC) $(CFLAGS) -mmcu=$(MMCU) -DF_CPU=$(FREQ) ../../baseRoulante/AVR/common/usart.c -c

clean:
	rm -rf *.o *.hex *.out

upload: all
	sudo avrdude -P /dev/ttyUSB0 -c stk500v1 -p m328p -b 57600 -D -U flash:w:baliseRobot.hex
