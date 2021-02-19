#ifndef _MAX7219_H
#define _MAX7219_H

#include "common/stm32f103.h"
#include "common/spi.h"

#define MAX7219_REG_NOOP         0x00
#define MAX7219_REG_DIGIT0       0x01
#define MAX7219_REG_DIGIT1       0x02
#define MAX7219_REG_DIGIT2       0x03
#define MAX7219_REG_DIGIT3       0x04
#define MAX7219_REG_DIGIT4       0x05
#define MAX7219_REG_DIGIT5       0x06
#define MAX7219_REG_DIGIT6       0x07
#define MAX7219_REG_DIGIT7       0x08
#define MAX7219_REG_DECODE_MODE  0x09
#define MAX7219_REG_INTENSITY    0x0a
#define MAX7219_REG_SCAN_LIMIT   0x0b
#define MAX7219_REG_SHUTDOWN     0x0c
#define MAX7219_REG_DISPLAY_TEST 0x0f

void max7219_turn_off();
void max7219_turn_on();
void max7219_set_intensity(uint8_t intensity);
void max7219_test();
void max7219_test2();
void max7219_init();

#endif /* _MAX7219_H */
