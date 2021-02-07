#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <libusb-1.0/libusb.h>

#define DEV_VID 0xC251
#define DEV_PID 0x1C01

bool upload_fw() {
	bool ret = false;
	libusb_device **devices;
	libusb_device *device;
	struct libusb_device_descriptor dev_desc;
	struct libusb_config_descriptor *conf_desc = NULL;
	libusb_device_handle *handle = NULL;
	int err, config;

	int i = 0;

	if((err = libusb_get_device_list(NULL, &devices)) < 0) {
		printf("failed getting devices\n");
		goto _upload_fw_cleanup;
	}

	while((device = devices[i++]) != NULL) {

		if((err = libusb_get_device_descriptor(device, &dev_desc)) < 0) {
			printf("error getting descriptor\n");
			goto _upload_fw_cleanup;
		}

		if(dev_desc.idVendor == DEV_VID && dev_desc.idProduct == DEV_PID) {
			break;
		}
	}

	if(device == NULL) {
		printf("unable to find device\n");
		goto _upload_fw_cleanup;
	}

	if((err = libusb_get_config_descriptor(device, 0, &conf_desc)) < 0) {
		printf("error getting conf descriptor\n");
		goto _upload_fw_cleanup;
	}

	printf("num ifaces: %d\n", conf_desc->bNumInterfaces);
	if(conf_desc->bNumInterfaces < 1) {
		printf("error: no interfaces!\n");
		goto _upload_fw_cleanup;
	}

	if(conf_desc->interface[0].num_altsetting < 1) {
		printf("error: no iface descriptors\n");
		goto _upload_fw_cleanup;
	}

	if(conf_desc->interface[0].altsetting->bNumEndpoints < 1) {
		printf("error: no endpoints\n");
		goto _upload_fw_cleanup;
	}

	uint8_t ep_addr = conf_desc->interface[0].altsetting
		->endpoint[0].bEndpointAddress;

	printf("EP addr: %d\n",
			conf_desc->interface[0].altsetting->endpoint[0].bEndpointAddress);

	if((err = libusb_open(device, &handle)) < 0) {
		printf("error opening device: %d\n", err);
		goto _upload_fw_cleanup;
	}

	if((err = libusb_get_configuration(handle, &config)) < 0) {
		printf("error getting config: %d\n", err);
		goto _upload_fw_cleanup;
	}

	if((err = libusb_set_configuration(handle, config)) < 0) {
		printf("unable to set config: %d\n", err);
		goto _upload_fw_cleanup;
	}

	if((err = libusb_claim_interface(handle, 0)) < 0) {
		printf("error: couldnt claim iface: %d\n", err);
		goto _upload_fw_cleanup;
	}

	printf("active conf: %d\n", config);

	if(libusb_kernel_driver_active(handle, 0) == 1) {
		printf("kernel driver is active\n");

		if(libusb_detach_kernel_driver(handle, 0) == 0) {
			printf("detached kernel driver\n");
		} else {
			printf("error: unable to detach kernel driver!\n");
			goto _upload_fw_cleanup;
		}
	}

	uint8_t tmp[25];

	for(int i = 0; i < 25; i++) {
		tmp[i] = i;
	}

	int n_written;

	if((err = libusb_bulk_transfer(
					handle, ep_addr, tmp, 25, &n_written, 5000)) < 0) {
		printf("error writing data: %d\n", err);
		goto _upload_fw_cleanup;
	}

	printf("done writing data\n");

	ret = true;			

_upload_fw_cleanup:
	libusb_release_interface(handle, 0);
	libusb_free_device_list(devices, 1);
	libusb_free_config_descriptor(conf_desc);
	libusb_close(handle);
	return ret;
}

int main() {
	int r = libusb_init(NULL);

	if(r < 0) {
		printf("libusb init err\n");
		return 1;
	}

	upload_fw();	

	libusb_exit(NULL);

	return 0;
}
