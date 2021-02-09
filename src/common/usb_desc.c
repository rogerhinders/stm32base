#include "common/usb_desc.h"

void usb_desc_get_device(struct usb_device_descriptor *dst) {
	dst->size = sizeof(struct usb_device_descriptor);
	dst->type = USB_DESC_TYPE_DEV;
	dst->bcd_spec = 0x0200;
	dst->device_class = 0x00;
	dst->device_sub_class = 0x00;
	dst->device_protocol = 0x00;
	dst->max_packet_size_ep0 = USB_EP0_MAX_PACKET_SZ;
	dst->id_vendor = USB_DESC_VENDOR_ID;
	dst->id_product = USB_DESC_PRODUCT_ID;
	dst->bcd_device = USB_DESC_DEVICE_VER;
	dst->index_manufacturer = USB_DESC_STR_IDX_MANUFACTURER;
	dst->index_product = USB_DESC_STR_IDX_PRODUCT;
	dst->index_serial_number = USB_DESC_STR_IDX_SERIAL_NUM;
	dst->n_configurations = 1;
}

void usb_desc_get_config(struct usb_config_descriptor *dst) {
	dst->size = sizeof(struct usb_config_descriptor);
	dst->type = USB_DESC_TYPE_CONF;
	dst->total_size = sizeof(struct usb_config_descriptor_container);
	dst->n_ifaces = 1;
	dst->config_value = 1;
	dst->config_index = 0;
	dst->attributes = 0x80;
	dst->max_power = (200/2);
}

void usb_desc_get_iface(struct usb_iface_descriptor *dst) {
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

void usb_desc_get_ep(struct usb_ep_descriptor *dst) {
	dst->size = sizeof(struct usb_ep_descriptor);
	dst->type = USB_DESC_TYPE_EP;
	dst->ep_address = 1;
	dst->attributes = 0x2; //bulk 0x3; //interrupt
	dst->max_packet_size = 64;
	dst->interval = 20;
}

void usb_desc_get_config_container(
		struct usb_config_descriptor_container *dst) {
	usb_desc_get_config(&dst->config);
	usb_desc_get_iface(&dst->iface);
	usb_desc_get_ep(&dst->ep);
}

void usb_desc_get_string(uint32_t id, struct usb_str_descriptor *dst) {
	dst->type = USB_DESC_TYPE_STR;

	switch(id) {
	case USB_DESC_STR_IDX_LANG:
		dst->size = 4;
		/* us_EN */
		dst->data[0] = 0x09;
		dst->data[1] = 0x04;
		break;
	case USB_DESC_STR_IDX_MANUFACTURER:
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
	case USB_DESC_STR_IDX_PRODUCT:
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
	case USB_DESC_STR_IDX_SERIAL_NUM:
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
