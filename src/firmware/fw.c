#include <stdint.h>
#include "common/stm32f103.h"
#include "common/usb.h"
#include "common/i2c.h"
#include "common/lcd.h"
#include "common/flash.h"
#include "common/spi.h"
#include "common/max7219.h"

int main() {
//	flash_init();
//	stm32_clock_init();
	stm32_gpio_init();
	i2c_init();
	lcd_init();
	spi_init();

	max7219_init();
	max7219_test2();

	lcd_print_x32(SPI1_SR,0,1);
	lcd_print_x32(SPI1_CR1, 0,2);
	lcd_print_x32(0xffffffff, 0,3);

	while(1) {

		for(int i =0; i < 1000000; i++) {
		}

	//	GPIOA_ODR |= 2;
		
		lcd_render();
	}

	return 0;
}
