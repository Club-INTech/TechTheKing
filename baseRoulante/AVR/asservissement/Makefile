CC=avr-g++
CFLAGS=-Wall -O2
MMCU=atmega328p
FREQ=16000000

all: asservissement.hex

asservissement.hex: asservissement.out
	avr-objcopy -O ihex -R .eeprom asservissement.out asservissement.hex

asservissement.out: main.o Asservissement.o EnvoiPosition.o LectureSerie.o Manager.o HardwareSerial.o Print.o
	$(CC) $(CFLAGS) -mmcu=$(MMCU) -DF_CPU=$(FREQ) main.o Asservissement.o EnvoiPosition.o LectureSerie.o Manager.o HardwareSerial.o Print.o -o asservissement.out

main.o: main.cpp
	$(CC) $(CFLAGS) -mmcu=$(MMCU) -DF_CPU=$(FREQ) main.cpp -c

LectureSerie.o: lib/LectureSerie.cpp
	$(CC) $(CFLAGS) -mmcu=$(MMCU) -DF_CPU=$(FREQ) lib/LectureSerie.cpp -c

EnvoiPosition.o: lib/EnvoiPosition.cpp
	$(CC) $(CFLAGS) -mmcu=$(MMCU) -DF_CPU=$(FREQ) lib/EnvoiPosition.cpp -c

Asservissement.o: lib/Asservissement.cpp
	$(CC) $(CFLAGS) -mmcu=$(MMCU) -DF_CPU=$(FREQ) lib/Asservissement.cpp -c

Manager.o: lib/Manager.cpp
	$(CC) $(CFLAGS) -mmcu=$(MMCU) -DF_CPU=$(FREQ) lib/Manager.cpp -c

HardwareSerial.o: ../common/HardwareSerial.cpp
	$(CC) $(CFLAGS) -mmcu=$(MMCU) -DF_CPU=$(FREQ) ../common/HardwareSerial.cpp -c

Print.o: ../common/Print.cpp
	$(CC) $(CFLAGS) -mmcu=$(MMCU) -DF_CPU=$(FREQ) ../common/Print.cpp -c

usart.o: ../common/usart.c
	$(CC) $(CFLAGS) -mmcu=$(MMCU) -DF_CPU=$(FREQ) ../common/usart.c -c

clean:
	rm -rf *.o *.hex *.out

upload: all
	sudo avrdude -c usbtiny -p m328p -U flash:w:asservissement.hex