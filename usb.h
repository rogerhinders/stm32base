#ifndef USB_H
#define USB_H

#include <stdint.h>
#include <stdio.h>
#include "stm32f103.h"
#include "util.h"

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

#define USB_DESC_TYPE_DEV 1
#define USB_DESC_TYPE_CONF 2
#define USB_DESC_TYPE_STR 3
#define USB_DESC_TYPE_IFACE 4
#define USB_DESC_TYPE_EP 5
#define USB_DESC_TYPE_DEVQ 6
#define USB_DESC_TYPE_OTHER_SCONF 7
#define USB_DESC_TYPE_IFACE_PWR 8
#define USB_DESC_TYPE_OTG 9

#define USB_EP0_MAX_PACKET_SZ 64
#define USB_VENDOR_ID 0xC251
#define USB_PRODUCT_ID 0x1C01
#define USB_DEVICE_VER 0x0100

#define USB_TXN_TYPE_IN 0
#define USB_TXN_TYPE_OUT 1
#define USB_TXN_TYPE_SETUP 2

#define USB_STR_DESC_IDX_LANG 0
#define USB_STR_DESC_IDX_MANUFACTURER 1
#define USB_STR_DESC_IDX_PRODUCT 2
#define USB_STR_DESC_IDX_SERIAL_NUM 3

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



struct __attribute__((__packed__)) usb_device_descriptor {
	uint8_t size;
	uint8_t type;
	uint16_t bcd_spec;
	uint8_t device_class;
	uint8_t device_sub_class;
	uint8_t device_protocol;
	uint8_t max_packet_size_ep0;
	uint16_t id_vendor;
	uint16_t id_product;
	uint16_t bcd_device;
	uint8_t index_manufacturer;
	uint8_t index_product;
	uint8_t index_serial_number;
	uint8_t n_configurations;
};

struct __attribute__((__packed__)) usb_config_descriptor {
	uint8_t size;
	uint8_t type;
	uint16_t total_size;
	uint8_t n_ifaces;
	uint8_t config_value;
	uint8_t config_index;
	uint8_t attributes;
	uint8_t max_power;
};

struct __attribute__((__packed__)) usb_iface_descriptor {
	uint8_t size;
	uint8_t type;
	uint8_t number;
	uint8_t alt_setting;
	uint8_t n_endpoints;
	uint8_t class_code;
	uint8_t sub_class_code;
	uint8_t protocol;
	uint8_t index_str;
};

struct __attribute__((__packed__)) usb_ep_descriptor {
	uint8_t size;
	uint8_t type;
	uint8_t ep_address;
	uint8_t attributes;
	uint16_t max_packet_size;
	uint8_t interval;
};

struct __attribute__((__packed__)) usb_str_descriptor {
	uint8_t size;
	uint8_t type;
	uint8_t data[62];
};

struct __attribute__((__packed__)) usb_config_descriptor_container {
	struct usb_config_descriptor config;
	struct usb_iface_descriptor iface;
	struct usb_ep_descriptor ep;
};

void usb_parse_request(volatile const uint32_t *src, struct usb_request *dst);
void usb_get_device_descriptor(struct usb_device_descriptor *dst);
void usb_get_config_descriptor(struct usb_config_descriptor *dst);
void usb_get_iface_descriptor(struct usb_iface_descriptor *dst);
void usb_get_ep_descriptor(struct usb_ep_descriptor *dst);
void usb_get_config_descriptor_container(
		struct usb_config_descriptor_container *dst);
void usb_get_string_descriptor(uint32_t id, struct usb_str_descriptor *dst);
void usb_init();
void init_usb_ep();
void usb_write_ep(void *data, size_t n);
void usb_handle_request(uint32_t ep_id);
void irq_usb_lp_can_rx0_handler();

#endif /* USB_H */
