OBJS=main.o Util.o EnvoiPosition.o Asservissement.o LectureSerie.o Manager.o i2c.o serial.o
CC=avr-g++
CFLAGS=-Wall -O2 
MMCU=atmega328p
FREQ=20000000

all: main.hex

main.hex: main.out
	avr-objcopy -O ihex -R .eeprom main.out main.hex

main.out: $(OBJS)
	$(CC) $(CFLAGS) -mmcu=$(MMCU) -DF_CPU=$(FREQ) main.o Util.o EnvoiPosition.o Asservissement.o LectureSerie.o Manager.o i2c.o serial.o -o main.out

main.o: main.cpp
	$(CC) $(CFLAGS) -mmcu=$(MMCU) -DF_CPU=$(FREQ) main.cpp -c

Asservissement.o: ./lib/Asservissement.cpp
	$(CC) $(CFLAGS) -mmcu=$(MMCU) -DF_CPU=$(FREQ) ./lib/Asservissement.cpp -c

Manager.o: ./lib/Manager.cpp
	$(CC) $(CFLAGS) -mmcu=$(MMCU) -DF_CPU=$(FREQ) ./lib/Manager.cpp -c

LectureSerie.o: ./lib/LectureSerie.cpp
	$(CC) $(CFLAGS) -mmcu=$(MMCU) -DF_CPU=$(FREQ) ./lib/LectureSerie.cpp -c
	
EnvoiPosition.o: ./lib/EnvoiPosition.cpp
	$(CC) $(CFLAGS) -mmcu=$(MMCU) -DF_CPU=$(FREQ) ./lib/EnvoiPosition.cpp -c

Util.o: ./lib/Util.c
	$(CC) $(CFLAGS) -mmcu=$(MMCU) -DF_CPU=$(FREQ) ./lib/Util.c -c
		
i2c.o: ./lib/i2c.c
	$(CC) $(CFLAGS) -mmcu=$(MMCU) -DF_CPU=$(FREQ) ./lib/i2c.c -c

serial.o: ./lib/serial.c
	$(CC) $(CFLAGS) -mmcu=$(MMCU) -DF_CPU=$(FREQ) ./lib/serial.c -c

clean:
	rm -rf *.o *.hex *.out

upload: all
	sudo avrdude -P /dev/ttyUSB0 -c stk500v1 -p m328p -b 57600 -D -U flash:w:main.hex

upload2: all
	sudo avrdude -c usbasp -p m328p -U flash:w:main.hex

upload3: all
	sudo avrdude -c usbtiny -p m328p -U flash:w:main.hex
 
