#include <stdint.h>
#include "common/stm32f103.h"
#include "common/usb.h"
#include "common/i2c.h"
#include "common/lcd.h"

int main() {
	stm32_flash_init();
	stm32_clock_init();
	stm32_gpio_init();
	i2c_init();
	lcd_init();
	usb_init();

	lcd_put_char('R',0,1);
	lcd_print_x32(0xAABBCCDD, 1,1);

	while(1) {

		for(int i =0; i < 1000000; i++) {
		}
		
		lcd_render();
	}

	return 0;
}
