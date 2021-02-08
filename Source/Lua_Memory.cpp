// PSP Lua Memory ( FileName: Lua_Memory.cpp )
// ------------------------------------------------------------------------
// Version: 1.00
// Copyright (c) 2012 M4MSOFT
// ------------------------------------------------------------------------

// Include C/C++ header files
#include <valloc.h>
#include <malloc.h>

// Include the psp usblib Header file.
#include "pspusblib.h"

// Include the Lua header files for C++.
#include "luaplayer.hpp"
#include "Lua_Memory.hpp"

// Define the constant values with internal linkage.
static const int C_INT_MEMORYBLOK_SIZE = 1024 * 1024;

// -----------------------------------------------------------------------------------------------------
// Define the function prototypes
static int lua_FreeRam(lua_State * L);
static int lua_FreeVram(lua_State * L);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Return the free ram memory space in bytes.
static int lua_FreeRam(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "Argument error: Memory.freeRam() takes no arguments.");

	// Define the pointer Array.
	void* pArrayBuf[64];

	// for a realistic number, try to allocate 0.5 mb blocks until out of memory
	int i;
	for (i = 0; i < 64; ++i)
	{
		pArrayBuf[i] = malloc(C_INT_MEMORYBLOK_SIZE);
		if (!pArrayBuf[i])
		{
			--i;
			break;
		}
	}

	// Return the current free ram memory in bytes.
	lua_pushinteger(L, ((++i)*C_INT_MEMORYBLOK_SIZE)); // First result.

	// Free the ram memory
	for (; i >= 0; --i)
		free(pArrayBuf[i]);

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Return the free vram memory space in bytes.
static int lua_FreeVram (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "Argument error: Memory.freeVram() takes no arguments.");

	// Return the current free vram memory in bytes.
	lua_pushinteger(L, vmemavail());

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Define the usb module table.
static const luaL_Reg mod_lib[] =
{
	{"freeRam", lua_FreeRam},
	{"freeVram", lua_FreeVram},
	{NULL, NULL}
};
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Open the PSP lua Memory libraray.
LUAMOD_API int luaopen_Memory(lua_State * L)
{
	// Create the module.
	luaL_newlib(L, mod_lib);

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
