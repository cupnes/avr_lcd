/*
ビルド手順
avr-gcc -mmcu=atmega168 -Wall -Os -DF_CPU=10000000 -I./ -o lcd.out lcd.c
avr-objcopy -O ihex lcd.out lcd.c.hex
sudo avrdude -p m168 -c avrisp2 -U lcd.c.hex
 */

#include <avr/io.h>

#define	LCD_PORT	PORTD

#define	LCD_E(b) {				\
		if(b) LCD_PORT |= _BV(3);	\
		else LCD_PORT &= ~_BV(3);	\
	}

#define	LCD_RS(b) {				\
		if(b) LCD_PORT |= _BV(2);	\
		else LCD_PORT &= ~_BV(2);	\
	}

static void waitus(unsigned int _n)
{
	/* volatile unsigned int n = _n * 10; */
	volatile unsigned int n = _n * 20;
	while (n-- > 0);
}

static void waitms(unsigned int _n)
{
	/* volatile unsigned int n = _n * 1500; */
	volatile unsigned int n = _n * 20000;
	while (n-- > 0);
}

static void lcd_write8(unsigned char ch)
{
	LCD_PORT = (LCD_PORT & 0x0f) | (ch & 0xf0);

	LCD_E(1);
	waitus(1);
	LCD_E(0);
	waitus(100);
}

static void lcd_write4(unsigned char ch)
{
	lcd_write8(ch);
	lcd_write8(ch<<4);
}

static void lcd_init(void)
{
	LCD_RS(0);
	LCD_E(0);
	waitms(100);

	lcd_write8(0b00110000);
	waitms(5);

	lcd_write8(0b00110000);
	waitus(100);

	lcd_write8(0b00110000);

	lcd_write8(0b00100000);

	lcd_write4(0b00101000);

	lcd_write4(0b00000110);
	lcd_write4(0b00001100);

	lcd_write4(0b00000001);
	waitms(2);
}

static void lcd_puts(char *s)
{
	LCD_RS(1);
	while(*s){
		lcd_write4(*s++);
	}
}

int main(void)
{
	PORTD = 0;
	DDRD = 0xfc;

	lcd_init();
	lcd_puts("Hello");

	while (1);

	return 0;
}
