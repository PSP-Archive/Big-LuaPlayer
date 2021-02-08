// Psp Irda library ( FileName: pspirdalib.c )
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

// Include the psp irda Header files.
#include "pspirdalib.h"

// -----------------------------------------------------------------------------------------------------
// Private variables.
static bool init = false;
static SceUID irda_fd = -1;
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Initializes the irda library.
int pspirda_init()
{
	if (init)
		return 0;

	// Create the IRDA handle.
	if ((irda_fd = sceIoOpen("irda0:", PSP_O_RDWR, 0)) < 0);
		return -1;

	init = true;
	return 0;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Check if the library has been initalized.
int pspirda_initalized()
{
	return init;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Shutdowns the irda library.
void pspirda_term()
{
	if (!init)
		return;

	// Destroy the IRDA handle.
	sceIoClose(irda_fd);
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// write data to a serial port.
int pspirda_write(const void *data, unsigned int size)
{
	if (!init) return -1;
	return sceIoWrite(irda_fd, data, size);
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
int pspirda_read(void *data, unsigned int size)
{
	if (!init) return -1;
	return sceIoWrite(irda_fd, data, size);
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
signed long pspirda_seek(signed long offset, int whence)
{
	if (!init) return -1;
	return sceIoLseek(irda_fd, offset, whence);
}
// -----------------------------------------------------------------------------------------------------
