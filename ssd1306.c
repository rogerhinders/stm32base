#include "ssd1306.h"

void ssd1306_send_start() {

	I2C1_CR1 |= I2C1_CR1_START;
	while(!(I2C1_SR1 & I2C1_SR1_SB)) ;


	I2C1_DR = 0x3c << 1;


	while(!(I2C1_SR1 & I2C1_SR1_ADDR)) ;

	uint32_t read_clear = I2C1_SR1;
	read_clear = I2C1_SR2;

	/* avoid compiler optimization */
	(void)read_clear;
	
	while((I2C1_SR1 & I2C1_SR1_ADDR)) ;


	while(!(I2C1_SR1 & I2C1_SR1_TXE)) ;
}


void ssd1306_run_cmd(uint8_t cmd) {
	ssd1306_send_start();
	
	while(!(I2C1_SR1 & I2C1_SR1_TXE)) ;
	I2C1_DR = 0x0;
	while(!(I2C1_SR1 & I2C1_SR1_TXE)) ;
	I2C1_DR = cmd;

	while(!(I2C1_SR1 & I2C1_SR1_TXE)) ;
	while(!(I2C1_SR1 & I2C1_SR1_BTF)) ;

	I2C1_CR1 |= I2C1_CR1_STOP;
}

void ssd1306_write_array(uint8_t *buf, uint32_t len) {
	ssd1306_run_cmd(0x21);
	ssd1306_run_cmd(0x00);
	ssd1306_run_cmd(0x7f);

	ssd1306_run_cmd(0x22);
	ssd1306_run_cmd(0x00);
	ssd1306_run_cmd(0x07);

	ssd1306_send_start();

	while(!(I2C1_SR1 & I2C1_SR1_TXE)) ;
	I2C1_DR = 0x40;

	for(uint32_t i = 0; i < len; i++) {
		while(!(I2C1_SR1 & I2C1_SR1_TXE)) ;
		I2C1_DR = *buf;
		buf++;
	}

	while(!(I2C1_SR1 & I2C1_SR1_TXE)) ;
	while(!(I2C1_SR1 & I2C1_SR1_BTF)) ;

	I2C1_CR1 |= I2C1_CR1_STOP;
}

void ssd1306_init() {
	for(int i = 0; i < 1000000; i++) {
		/* waste time, wait for display to be ready .. */
	}

	/* enable display */
	ssd1306_run_cmd(0xae);
	/* set contrast */
	ssd1306_run_cmd(0x81);
	ssd1306_run_cmd(0x7f);
	/* deactivate scroll */
	ssd1306_run_cmd(0x2e);
	/* set horiz mode */
	ssd1306_run_cmd(0x20);
	ssd1306_run_cmd(0x00);
	/*  segment remap */
	ssd1306_run_cmd(0xa1);
	/* set multiplex ratio, vres - 1, 32-1 = 31 */
	ssd1306_run_cmd(0xa8);
	ssd1306_run_cmd(0x1f);
	/* set normal com scan dir */
	ssd1306_run_cmd(0xc8);
	/* set 0 display offset */
	ssd1306_run_cmd(0xd3);
	ssd1306_run_cmd(0x00);
	/* set startline 0 */
	ssd1306_run_cmd(0x40);
	/* set com pins */
	ssd1306_run_cmd(0xda);
	ssd1306_run_cmd(0x02);
	/* set clock div */
	ssd1306_run_cmd(0xd5);
	ssd1306_run_cmd(0x80);
	/* set precharge */
	ssd1306_run_cmd(0xd9);
	ssd1306_run_cmd(0xf1);
	/* set vcom */
	ssd1306_run_cmd(0xdb);
	ssd1306_run_cmd(0x40);
	/* enable chargepump */
	ssd1306_run_cmd(0x8d);
	ssd1306_run_cmd(0x14);
	/* set entire display on from ram */
	ssd1306_run_cmd(0xa4);
	/* set normal non-inverted disp */
	ssd1306_run_cmd(0xa6);
	/* set display on */
	ssd1306_run_cmd(0xaf);
}
