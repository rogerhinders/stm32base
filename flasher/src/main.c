#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <libusb-1.0/libusb.h>

#define DEV_VID 0xC251
#define DEV_PID 0x1C01

uint8_t *read_fw(char *path, size_t *n) {
	FILE *fp;
	uint8_t *buf = NULL;

	fp = fopen(path, "rb");

	if(fp == NULL) {
		goto _read_fw_cleanup;
	}

	fseek(fp, 0, SEEK_END);
	*n = ftell(fp);
	rewind(fp);

	buf = malloc(*n);

	if(buf == NULL) {
		goto _read_fw_cleanup;
	}

	size_t read_n = fread(buf, 1, *n, fp);

	if(read_n != *n) {
		free(buf);
		buf = NULL;
	}

_read_fw_cleanup:
	if(fp != NULL)
		fclose(fp);

	return buf;
}

bool upload_fw(uint8_t *buf, size_t n) {
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
	size_t max_packet_size = conf_desc->interface[0].altsetting
		->endpoint[0].wMaxPacketSize;

	printf("EP addr: %d\n",
			conf_desc->interface[0].altsetting->endpoint[0].bEndpointAddress);
	printf("max packet size: %lu\n", max_packet_size);

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

/*	if(libusb_control_transfer(
			handle,
			0x40, //req type, host-to-dev,vendor,device
			0x1, //1 = start fw flash
			0, // wValue
			0, // wIndex
			NULL, //data	
			0, //wLen
			5000 //timeout
			) < 0) {
		printf("error writing control data\n");
		goto _upload_fw_cleanup;
	}
	
	printf("sent control data\n");*/

//	size_t tot_written = 0;
	int n_written;
	
/*
	int32_t packet_size = 64;

	while(n_written < n) {
		printf("writing #%ld\n", tot_written / packet_size);
		if((err = libusb_bulk_transfer(
						handle, ep_addr, buf+tot_written, packet_size, &n_written, 5000)) < 0) {
			printf("error writing data: %d\n", err);
			goto _upload_fw_cleanup;
		}

		tot_written += n_written;
	}
*/
	if((err = libusb_bulk_transfer(
					handle, ep_addr, buf, n, &n_written, 5000)) < 0) {
		printf("error writing data: %d\n", err);
		goto _upload_fw_cleanup;
	}

	//printf("done writing data at %ld bytes\n", tot_written);
	printf("done writing data\n");

	ret = true;			

_upload_fw_cleanup:
	libusb_release_interface(handle, 0);
	libusb_free_device_list(devices, 1);
	libusb_free_config_descriptor(conf_desc);
	libusb_close(handle);
	return ret;
}

int main(int argc, char *argv[]) {
	if(argc != 2) {
		printf("usage: %s <path>\n", argv[0]);
		return 0;
	}

	int r = libusb_init(NULL);

	if(r < 0) {
		printf("libusb init err\n");
		return 1;
	}

	size_t n;
	uint8_t *buf = read_fw(argv[1], &n);

	if(buf == NULL) {
		printf("error reading file\n");
	} else {
		printf("read %s at %ldb\n", argv[1], n);
		upload_fw(buf, n);
		free(buf);
	}


	libusb_exit(NULL);

	return 0;
}
