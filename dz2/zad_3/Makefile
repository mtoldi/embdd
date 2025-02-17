SHELL=cmd
DEVICE     = atmega328p
CLOCK      = 16000000
PROGRAMMER = arduino
PORT	   = COM3
BAUD       = 115200
#BAUD      = 57600
FILENAME   = main
COMPILE    = avr-gcc -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)
CONFIGFILE = "avrdude.conf"


all: clean build upload
	
build:
	$(COMPILE) -c $(FILENAME).c -o $(FILENAME).o
	$(COMPILE) -o $(FILENAME).elf $(FILENAME).o
	avr-objcopy -j .text -j .data -O ihex $(FILENAME).elf $(FILENAME).hex
	avr-size --format=avr --mcu=$(DEVICE) $(FILENAME).elf

upload:
	avrdude -v -p $(DEVICE) -c $(PROGRAMMER) -P $(PORT) -b $(BAUD) -C $(CONFIGFILE) -U flash:w:$(FILENAME).hex:i 

clean:
	del $(FILENAME).o
	del $(FILENAME).elf
	del $(FILENAME).hex
