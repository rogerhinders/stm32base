#include "usb.h"

uint32_t dev_usb_addr = 0;

void usb_parse_request(volatile const uint32_t *src, struct usb_request *dst) {
	uint8_t rtype = *src & 0xff;

	dst->direction = rtype & 0x80 ? USB_DIR_DEV_TO_HOST : USB_DIR_HOST_TO_DEV;
	dst->type = rtype >> 5 & 3;
	dst->recipient = rtype & 0x1f;

	dst->which = *src >> 8 & 0xff;

	src++;

	dst->w_value = *src & 0xffff;
	dst->w_value_l = *src & 0xff;
	dst->w_value_h = (*src >> 8) & 0xff;

	src++;
	dst->w_index = *src & 0xffff;

	src++;
	dst->w_length = *src & 0xffff;
}

void usb_get_device_descriptor(struct usb_device_descriptor *dst) {
	dst->size = sizeof(struct usb_device_descriptor);
	dst->type = USB_DESC_TYPE_DEV;
	dst->bcd_spec = 0x0200;
	dst->device_class = 0x00;
	dst->device_sub_class = 0x00;
	dst->device_protocol = 0x00;
	dst->max_packet_size_ep0 = USB_EP0_MAX_PACKET_SZ;
	dst->id_vendor = USB_VENDOR_ID;
	dst->id_product = USB_PRODUCT_ID;
	dst->bcd_device = USB_DEVICE_VER;
	dst->index_manufacturer = USB_STR_DESC_IDX_MANUFACTURER;
	dst->index_product = USB_STR_DESC_IDX_PRODUCT;
	dst->index_serial_number = USB_STR_DESC_IDX_SERIAL_NUM;
	dst->n_configurations = 1;
}

void usb_get_config_descriptor(struct usb_config_descriptor *dst) {
	dst->size = sizeof(struct usb_config_descriptor);
	dst->type = USB_DESC_TYPE_CONF;
	dst->total_size = sizeof(struct usb_config_descriptor_container);
	dst->n_ifaces = 1;
	dst->config_value = 1;
	dst->config_index = 0;
	dst->attributes = 0x80;
	dst->max_power = (200/2);
}

void usb_get_iface_descriptor(struct usb_iface_descriptor *dst) {
	dst->size = sizeof(struct usb_iface_descriptor);
	dst->type = USB_DESC_TYPE_IFACE;
	dst->number = 0;
	dst->alt_setting = 0;
	dst->n_endpoints = 1;
	dst->class_code = 0xff;
	dst->sub_class_code = 0xff;
	dst->protocol = 0xff;
	dst->index_str = 0;
}

void usb_get_ep_descriptor(struct usb_ep_descriptor *dst) {
	dst->size = sizeof(struct usb_ep_descriptor);
	dst->type = USB_DESC_TYPE_EP;
	dst->ep_address = 1;
	dst->attributes = 0x3; //interrupt
	dst->max_packet_size = 64;
	dst->interval = 20;
}

void usb_get_config_descriptor_container(
		struct usb_config_descriptor_container *dst) {
	usb_get_config_descriptor(&dst->config);
	usb_get_iface_descriptor(&dst->iface);
	usb_get_ep_descriptor(&dst->ep);
}

void usb_get_string_descriptor(uint32_t id, struct usb_str_descriptor *dst) {
	dst->type = USB_DESC_TYPE_STR;

	switch(id) {
	case USB_STR_DESC_IDX_LANG:
		dst->size = 4;
		/* us_EN */
		dst->data[0] = 0x09;
		dst->data[1] = 0x04;
		break;
	case USB_STR_DESC_IDX_MANUFACTURER:
		dst->size = 2+18;
		dst->data[0] = 'M';
		dst->data[1] = 0;
		dst->data[2] = 'y';
		dst->data[3] = 0;
		dst->data[4] = ' ';
		dst->data[5] = 0;
		dst->data[6] = 'C';
		dst->data[7] = 0;
		dst->data[8] = 'o';
		dst->data[9] = 0;
		dst->data[10] = 'r';
		dst->data[11] = 0;
		dst->data[12] = 'p';
		dst->data[13] = 0;
		dst->data[14] = '.';
		dst->data[15] = 0;
		dst->data[16] = 0;
		dst->data[17] = 0;
		break;
	case USB_STR_DESC_IDX_PRODUCT:
		dst->size = 2+22;
		dst->data[0] = 'M';
		dst->data[1] = 0;
		dst->data[2] = 'y';
		dst->data[3] = 0;
		dst->data[4] = ' ';
		dst->data[5] = 0;
		dst->data[6] = 'P';
		dst->data[7] = 0;
		dst->data[8] = 'r';
		dst->data[9] = 0;
		dst->data[10] = 'o';
		dst->data[11] = 0;
		dst->data[12] = 'd';
		dst->data[13] = 0;
		dst->data[14] = 'u';
		dst->data[15] = 0;
		dst->data[16] = 'c';
		dst->data[17] = 0;
		dst->data[18] = 't';
		dst->data[19] = 0;
		dst->data[20] = 0;
		dst->data[21] = 0;
		break;
	case USB_STR_DESC_IDX_SERIAL_NUM:
		dst->size = 2+18;
		dst->data[0] = '3';
		dst->data[1] = 0;
		dst->data[2] = '1';
		dst->data[3] = 0;
		dst->data[4] =  '3';
		dst->data[5] = 0;
		dst->data[6] =  '3';
		dst->data[7] = 0;
		dst->data[8] =  '7';
		dst->data[9] = 0;
		dst->data[10] =  '-';
		dst->data[11] = 0;
		dst->data[12] =  '0';
		dst->data[13] = 0;
		dst->data[14] =  '1';
		dst->data[15] = 0;
		dst->data[16] = 0;
		dst->data[17] = 0;
		break;
	default:
		dst->size = 0;
		dst->data[0] = 0;
		dst->data[1] = 0;
	}
}

