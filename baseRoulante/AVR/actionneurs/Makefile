CC=avr-gcc
CFLAGS=-Wall -O2 
MMCU=atmega328p
FREQ=16000000

all: main.hex

main.hex: main.out
	avr-objcopy -O ihex -R .eeprom main.out main.hex

main.out: main.o actionneurs.o i2c.o ax12.o
	$(CC) $(CFLAGS) -mmcu=$(MMCU) -DF_CPU=$(FREQ) main.o actionneurs.o i2c.o ax12.o -o main.out

main.o: main.c
	$(CC) $(CFLAGS) -mmcu=$(MMCU) -DF_CPU=$(FREQ) main.c -c

actionneurs.o: actionneurs.c
	$(CC) $(CFLAGS) -mmcu=$(MMCU) -DF_CPU=$(FREQ) actionneurs.c -c

i2c.o: i2c.c
	$(CC) $(CFLAGS) -mmcu=$(MMCU) -DF_CPU=$(FREQ) i2c.c -c

ax12.o: ax12.c
	$(CC) $(CFLAGS) -mmcu=$(MMCU) -DF_CPU=$(FREQ) ax12.c -c

clean:
	rm -rf *.o *.hex *.out

upload: all
	sudo avrdude -P /dev/ttyUSB0 -c stk500v1 -p m328p -b 57600 -D -U flash:w:main.hex

upload2: all
	sudo avrdude -c usbasp -p m328p -U flash:w:main.hex

upload3: all
	sudo avrdude -c usbtiny -p m328p -U flash:w:main.hex
 
