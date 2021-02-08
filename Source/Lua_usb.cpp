// PSP Lua usb ( FileName: Lua_usb.cpp )
// ------------------------------------------------------------------------
// Version: 1.00
// Copyright (c) 2012 M4MSOFT
// ------------------------------------------------------------------------

// Include the psp usblib Header file.
#include "pspusblib.h"

// Include the Lua header files for C++.
#include "luaplayer.hpp"
#include "Lua_usb.hpp"

// -----------------------------------------------------------------------------------------------------
// Define the function prototypes
static int lua_usbInit(lua_State * L);
static int lua_usbTerm(lua_State * L);
static int lua_usbActivate(lua_State * L);
static int lua_usbDeactivate(lua_State * L);
static int lua_usbIsActivated (lua_State * L);
static int lua_usbCable(lua_State * L);
static int lua_usbConnected(lua_State * L);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Initializes the usb driver.
static int lua_usbInit (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "Argument error: usb.initialize() takes no arguments.");

	// Initializes the usb driver and return the result.
	lua_pushboolean(L, pspUSB_Init());

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Terminate the usb driver.
static int lua_usbTerm (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "Argument error: usb.terminate() takes no arguments.");

	// Terminate the usb driver and return the result.
	lua_pushboolean(L, pspUSB_Term());

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Activate the usb driver.
static int lua_usbActivate (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "Argument error: usb.activate() takes no arguments.");

	// Activate the usb driver and return the result.
	lua_pushboolean(L, pspUSB_activate());

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Deactivate the usb driver.
static int lua_usbDeactivate (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "Argument error: usb.deactivate() takes no arguments.");

	// Deactivate the usb driver and return the result.
	lua_pushboolean(L, pspUSB_deactivate());

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Chack if the usb driver is Activated.
static int lua_usbIsActivated (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "Argument error: usb.isActivated() takes no arguments.");

	// Return the status of the usb driver.
	lua_pushboolean(L, pspUSB_isActivated());

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Chack if the usb cable is connected.
static int lua_usbCable (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "Argument error: usb.cable() takes no arguments.");

	// Return the status of the usb cable.
	lua_pushboolean(L, pspUSB_cable());

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Chack if the usb connection established.
static int lua_usbConnected (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "Argument error: usb.connected() takes no arguments.");

	// Return the status of the usb connection.
	lua_pushboolean(L, pspUSB_connected());

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Define the usb module table.
static const luaL_Reg mod_lib[] =
{
	{"initialize", lua_usbInit},
	{"terminate", lua_usbTerm},
	{"activate", lua_usbActivate},
	{"deactivate", lua_usbDeactivate},
	{"isActivated", lua_usbIsActivated},
	{"cable", lua_usbCable},
	{"connected", lua_usbConnected},
	{NULL, NULL}
};
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Open the PSP lua usb libraray.
LUAMOD_API int luaopen_USB(lua_State * L)
{
	// Create the module.
	luaL_newlib(L, mod_lib);

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
