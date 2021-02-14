#include "common/spi.h"

void spi_init() {
	GPIOA_ODR |= 0x10;

	RCC_APB2ENR |= RCC_APB2ENR_SPI1EN;
	SPI1_CR2 = SPI_CR2_SSOE;
	SPI1_CR1 =  SPI_CR1_MSTR | (1 << SPI_P_CR1_BR) | SPI_CR1_SPE;// | SPI_CR1_DFF;

	while(!(SPI1_SR & SPI_SR_TXE));
}
