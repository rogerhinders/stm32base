#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include "stm32f103.h"
#include "usb.h"
#include "amifont.h"
#include "sine.h"
#include "i2c.h"
#include "ssd1306.h"

#define LCD_W 128
#define LCD_H 32

#define LCD_MAX_CHAR ((LCD_W/8) * (LCD_H/8))

uint32_t row0 = 0;
uint32_t row1 = 0;
uint32_t row2 = 0;
uint32_t row3 = 0;
uint8_t lcd_buf[512];

void put_char(uint8_t *buf, uint8_t c, uint32_t col, uint32_t row) {
	int c2 = (int)(c-0x20);
	
	for(int i = 0; i < 8; i++) {
		buf[col*8+i + row * LCD_W] = font[c2*8+i];
	}
}

void put_pix(uint8_t *buf, uint32_t x, uint32_t y) {
	buf[x + (y/8) * LCD_W] |= (1 << (y & 7));
}

void h32(uint32_t d, uint32_t row) {
	switch(row) {
	case 0: row0 = d; break;
	case 1: row1 = d; break;
	case 2: row2 = d; break;
	case 3: row3 = d; break;
	}
}

void put_h32(uint8_t *buf, uint32_t d, uint32_t col, uint32_t row) {
	char hex[8];

	char *lut = "0123456789ABCDEF";

	put_char(buf, '0', col, row);
	put_char(buf, 'x', col+1, row);
	for(int i = 0; i < 8; i++) {
		put_char(buf, lut[(d >> (7-i)*4) & 0xf], col+i+2, row);	
	}

	ssd1306_write_array(lcd_buf, 512);
}

void lcd_clear_buf() {
	for(int i = 0; i < 512; i++) {
		lcd_buf[i] = 0x0;
	}
}

void lcd_draw_buf() {
	ssd1306_write_array(lcd_buf, 512);
}

int main() {
	row0 = 0;
	row1 = 0;
	row2 = 0;
	row3 = 0;
	init_flash();
	init_clock();
	init_gpio();
	init_i2c();


	for(int i = 0; i < 1000000; i++) {
		//waste time, wait for display to be ready ..
	}

	ssd1306_init();
	lcd_clear_buf();
	lcd_draw_buf();

	init_usb();

	while(1) {

		for(int i =0; i < 1000000; i++) {
		}

		put_h32(lcd_buf, row0, 0, 0);
		put_h32(lcd_buf, row1, 0, 1);
		put_h32(lcd_buf, row2, 0, 2);
		put_h32(lcd_buf, row3, 0, 3);
	}

	return 0;
}
