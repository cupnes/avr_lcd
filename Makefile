CFLAGS	=	-mmcu=atmega168
CFLAGS	+=	-Wall -Wextra

.c.o:
	avr-gcc $(CFLAGS) -o $@ $<

lcd.hex: lcd.o
	avr-objcopy -O ihex $< $@

program: lcd.hex
	sudo avrdude -p m168 -c avrisp2 -U $<

clean:
	rm -f *~ *.o *.hex

.PHONY: program clean
