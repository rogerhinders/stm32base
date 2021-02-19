#include "common/spi.h"

void spi_send8(uint8_t b) {
	SPI1_DR = b;
	while(!(SPI1_SR & SPI_SR_TXE));
}

void spi_send16(uint16_t b) {
	SPI1_DR = b;
	while(!(SPI1_SR & SPI_SR_TXE));
}

void spi_init() {
//	GPIOA_ODR |= 0x10;

	RCC_APB2ENR |= RCC_APB2ENR_SPI1EN;
	SPI1_CR2 = SPI_CR2_SSOE;
	SPI1_CR1 =  SPI_CR1_MSTR | (2 << SPI_P_CR1_BR) | SPI_CR1_SPE | SPI_CR1_SSM 
		| SPI_CR1_BIDIOE | SPI_CR1_BIDIMODE;// | SPI_CR1_DFF;
//			| SPI_CR1_SSI;

	while(!(SPI1_SR & SPI_SR_TXE));
}
