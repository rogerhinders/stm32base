#ifndef _SPI_H
#define _SPI_H

#include "common/stm32f103.h"

void spi_send8(uint8_t b);
void spi_send16(uint16_t b);
void spi_init();

#endif /* _SPI_H */
