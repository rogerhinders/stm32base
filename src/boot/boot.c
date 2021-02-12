#include <stdint.h>
#include "common/stm32f103.h"
#include "common/usb.h"
#include "common/i2c.h"
#include "common/lcd.h"
#include "common/flash.h"
#include "common/common.h"

enum FlasherState {
	FLASHER_READY, FLASHER_IN_PROGRESS, FLASHER_ERR, FLASHER_DONE
};

static uint32_t counter;
static uint32_t n_flash_written;
static enum FlasherState f_state;
static uint16_t page_buf[FLASH_PAGE_SIZE/2];
static uint32_t page_buf_i;
uint32_t current_page;

bool req_out_handler(uint32_t ep_id) {
	if(ep_id == 1) {
		if(f_state == FLASHER_ERR) {
			return false;
		}

		if(f_state == FLASHER_DONE) {
			return false;
		}
	
		size_t n_bytes = USB_BDT_COUNT_RX(ep_id) & USB_COUNTN_RX_COUNTN_RX;

		if(f_state == FLASHER_READY) {
			f_state = FLASHER_IN_PROGRESS;
			page_buf_i = 0;
			current_page = FW_FLASH_BASE_ADDR / FLASH_PAGE_SIZE;
		}

		size_t n_count = n_bytes;

		/* make sure we dont miss last byte if uneven.. */
		if(n_count % 2 != 0) {
			n_count++;
		}

		uint32_t pma_base = USB_BDT_ADDR_RX(ep_id);

		for(int i = 0; i < n_count/2; i++) {
			page_buf[page_buf_i++] = USB_PMA(pma_base + i * 2);
		}

		if(n_bytes < 64) { /* if last data */
			if(!flash_write_page(page_buf, current_page)) {
				f_state = FLASHER_ERR;
				return false;
			}

			f_state = FLASHER_DONE;
			lcd_put_str("flash done!",0,1);
		} else {
			if(page_buf_i == (FLASH_PAGE_SIZE/2)) {
				if(!flash_write_page(page_buf, current_page)) {
					f_state = FLASHER_ERR;
					return false;
				}

				current_page++;
				page_buf_i = 0;
			}
		}

		n_flash_written += n_bytes;
		counter++;
	}

	return true;
}

void boot_fw() {
	asm volatile(
			"ldr r0,=0x08002001\n\t"
			"bx r0\n\t"
		);
}

int main() {
	counter = 0;
	n_flash_written = 0; 
	f_state = FLASHER_READY;

	flash_init();
	stm32_clock_init();
	stm32_gpio_init();

	/* if KEY not pressed, boot fw */
	if(!(GPIOA_IDR & 1)) {
		boot_fw();

		while(1) ;
	}

	i2c_init();
	lcd_init();
	usb_init();

	usb_set_custom_request_out_handler(req_out_handler);

	char *s_mode = "Ready to flash..";
	lcd_put_str(s_mode, 0, 0);

	while(1) {

		for(int i =0; i < 1000000; i++) {
		}
		
		lcd_render();
	}

	return 0;
}
