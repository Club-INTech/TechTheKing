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

LectureSerie.o: lib_asservissement/LectureSerie.cpp
	$(CC) $(CFLAGS) -mmcu=$(MMCU) -DF_CPU=$(FREQ) lib_asservissement/LectureSerie.cpp -c

EnvoiPosition.o: lib_asservissement/EnvoiPosition.cpp
	$(CC) $(CFLAGS) -mmcu=$(MMCU) -DF_CPU=$(FREQ) lib_asservissement/EnvoiPosition.cpp -c

Asservissement.o: lib_asservissement/Asservissement.cpp
	$(CC) $(CFLAGS) -mmcu=$(MMCU) -DF_CPU=$(FREQ) lib_asservissement/Asservissement.cpp -c

Manager.o: lib_asservissement/Manager.cpp
	$(CC) $(CFLAGS) -mmcu=$(MMCU) -DF_CPU=$(FREQ) lib_asservissement/Manager.cpp -c

HardwareSerial.o: lib_avr/HardwareSerial.cpp
	$(CC) $(CFLAGS) -mmcu=$(MMCU) -DF_CPU=$(FREQ) lib_avr/HardwareSerial.cpp -c

Print.o: lib_avr/Print.cpp
	$(CC) $(CFLAGS) -mmcu=$(MMCU) -DF_CPU=$(FREQ) lib_avr/Print.cpp -c

clean:
	rm -rf *.o *.hex *.out







