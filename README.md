MinimalBulkRead
===============

Minimalist code to bulk-read with libusb from a Saleae clone running the sigrok firmware

Of course, all return values should be tested and error handled...
This is just the shortest functional code I wrote to log data from
a Saleae (logic analyser) clone running the Sigrok 'fx2lafw' firmware.

The firmware should be pre-loaded with something like :

$ cycfx2prog -d=001.003 prg:fx2lafw-saleae-logic.ihx run
	(lsusb --> Lakeview -> device address)
	(0925:3881 Lakeview Research Saleae Logic)

The program just needs libusb (1.x) to compile,
it connects to the device, prepares asynchronous bulk read operation
and send a control command to start the acquisition at full speed (24 Mega-samples/second)
(this can be changed by modifying cmd_start[] which contains a divisor)

The incoming data are written on stdout within the callback function.
<ctrl>-C is used to stop the program (which doesn't stop gracefully but who cares?)

Note:
-----
* On Raspberry Pi, https://www.kernel.org/doc/Documentation/usb/usbmon.txt shows that
   transfers are sliced in 16 KB chunks, so, it is not usefull to have > 16*1024 buffers
   4 MSamples/sec works ...most of the time (cmd_start[2] = (48000000/4000000)-1 = 0x0b)

See also:
---------
* http://libusb.sourceforge.net/api-1.0/
* http://sigrok.org/wiki/MCU123_Saleae_Logic_clone
* http://sigrok.org/wiki/Fx2lafw
* apt-cache search cycfx2prog
* Cypress FX2, EZ-USB,...
