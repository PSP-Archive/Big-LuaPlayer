// Psp usb library ( FileName: pspusblib.c )
// ------------------------------------------------------------------------
// Version: 1.00
// Copyright (c) 2012 M4MSOFT
// ------------------------------------------------------------------------

// Include C/C++ header files
#include <stdbool.h>

// Include psp Header files.
#include <pspiofilemgr.h>
#include <pspmodulemgr.h>
#include <pspusb.h>
#include <pspusbstor.h>

// Include the PSP modlib Header files.
#include "pspmodlib.h"

// Include the psp usblib Header file.
#include "pspusblib.h"

// -----------------------------------------------------------------------------------------------------
// Defines.
#define USBLIB_BOOTCAPACITY 0x800000
#define USBLIB_DRIVERPID    0x1c8
#define USBLIB_MODCOUNT     5
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Structures
typedef struct
{
    const char* name;
	int modid;
} usbmodule;
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Private variables.
static bool usbInit = false;
static bool usbActivated = false;

static usbmodule usbMods[USBLIB_MODCOUNT] =
{
	{"semawm", -1},
	{"usbstor", -1},
	{"usbstormgr", -1},
	{"usbstorms", -1},
	{"usbstorboot", -1}
};
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Initializes the library.
int pspUSB_Init ()
{
	if (usbInit)
		return usbInit;

	// Load the psp usb modules.
	int i;
	for (i = 0; i < USBLIB_MODCOUNT; ++i)
	{
		usbMods[i].modid = pspmodStartPSPModule(usbMods[i].name);
		if (usbMods[i].modid < 0)
			goto label_error;
	}

	// Setup the usb driver.
	if (sceUsbStart(PSP_USBBUS_DRIVERNAME, 0, 0) != 0)
		goto label_error;
	if (sceUsbStart(PSP_USBSTOR_DRIVERNAME, 0, 0) != 0)
	{
		sceUsbStop(PSP_USBSTOR_DRIVERNAME, 0, 0);
		goto label_error;
	}
	if (sceUsbstorBootSetCapacity(USBLIB_BOOTCAPACITY) != 0)
	{
		sceUsbStop(PSP_USBSTOR_DRIVERNAME, 0, 0);
		sceUsbStop(PSP_USBBUS_DRIVERNAME, 0, 0);
		goto label_error;
	}

	usbInit = true;
	return usbInit;

	// Initialize failure... abort
label_error:
	for ( i = USBLIB_MODCOUNT - 1; i >= 0; --i)
		if (usbMods[i].modid >= 0)
			if (!pspmodStopModule(usbMods[i].modid))
				usbMods[i].modid = -1;

	return usbInit;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Shutdowns the library.
int pspUSB_Term ()
{
	if (!usbInit)
		return !usbInit;

	// deactivate the usb driver.
	if (!pspUSB_deactivate())
		return !usbInit;

	// Unload the usb driver.
	sceUsbStop(PSP_USBSTOR_DRIVERNAME, 0, 0);
	sceUsbStop(PSP_USBBUS_DRIVERNAME, 0, 0);

	// Unload the usb modules.
	int i;
	for ( i = USBLIB_MODCOUNT - 1; i >= 0; --i)
		if (usbMods[i].modid >= 0)
			if (!pspmodStopModule(usbMods[i].modid))
				usbMods[i].modid = -1;

	usbInit = false;
	return !usbInit;
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Activate the USB connection.
int pspUSB_activate()
{
	// Initializes the library.
	if (!usbInit)
		if (!pspUSB_Init())
			return false;

	// Activite the usb driver.
	if (!usbActivated)
		if (!sceUsbActivate(USBLIB_DRIVERPID))
			usbActivated = true;

	// return true on succes.
	return usbActivated;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Deactivate the USB connection.
int pspUSB_deactivate()
{
	if (!usbInit)
		return false;

	// Deactivate the USB driver.
	if (usbActivated)
		sceUsbDeactivate(USBLIB_DRIVERPID);
	if (!pspUSB_isActivated())
		usbActivated = false;

	return !usbActivated;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Chack if the usb driver is Activated.
int pspUSB_isActivated()
{
	if (!usbInit)
		return false;
	return (sceUsbGetState() & PSP_USB_ACTIVATED);
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Chack if the usb cable is connected.
int pspUSB_cable()
{
	if (!usbInit)
		return false;
	return (sceUsbGetState() & PSP_USB_CABLE_CONNECTED);
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Chack if the usb connection established.
int pspUSB_connected()
{
	if (!usbInit)
		return false;
	return (sceUsbGetState() & PSP_USB_CONNECTION_ESTABLISHED);
}
// -----------------------------------------------------------------------------------------------------

