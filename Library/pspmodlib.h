// Psp Module Loader ( FileName: pspmodlib.h )
// ------------------------------------------------------------------------
// Version: 1.00
// Copyright (c) 2012 M4MSOFT
// ------------------------------------------------------------------------

#ifndef __pspmodlib_h
#define __pspmodlib_h

#ifdef __cplusplus
extern "C" {
#endif

// -----------------------------------------------------------------------------------------------------
// error codes
#define MODLIB_ERR_NONE         0  // no error
#define MODLIB_ERR_CANTLOADMOD -1  // problem loading the module
#define MODLIB_ERR_CANTRUNMOD  -2  // problem running the module
#define MODLIB_ERR_CANTSTOPMOD -3  // problem Stopping the module
#define MODLIB_ERR_CANTFREEMOD -4  // problem unloading the module
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// load and start a module.
//
// @param: path - the location of the module.
// @param: name - the module name.
// @param: ext - the file extions.
//
// @return: ind - The module ID or a Error code.
//
int pspmodStartModule (const char *path, const char *name, const char *ext);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// load and start a psp module.
//
// @param: name - the module name.
//
// @return: ind - The module ID or a Error code.
//
int pspmodStartPSPModule (const char *name);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// stop and unload a module.
//
// @param: modid - the module id.
//
// @return: int - the error code.
//
int pspmodStopModule (int modid);
// -----------------------------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif
