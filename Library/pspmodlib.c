// Psp Module Loader ( FileName: pspmodlib.c )
// ------------------------------------------------------------------------
// Version: 1.00
// Copyright (c) 2012 M4MSOFT
// ------------------------------------------------------------------------

// Include psp Header files.
#include <pspkernel.h>

// Include C header files.
//#include <stdio.h>
//#include <stdlib.h>
#include <string.h>
//#include <math.h>

// Include the PSP modlib Header files.
#include "pspmodlib.h"

// -----------------------------------------------------------------------------------------------------
// Defines.
#define MODLIB_PATH "flash0:/kd"
#define MODLIB_EXT  ".prx"
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
int pspmodStartModule(const char *path, const char *name, const char *ext)
{
    char fullPath[256] = {};
	int loadResult;
    int startResult;
    int status;

	// check if the module is already loaded.
	SceModule *pMod;
	pMod = sceKernelFindModuleByName(name);
	if (pMod) return pMod->modid;

	// Set the mudule load path.
	strcpy(fullPath, MODLIB_PATH); strcat(fullPath, "/"); strcat(fullPath, name); strcat(fullPath, ext);

    loadResult = sceKernelLoadModule(fullPath, 0, NULL);
	if (loadResult & 0x80000000)
		return MODLIB_ERR_CANTLOADMOD;

    startResult = sceKernelStartModule(loadResult, 0, NULL, &status, NULL);
	if (loadResult != startResult)
	{
		sceKernelStopModule(loadResult, 0, NULL, &status, NULL);
		return MODLIB_ERR_CANTRUNMOD;
	}

    return loadResult;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
int pspmodStartPSPModule(const char *name)
{
	return pspmodStartModule(MODLIB_PATH, name, MODLIB_EXT);
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
int pspmodStopModule(int modID)
{
    int status;
	sceKernelStopModule(modID, 0, NULL, &status, NULL);
	sceKernelUnloadModule(modID);
	//if (sceKernelStopModule(modID, 0, NULL, &status, NULL) & 0x80000000)
		//return MODLIB_ERR_CANTSTOPMOD;
	//if (sceKernelUnloadModule(modID) & 0x80000000)
		//return MODLIB_ERR_CANTFREEMOD;
    return MODLIB_ERR_NONE;
}
// -----------------------------------------------------------------------------------------------------
