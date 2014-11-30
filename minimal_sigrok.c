#include <stdlib.h>
#include <stdio.h>
#include <libusb-1.0/libusb.h>

#define NBUF	3
unsigned char async_buf[NBUF][1024*1024];
unsigned char sync_buf[1024*1024];
unsigned char cmd_start[3] = {0x40, 0x00, 0x01};

#define EP_BULK		(2 | LIBUSB_ENDPOINT_IN)
#define EP_START	(LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_ENDPOINT_OUT)
#define CMD_START	0xb1

void callback(struct libusb_transfer *utp)
	{
	write(1, utp->buffer, utp->actual_length);
	libusb_submit_transfer(utp); /* recycle */
	}
main()
	{
	libusb_context		*uctxp;
	libusb_device_handle 	*udevp;
	struct libusb_device	*udp;
	struct libusb_transfer	*utp;
	int	n_read;
	int	i;

	/* init */
	libusb_init(&uctxp);
	udevp = libusb_open_device_with_vid_pid(uctxp, 0x0925, 0x3881); /* Sigrok - Saleae clone */
	udp = libusb_get_device(udevp);
	libusb_claim_interface(udevp, 0);

	/* prepare asynchronous transfers */
	for (i = 0; i < NBUF; i++)
		{
		utp = libusb_alloc_transfer(0);
		libusb_fill_bulk_transfer(utp, udevp, EP_BULK, async_buf[i], sizeof(async_buf[0]), callback, 0, 2000);
		libusb_submit_transfer(utp);
		}

	libusb_control_transfer(udevp, EP_START, CMD_START, 0, 0, cmd_start, sizeof(cmd_start), 100);

	while (1)
		{
		libusb_handle_events(uctxp);
		}
	}
