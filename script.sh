avr-gcc -g -Os -mmcu=atmega32 -c smoke.c
avr-gcc -g -mmcu=atmega32 -o smoke.elf smoke.o
avr-objcopy -j .text -j .data -O ihex smoke.elf smoke.hex
avr-size --format=avr --mcu=atmega32 smoke.elf
avrdude -c usbasp -p m32 -P /dev/ttyACM0 -B10 -U flash:w:smoke.hex -F
