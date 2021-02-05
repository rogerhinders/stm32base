#include "i2c.h"

void i2c_init() {
	RCC_APB1ENR |= RCC_APB1ENR_I2C1_ENABLE;

	RCC_APB1RSTR |= RCC_APB1RSTR_I2C1RST;
	RCC_APB1RSTR &= ~RCC_APB1RSTR_I2C1RST;

	I2C1_CR2 |= 36;
	I2C1_CCR &= ~(1<<15);

	//fpclk1 = 72/2 = 36mhz
	//Tpclk1 = 1/36
	uint32_t ccr = 0xb4;
	I2C1_CCR |= ccr;

	I2C1_TRISE |= 0x25; //0x0009;

	I2C1_CR1 |= I2C1_CR1_PE;
}

