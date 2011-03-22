CC=avr-gcc
CFLAGS=-Wall -O2 
MMCU=atmega328p
FREQ=20000000

all: main.hex

main.hex: main.out
	avr-objcopy -O ihex -R .eeprom main.out main.hex

main.out: main.o compteur.o i2c.o
	$(CC) $(CFLAGS) -mmcu=$(MMCU) -DF_CPU=$(FREQ) main.o compteur.o i2c.o -o main.out

main.o: main.c
	$(CC) $(CFLAGS) -mmcu=$(MMCU) -DF_CPU=$(FREQ) main.c -c

compteur.o: compteur.c
	$(CC) $(CFLAGS) -mmcu=$(MMCU) -DF_CPU=$(FREQ) compteur.c -c

i2c.o: i2c.c
	$(CC) $(CFLAGS) -mmcu=$(MMCU) -DF_CPU=$(FREQ) i2c.c -c

clean:
	rm -rf *.o *.hex *.out

upload: all
	sudo avrdude -P /dev/ttyUSB0 -c stk500v1 -p m328p -b 57600 -D -U flash:w:main.hex

upload2: all
	sudo avrdude -c usbasp -p m328p -U flash:w:main.hex

upload3: all
	sudo avrdude -c usbtiny -p m328p -U flash:w:main.hex
 
