// Psp Irda library ( FileName: pspirdalib.h )
// ------------------------------------------------------------------------
// Version: 1.00
// Copyright (c) 2012 M4MSOFT
// ------------------------------------------------------------------------

#ifndef _pspirdalib_h
#define _pspirdalib_h

#ifdef __cplusplus
extern "C" {
#endif

// -----------------------------------------------------------------------------------------------------
// Initializes the irda library.
//
// @param: void.
//
// @return: int - 0 on succes.
//
int pspirda_init();
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Check if the library has been initalized.
//
// @param: void.
//
// @return: int - o = false, 1 = true.
//
int pspirda_initalized();
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Shutdowns the irda library.
//
// @param: void.
//
// @return: void.
//
void pspirda_term();
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// write data to a irda port.
//
// @param: data - Pointer to the data to write.
// @param: size - Size of data to write.
//
// @return: int - The number of bytes written.
//
int pspirda_write(const void *data, unsigned int size);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// read data from a irda port.
//
// @param: data - Pointer to the buffer where the read data will be placed.
// @param: size - Size of the read in bytes.
//
// @return: int - The number of bytes read.
//
int pspirda_read(void *data, unsigned int size);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Reposition read/write file descriptor offset.
//
// @param: offset - Relative offset from the start position given by whence
// @param: whence - Set to SEEK_SET to seek from the start of the file, SEEK_CUR seek from the current position and SEEK_END to seek from the end.
//
// @return: int - The position in the file after the seek.
//
signed long pspirda_seek(signed long offset, int whence);
// -----------------------------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif
