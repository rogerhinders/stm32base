#include "common/max7219.h"

void max7219_test() {
	GPIOA_ODR &= 0x10;

	SPI1_DR = 0xff;
	while(!(SPI1_SR & SPI_SR_TXE));
	
	SPI1_DR = 0xff;
	while(!(SPI1_SR & SPI_SR_TXE));

	GPIOA_ODR |= 0x10;
}

void max7219_init() {
}
