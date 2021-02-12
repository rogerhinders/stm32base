#include "common/usb.h"

static uint32_t dev_usb_addr = 0;
static struct usb_request req;

void usb_parse_request(uint32_t ep_id, struct usb_request *dst) {
	uint32_t offset = USB_BDT_ADDR_RX(ep_id);
	uint8_t rtype = USB_PMA(offset) & 0xff;

	dst->direction = rtype & 0x80 ? USB_DIR_DEV_TO_HOST : USB_DIR_HOST_TO_DEV;
	dst->type = rtype >> 5 & 3;
	dst->recipient = rtype & 0x1f;

	dst->which = USB_PMA(offset) >> 8 & 0xff;

	offset += 2;

	dst->w_value = USB_PMA(offset) & 0xffff;
	dst->w_value_l = USB_PMA(offset) & 0xff;
	dst->w_value_h = (USB_PMA(offset) >> 8) & 0xff;

	offset += 2;

	dst->w_index = USB_PMA(offset) & 0xffff;

	offset += 2;

	dst->w_length = USB_PMA(offset) & 0xffff;
}

static void reset() {
	dev_usb_addr = 0;
	/* clear irq */
	USB_ISTR = 0;

	USB_CNTR = USB_CNTR_RESETM | USB_CNTR_CTRM | USB_CNTR_PMAOVRM
			| USB_CNTR_ERRM | USB_CNTR_WKUPM | USB_CNTR_SUSPM | USB_CNTR_SOFM
			| USB_CNTR_ESOFM;

	/* set btable offset */
	USB_BTABLE = USB_BTABLE_BASE_OFFSET;

	/* setup default endpoint 0 */
	/* set buffer desc table entry for ep0 */
	USB_BDT_ADDR_TX(0) = 0x40;
	USB_BDT_ADDR_RX(0) = 0x80;

	USB_BDT_COUNT_TX(0) = 0;
	USB_BDT_COUNT_RX(0) = USB_COUNTN_RX_BLSIZE | (2 << USB_P_COUNTN_RX_NUM_BLOCK);

	/* set ep1 buffers */
	USB_BDT_ADDR_TX(1) = 0xc0;
	USB_BDT_ADDR_RX(1) = 0x100;

	USB_BDT_COUNT_TX(1) = 0;
	USB_BDT_COUNT_RX(1) = USB_COUNTN_RX_BLSIZE | (2 << USB_P_COUNTN_RX_NUM_BLOCK);

	/* enable device */
	USB_DADDR = USB_DADDR_EF;

	/* enable ep0 */
	USB_EP(0) = (USB_EP_TYPE_CNTR << USB_P_EP_EP_TYPE) | (USB_EP_STAT_VALID << USB_P_EP_STAT_RX);

	/* enable ep1 */
	USB_EP(1) = (USB_EP_TYPE_BULK << USB_P_EP_EP_TYPE) 
		| (USB_EP_STAT_VALID << USB_P_EP_STAT_RX)
		| (USB_EP_DTOG_RX << USB_P_EP_DTOG_RX) | 1;
}

static void ep_reg_rx_valid(uint32_t ep_id) {
	uint32_t v = (USB_EP(ep_id) ^ (USB_EP_STAT_VALID << USB_P_EP_STAT_RX)) & (USB_EP_INV_MASK | USB_EP_STAT_RX);
	USB_EP(ep_id) = v;
}

static void ep_reg_tx_valid(uint32_t ep_id) {
	uint32_t v = (USB_EP(ep_id) ^ (USB_EP_STAT_VALID << USB_P_EP_STAT_TX)) & (USB_EP_INV_MASK | USB_EP_STAT_TX);
	USB_EP(ep_id) =v;
}

static void ep_reg_stall(uint32_t ep_id) {
	uint32_t v = USB_EP(ep_id);
	v = (v ^ (USB_EP_STAT_STALL << USB_P_EP_STAT_RX));
	v = (v ^ (USB_EP_STAT_STALL << USB_P_EP_STAT_TX));
	USB_EP(ep_id) = v & (USB_EP_INV_MASK | USB_EP_STAT_TX | USB_EP_STAT_RX);
}

static void ep_reg_clear(uint32_t ep_id, uint32_t bm) {
	uint32_t v = USB_EP(ep_id);
	USB_EP(ep_id) = v & ~bm & USB_EP_INV_MASK;
}

void usb_write_ep(uint32_t ep_id, void *data, size_t n) {
	uint16_t *p = data;

	if(n > USB_EP0_MAX_PACKET_SZ) {
		n = USB_EP0_MAX_PACKET_SZ;
	}

	/* make sure we dont miss last byte if uneven length */
	size_t copy_n = n;

	if(copy_n % 2 != 0) {
		copy_n++;
	}

	uint32_t pma_base = USB_BDT_ADDR_TX(ep_id);

	for(int i = 0; i < copy_n/2; i++) {
		USB_PMA(pma_base + i * 2) = *p;
		p++;
	}

	USB_BDT_COUNT_TX(ep_id) = n;

	ep_reg_tx_valid(ep_id);
}

static bool default_custom_request_out_handler(uint32_t ep_id) {
	return true;
}

