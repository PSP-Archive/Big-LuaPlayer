// Big LUAPlayer ( FileName: main.cpp )
// -----------------------------------------------------------------------------------------------------
// Version: Beta 1.10
// Copyright (c) 2014 M4MSOFT
// -----------------------------------------------------------------------------------------------------

// Include C/C++ header files
#include <string.h>
#include <stdio.h>

// Include psp Header files.
#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspdebug.h>

// Include commen psp Header files.
#include "Common/callbacks.h"

// Include the Lua header files for C++.
#include "luaplayer.hpp"

// Select the luaplayer objects to use in this file.
using luaplayer::luaL_openplayerlibs;

// Declare the control check if a Constant value.
// const unsigned int PSP_CTRL_CHECK = PSP_CTRL_SELECT + PSP_CTRL_START +
//									  PSP_CTRL_TRIANGLE + PSP_CTRL_CIRCLE + PSP_CTRL_CROSS + PSP_CTRL_SQUARE +
//									  PSP_CTRL_HOME;

// Include the PSP glib2D Header file.
#include "glib2d.h"

// Include the psp serial IO Header files.
#include "pspsiolib.h"

// Include the psp irda Header files.
#include "pspirdalib.h"

// Define pspDebugScreenPrintf function.
#define printf pspDebugScreenPrintf

// Function prototypes
int SaveErrorToFile(const char * strPath, const char * strError);

// Define the module info section
PSP_MODULE_INFO("Big LuaPlayer v1.10", 0, 1, 1);

// Define the main thread's attribute value (optional)
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);

// Define the heap of free memory.
PSP_HEAP_SIZE_KB(20480); // 20 MB
//PSP_HEAP_SIZE_KB(10240); // 10 MB
//PSP_HEAP_SIZE_MAX();

// -----------------------------------------------------------------------------------------------------
// Main function
int main(int argc, char *argv[])
{
	setupCallbacks(); // Set up the system callback thread.

	// Start the graphics library.
	g2dResetDrawTarget();
	g2dClear(0x000000);
	g2dFlip(0);

	pspDebugScreenInit();
	pspDebugScreenEnableBackColor(0);

	// init Lua and load all libraries.
	printf("init Lua and load all libraries Lua.\n");
	lua_State * L = luaL_newstate();
	luaL_openlibs(L);
	printf("load all Big Luaplayer libraries.\n");
	luaL_openplayerlibs(L);

	// load script
	printf("\n");
	printf("Load the script.\n");
	int status = luaL_loadfile(L, "script.lua");

	// call script.
	printf("Call the script.\n");
	if (!status)
		status = lua_pcall(L, 0, LUA_MULTRET, 0);

	// show error, if any
	if (status)
	{
		const char * strError = lua_tostring(L, -1);
		printf("error: %s\n", strError);
		SaveErrorToFile("error.txt", strError);
		lua_pop(L, 1);  // remove error message
	}

	// Finish the GU and Sync.
	g2dCleanFinish();

	// Shutdowns the graphics library.
	g2dTerm();

	// Shutdowns the Serial IO library.
	pspsio_term();

	// Shutdowns the irda library.
	pspirda_term();

	// cleanup
	printf("Cleanup Lua and the script.\n");
	lua_close(L);

	// Exit the thread.
	sceKernelExitGame();
	return 0;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// print the Lua_error to a text file.
int SaveErrorToFile(const char * strPath, const char * strError)
{
	FILE * fp = fopen(strPath, "wb");
	if (fp == NULL) return -1;
	fwrite(strError, sizeof(char), strlen(strError), fp);
	fclose(fp);
	return 0;
}
// -----------------------------------------------------------------------------------------------------
