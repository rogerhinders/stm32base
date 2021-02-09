#ifndef USB_H
#define USB_H

#include <stdint.h>
#include <stddef.h>
#include "stm32f103.h"
#include "util.h"
#include "usb_desc.h"
#include "lcd.h"

#define USB_DIR_HOST_TO_DEV 0
#define USB_DIR_DEV_TO_HOST 1

#define USB_REQ_TYPE_STANDARD 0
#define USB_REQ_TYPE_CLASS 1
#define USB_REQ_TYPE_VENDOR 2
#define USB_REQ_TYPE_RESERVED 3

#define USB_REQ_RECIPIENT_DEV 0
#define USB_REQ_RECIPIENT_IFACE 1
#define USB_REQ_RECIPIENT_EP 2
#define USB_REQ_RECIPIENT_OTHER 3
#define USB_REQ_RECIPIENT_RESERVED 4

#define USB_REQUEST_CLEAR_FEATURE 1
#define USB_REQUEST_GET_CONF 8
#define USB_REQUEST_GET_DESC 6
#define USB_REQUEST_GET_IFACE 10
#define USB_REQUEST_GET_STATUS 0
#define USB_REQUEST_SET_ADDR 5
#define USB_REQUEST_SET_CONF 9
#define USB_REQUEST_SET_DESC 7
#define USB_REQUEST_SET_FEAT 3
#define USB_REQUEST_SET_IFACE 11
#define USB_REQUEST_SYNC_FRAME 12

#define USB_TXN_TYPE_IN 0
#define USB_TXN_TYPE_OUT 1
#define USB_TXN_TYPE_SETUP 2

struct usb_request {
	uint8_t direction;
	uint8_t type;
	uint8_t recipient;
	uint8_t which;
	uint16_t w_value;
	uint8_t w_value_l;
	uint8_t w_value_h;
	uint16_t w_index;
	uint16_t w_length;
};

void irq_usb_lp_can_rx0_handler();
void usb_init();

#endif /* USB_H */