void usb_init() {
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

void init_usb_ep() {
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
	*(volatile uint32_t *)(MEM_CAN_USB_BASE  + USB_BTABLE_BASE_OFFSET + USB_BDT_USB_ADDRn_TX_OFFSET) = 0x40;
//	*(volatile uint32_t *)(MEM_CAN_USB_BASE  + USB_BTABLE_BASE_OFFSET + USB_BDT_USB_COUNTn_TX_OFFSET) = 0x80;
	*(volatile uint32_t *)(MEM_CAN_USB_BASE  + USB_BTABLE_BASE_OFFSET + USB_BDT_USB_ADDRn_RX_OFFSET) = 0xc0;
	*(volatile uint32_t *)(MEM_CAN_USB_BASE  + USB_BTABLE_BASE_OFFSET + USB_BDT_USB_COUNTn_RX_OFFSET) = 0x100;

	*(volatile uint32_t *)(MEM_CAN_USB_BASE  + USB_BTABLE_BASE_OFFSET + USB_BDT_USB_COUNTn_TX_OFFSET) = 0x00;
	*(volatile uint32_t *)(
			MEM_CAN_USB_BASE  + USB_BTABLE_BASE_OFFSET
			+ USB_BDT_USB_COUNTn_RX_OFFSET) = USB_COUNTN_RX_BLSIZE | (2 << USB_P_COUNTN_RX_NUM_BLOCK);

	/* enable ep0 */
/*	uint32_t ep = USB_EP0R & USB_EP_INV_MASK;
	ep |= USB_EP_TYPE_CNTR << USB_P_EP_EP_TYPE;
	ep |= USB_EP_STAT_VALID << USB_P_EP_STAT_RX;
	ep |= USB_EP_STAT_VALID << USB_P_EP_STAT_TX;
*/
	/* enable device */
	USB_DADDR = USB_DADDR_EF;

	USB_EP0R = (USB_EP_TYPE_CNTR << USB_P_EP_EP_TYPE) | (USB_EP_STAT_VALID << USB_P_EP_STAT_RX);

}

void usb_write_ep(void *data, size_t n) {
	uint16_t *p = data;

	if(n > USB_EP0_MAX_PACKET_SZ) {
		n = USB_EP0_MAX_PACKET_SZ;
	}

	/* make sure we dont miss last byte if uneven length */
	size_t copy_n = n;

	if(copy_n % 2 != 0) {
		copy_n++;
	}

	for(int i = 0; i < copy_n/2; i++) {
		*(volatile uint32_t *)(MEM_CAN_USB_BASE + 0x40*2 + i*4) = *p;
		p++;
	}

	*(volatile uint32_t *)(MEM_CAN_USB_BASE  + USB_BTABLE_BASE_OFFSET + USB_BDT_USB_COUNTn_TX_OFFSET) = n;
	//USB_EP0R = (USB_EP0R ^ (USB_EP_STAT_VALID << USB_P_EP_STAT_TX)) & (USB_EP_INV_MASK | USB_EP_STAT_TX);
}

uint32_t usb_ep_reg_get(uint32_t ep_id) {
	switch(ep_id) {
	case 0: return USB_EP0R; break;
	case 1: return USB_EP1R; break;
	case 2: return USB_EP2R; break;
	case 3: return USB_EP3R; break;
	case 4: return USB_EP4R; break;
	case 5: return USB_EP5R; break;
	case 6: return USB_EP6R; break;
	case 7: return USB_EP7R; break;
	}

	return USB_EP0R;
}

void usb_ep_reg_set(uint32_t ep_id, uint32_t v) {
	switch(ep_id) {
	case 0: USB_EP0R = v; break;
	case 1: USB_EP1R = v; break;
	case 2: USB_EP2R = v; break;
	case 3: USB_EP3R = v; break;
	case 4: USB_EP4R = v; break;
	case 5: USB_EP5R = v; break;
	case 6: USB_EP6R = v; break;
	case 7: USB_EP7R = v; break;
	}
}

void usb_ep_reg_rx_valid(uint32_t ep_id) {
	uint32_t v = (usb_ep_reg_get(ep_id) ^ (USB_EP_STAT_VALID << USB_P_EP_STAT_RX)) & (USB_EP_INV_MASK | USB_EP_STAT_RX);
	usb_ep_reg_set(ep_id, v);
}

void usb_ep_reg_tx_valid(uint32_t ep_id) {
	uint32_t v = (usb_ep_reg_get(ep_id) ^ (USB_EP_STAT_VALID << USB_P_EP_STAT_TX)) & (USB_EP_INV_MASK | USB_EP_STAT_TX);
	usb_ep_reg_set(ep_id, v);
}

void usb_ep_reg_stall(uint32_t ep_id) {
	uint32_t v = usb_ep_reg_get(ep_id);
	v = (v ^ (USB_EP_STAT_STALL << USB_P_EP_STAT_RX));
	v = (v ^ (USB_EP_STAT_STALL << USB_P_EP_STAT_TX));
	usb_ep_reg_set(ep_id, v & (USB_EP_INV_MASK | USB_EP_STAT_TX | USB_EP_STAT_RX));
}

void usb_ep_reg_clear(uint32_t ep_id, uint32_t bm) {
	uint32_t v = usb_ep_reg_get(0);
	usb_ep_reg_set(ep_id, v & ~bm & USB_EP_INV_MASK);
}

struct usb_request req;

void usb_handle_request(uint32_t ep_id) {
	uint32_t epv = usb_ep_reg_get(ep_id);

	if(epv & USB_EP_CTR_RX) {
		/* clear interrupt bit */
		usb_ep_reg_clear(ep_id, USB_EP_CTR_RX);

		/* process data */
		if(epv & USB_EP_SETUP) { /* SETUP packet */
			struct usb_device_descriptor dev_desc;
			struct usb_config_descriptor_container conf_desc;
			struct usb_str_descriptor str_desc;

			usb_parse_request(
					(volatile uint32_t *)(MEM_CAN_USB_BASE + (0xc0*2)), &req);

			switch(req.which) {
			case USB_REQUEST_GET_DESC:
				switch(req.w_value_h) {
				case USB_DESC_TYPE_DEV:
					usb_get_device_descriptor(&dev_desc);
					usb_write_ep(
							&dev_desc, req.w_length);
					usb_ep_reg_tx_valid(ep_id);
					break;
				case USB_DESC_TYPE_CONF:
					usb_get_config_descriptor_container(&conf_desc);
					usb_write_ep(&conf_desc, req.w_length);
					usb_ep_reg_tx_valid(ep_id);
					break;
				case USB_DESC_TYPE_STR:
					usb_get_string_descriptor(req.w_value_l, &str_desc);
					usb_write_ep(&str_desc,
							str_desc.size < req.w_length ?
							str_desc.size : req.w_length);
					usb_ep_reg_tx_valid(ep_id);
					break;
				default:
					usb_ep_reg_stall(ep_id);
				}
				break;
			case USB_REQUEST_SET_CONF:
				if(req.w_value == 0) {
					/* unset conf */
				} else {
					/* set conf */
				}

				usb_write_ep(NULL, 0);
				usb_ep_reg_tx_valid(ep_id);
				break;
			case USB_REQUEST_SET_ADDR:
				dev_usb_addr = req.w_value & 0x7f;
				usb_write_ep(NULL, 0);
				usb_ep_reg_tx_valid(ep_id);
				break;
			default:
				/* do something */
				break;
			}

		} else { /* OUT packet */
			/* set RX valid */
				//h32(USB_EP0R,0);
			usb_ep_reg_rx_valid(ep_id);
		}

	}

	if(epv & USB_EP_CTR_TX) {
		usb_ep_reg_clear(ep_id, USB_EP_CTR_TX);

		if(req.direction == USB_DIR_DEV_TO_HOST) {
			usb_ep_reg_rx_valid(ep_id);
		} else {
			USB_DADDR = USB_DADDR_EF | dev_usb_addr;
		}
	} else {
	}
}

void irq_usb_lp_can_rx0_handler() {
	/* check if reset interrupt */
	if(USB_ISTR & USB_ISTR_RESET) {
		init_usb_ep();

		/* reset flag */
		USB_ISTR = ~USB_ISTR_RESET;
	}

	if(USB_ISTR & USB_ISTR_SOF) {
		/* reset flag */
		USB_ISTR = ~USB_ISTR_SOF;
	}

	if(USB_ISTR & USB_ISTR_SUSP) {
		USB_ISTR = ~USB_ISTR_SUSP;
	}

	if(USB_ISTR & USB_ISTR_WKUP) {
		USB_ISTR = ~USB_ISTR_WKUP;
	}

	if(USB_ISTR & USB_ISTR_ERR) {
		USB_ISTR = ~USB_ISTR_ERR;
	}

	if(USB_ISTR & USB_ISTR_ESOF) {
		USB_ISTR = ~USB_ISTR_ESOF;
	}

	if(USB_ISTR & USB_ISTR_PMAOVR) {
		USB_ISTR = ~USB_ISTR_PMAOVR;
	}

	while(USB_ISTR & USB_ISTR_CTR) {
		usb_handle_request(USB_ISTR & USB_ISTR_EP_ID);

	}
}
