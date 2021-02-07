#ifndef LCD_HDR
#define LCD_HDR

#include <stdio.h>
#include <stdlib.h>
#include "ssd1306.h"

#define LCD_W 128
#define LCD_H 32

#define LCD_MAX_CHAR ((LCD_W/8) * (LCD_H/8))

void lcd_put_pix(uint32_t x, uint32_t y);
void lcd_put_char(uint8_t c, uint32_t col, uint32_t row);
void lcd_print_x32(uint32_t d, uint32_t col, uint32_t row);
void lcd_clear();
void lcd_render();
void lcd_init();

#endif /* _LCD_HDR */
