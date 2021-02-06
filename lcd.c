#include "lcd.h"
#include "amifont.h"

static uint8_t lcd_buf[512];

void lcd_put_pix(uint32_t x, uint32_t y) {
	lcd_buf[x + (y/8) * LCD_W] |= (1 << (y & 7));
}

void lcd_put_char(uint8_t c, uint32_t col, uint32_t row) {
	int c2 = (int)(c-0x20);

	for(int i = 0; i < 8; i++) {
		lcd_buf[col*8+i + row * LCD_W] = amifont[c2*8+i];
	}
}

void lcd_print_x32(uint32_t d, uint32_t col, uint32_t row) {
	char hex[8];

	char *lut = "0123456789ABCDEF";

	lcd_put_char('0', col, row);
	lcd_put_char('x', col+1, row);

	for(int i = 0; i < 8; i++) {
		lcd_put_char(lut[(d >> (7-i)*4) & 0xf], col+i+2, row);
	}
}

void lcd_clear() {
	for(int i = 0; i < 512; i++) {
		lcd_buf[i] = 0x0;
	}
}

void lcd_render() {
	ssd1306_write_array(lcd_buf, 512);
}

void lcd_init() {
	ssd1306_init();
	lcd_clear();
	lcd_render();
}
