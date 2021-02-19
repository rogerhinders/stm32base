#include "common/max7219.h"

void set_load_low() {
	GPIOA_ODR &= ~0x10;
//	SPI1_CR1 &= ~SPI_CR1_SSI;
}

void set_load_high() {
	GPIOA_ODR |= 0x10;
	//SPI1_CR1 |= SPI_CR1_SSI;
}

void set_reg(uint8_t reg, uint8_t b) {
	set_load_low();

	spi_send8(reg);
	spi_send8(b);

//	spi_send16((reg << 8) | b);


	set_load_high();
}

void max7219_turn_off() {
	set_reg(MAX7219_REG_SHUTDOWN, 0);
}

void max7219_turn_on() {
	set_reg(MAX7219_REG_SHUTDOWN, 1);
}

void max7219_set_intensity(uint8_t intensity) {
	set_reg(MAX7219_REG_INTENSITY, intensity & 0xf);
}

void max7219_test() {
	set_reg(MAX7219_REG_DISPLAY_TEST, 0xff);
}

void max7219_test2() {
	return;
	set_reg(MAX7219_REG_DIGIT0, 0x1);
}

void max7219_init() {
	set_reg(MAX7219_REG_SCAN_LIMIT, 5);
	set_reg(MAX7219_REG_DECODE_MODE, 0);
	max7219_set_intensity(0x1);
	max7219_turn_on();
	return;
	max7219_turn_on();
	max7219_set_intensity(0x2);
}
