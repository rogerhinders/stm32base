#ifndef _USB_DESC_H
#define _USB_DESC_H

#include <stdint.h>
#include <stdio.h>

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
#define USB_DESC_VENDOR_ID 0xC251
#define USB_DESC_PRODUCT_ID 0x1C01
#define USB_DESC_DEVICE_VER 0x0100

#define USB_DESC_STR_IDX_LANG 0
#define USB_DESC_STR_IDX_MANUFACTURER 1
#define USB_DESC_STR_IDX_PRODUCT 2
#define USB_DESC_STR_IDX_SERIAL_NUM 3

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


void usb_desc_get_device(struct usb_device_descriptor *dst);
void usb_desc_get_config(struct usb_config_descriptor *dst);
void usb_desc_get_iface(struct usb_iface_descriptor *dst);
void usb_desc_get_ep(struct usb_ep_descriptor *dst);
void usb_desc_get_config_container(
		struct usb_config_descriptor_container *dst);
void usb_desc_get_string(uint32_t id, struct usb_str_descriptor *dst);

#endif /* _USB_DESC_H */
