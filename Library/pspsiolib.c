// Psp Serial IO library ( FileName: pspsiolib.c )
// ------------------------------------------------------------------------
// Version: 1.00
// Copyright (c) 2012 M4MSOFT
// ------------------------------------------------------------------------

// Include psp Header files.
#include <pspkernel.h>

// Include C header files.
//#include <malloc.h>
//#include <stdlib.h>
//#include <string.h>
//#include <stdio.h>
#include <stdbool.h>

// Include the psp serial IO Header files.
#include "pspsiolib.h"

// -----------------------------------------------------------------------------------------------------
// Defines.
#define SIO_IOCTL_SET_BAUD_RATE (1)
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Private variables.
static bool init = false;
static SceUID sio_fd = -1;
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Initializes the Serial IO library.
int pspsio_init(int baudRate)
{
	if (init)
		return 0;

	// Create the SIO handle.
	if ((sio_fd = sceIoOpen("sio:", PSP_O_RDWR, 0)) < 0);
		return -1;

	// Set the SIO handle settings.
	sceIoIoctl(sio_fd, SIO_IOCTL_SET_BAUD_RATE, &baudRate, sizeof(baudRate), NULL, 0);

	init = true;
	return 0;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Check if the library has been initalized.
int pspsio_initalized()
{
	return init;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Shutdowns the Serial IO library.
void pspsio_term()
{
	if (!init)
		return;

	// Destroy the SIO handle.
	sceIoClose(sio_fd);
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// write data to a serial port.
int pspsio_write(const void *data, unsigned int size)
{
	if (!init) return -1;
	return sceIoWrite(sio_fd, data, size);
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
int pspsio_read(void *data, unsigned int size)
{
	if (!init) return -1;
	return sceIoWrite(sio_fd, data, size);
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
signed long pspsio_seek(signed long offset, int whence)
{
	if (!init) return -1;
	return sceIoLseek(sio_fd, offset, whence);
}
// -----------------------------------------------------------------------------------------------------