static bool (*custom_request_out_handler)(uint32_t);

static void handle_request(uint32_t ep_id) {
	uint32_t epv = USB_EP(ep_id);

	if(epv & USB_EP_CTR_RX) {
		/* clear interrupt bit */
		ep_reg_clear(ep_id, USB_EP_CTR_RX);

		/* process data */
		if(epv & USB_EP_SETUP) { /* SETUP packet */
			struct usb_device_descriptor dev_desc;
			struct usb_config_descriptor_container conf_desc;
			struct usb_str_descriptor str_desc;

			usb_parse_request(ep_id, &req);

			switch(req.which) {
			case USB_REQUEST_GET_DESC:
				switch(req.w_value_h) {
				case USB_DESC_TYPE_DEV:
					usb_desc_get_device(&dev_desc);
					usb_write_ep(ep_id, &dev_desc, req.w_length);
					break;
				case USB_DESC_TYPE_CONF:
					usb_desc_get_config_container(&conf_desc);
					usb_write_ep(ep_id, &conf_desc, req.w_length);
					break;
				case USB_DESC_TYPE_STR:
					usb_desc_get_string(req.w_value_l, &str_desc);
					usb_write_ep(ep_id, &str_desc,
							str_desc.size < req.w_length ?
							str_desc.size : req.w_length);
					break;
				default:
					ep_reg_stall(ep_id);
				}
				break;
			case USB_REQUEST_SET_CONF:
				if(req.w_value == 0) {
					/* unset conf */
				} else {
					/* set conf */
				}

				usb_write_ep(ep_id, NULL, 0);
				break;
			case USB_REQUEST_SET_ADDR:
				dev_usb_addr = req.w_value & 0x7f;
				usb_write_ep(ep_id, NULL, 0);
				break;
			default:
				/* do something */
				break;
			}

		} else { /* OUT packet */
			/* set RX valid */
			if(!custom_request_out_handler(ep_id)) {
				ep_reg_stall(ep_id);
				return;
			}

			ep_reg_rx_valid(ep_id);
		}

	}

	if(epv & USB_EP_CTR_TX) {
		ep_reg_clear(ep_id, USB_EP_CTR_TX);

		if(req.direction == USB_DIR_DEV_TO_HOST) {
			ep_reg_rx_valid(ep_id);
		} else {
			USB_DADDR = USB_DADDR_EF | dev_usb_addr;
		}
	} else {
	}
}

void irq_usb_lp_can_rx0_handler() {
	/* check if reset interrupt */
	if(USB_ISTR & USB_ISTR_RESET) {
		reset();

		/* reset flag */
		USB_ISTR = ~USB_ISTR_RESET;
	}

	if(USB_ISTR & USB_ISTR_SOF) {
		/* reset flag */
		USB_ISTR = ~USB_ISTR_SOF;
//		lcd_print_x32(0xff01, 1,3);
	}

	if(USB_ISTR & USB_ISTR_SUSP) {
		USB_ISTR = ~USB_ISTR_SUSP;
//		lcd_print_x32(0xff02, 1,3);
	}

	if(USB_ISTR & USB_ISTR_WKUP) {
		USB_ISTR = ~USB_ISTR_WKUP;
		lcd_print_x32(0xff03, 1,3);
	}

	if(USB_ISTR & USB_ISTR_ERR) {
		USB_ISTR = ~USB_ISTR_ERR;
		lcd_print_x32(0xff04, 1,3);
	}

	if(USB_ISTR & USB_ISTR_ESOF) {
		USB_ISTR = ~USB_ISTR_ESOF;
//		lcd_print_x32(0xff05, 1,3);
	}

	if(USB_ISTR & USB_ISTR_PMAOVR) {
		USB_ISTR = ~USB_ISTR_PMAOVR;
		lcd_print_x32(0xff06, 1,3);
	}

	while(USB_ISTR & USB_ISTR_CTR) {
		handle_request(USB_ISTR & USB_ISTR_EP_ID);
	}
}

void usb_set_custom_request_out_handler(bool (*handler)(uint32_t)) {
	custom_request_out_handler = handler;
}

void usb_init() {
	/* setup default handlers */
	custom_request_out_handler = default_custom_request_out_handler;

	/* enable usb */
	RCC_APB1ENR |= RCC_APB1ENR_USB_ENABLE;

	/* setup irq */
	NVIC_ISER0 |= (1 << USB_LP_CAN_RX0);

	/*
	pa11 usb rx
	pa12 usb tx
	*/

	GPIOA_CRH &= 0xFFF00FFF;
	GPIOA_CRH |= 0x00077000;
	//GPIOA_CRH |= 0x00078000;
	GPIOA_BRR = 0x1800;

	/* force reset of usb */
	USB_CNTR = USB_CNTR_FRES;

	/*
		run 10us (if 72mhz clock) according to Tstartup
		1/72000000 = 0.014us/cycle => 10us/0.014us = 720 cycles
	*/

	for (int i = 0; i <720; ++ i) {
		asm("NOP");
	}

	/* clear interrupt statuses */
	USB_ISTR = 0;

	/* reset interrupt enable */
	USB_CNTR = USB_CNTR_RESETM;
}
