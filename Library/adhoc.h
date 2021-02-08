// Psp adhoc library ( FileName: adhoc.h )
// ------------------------------------------------------------------------
// Version: 1.00
// Copyright (c) 2012 M4MSOFT
// ------------------------------------------------------------------------

#ifndef PSP_ADHOC_H
#define PSP_ADHOC_H

#ifdef __cplusplus
extern "C" {
#endif

// -----------------------------------------------------------------------------------------------------
// Defines.
#define ADHOC_VOID    (0x0)
#define ADHOC_CONNECT (0x1)
#define ADHOC_CREATE  (0x2)
#define ADHOC_JOIN    (0x3)

#define ADHOC_SCAN (0x1)
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Structure, unions and pointers.

/** Scan info structure */
struct adhoc_scaninfo_
{
	struct adhoc_scaninfo_ *next;
	/** Channel number */
	int channel;
	/** Name of the connection (alphanumeric characters only) */
	char name[8];
	/** The BSSID */
	unsigned char bssid[6];
	/** Unknown */
	unsigned char unknown[2];
	/** Unknown */
	int unknown2;
};
typedef struct adhoc_scaninfo_* adhoc_scaninfo;

// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Initializes the adhoc library.
//
// @param: void.
//
// @return: int - 0 on succes.
//
int adhocInit();
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Shutdowns the adhoc library.
//
// @param: void.
//
// @return: void.
//
void adhocTerm();
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Scan the adhoc channels.
//
// @param: scaninfo - Pointer to store the scaninfo list.
// @param: len - Pointer to store the lenght of the list.
// @param: type - the scan type. (1 = enable scan)
// @param: timeout - the time the function may use in 50ms.
//
// @return: int - 0 on succes.
//
int adhocGetScan(adhoc_scaninfo *scaninfo, int *len, int type, int timeout);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Create a adhoc connection.
//
// @param: type - The connection type.
// @param: name - Name of the connection (alphanumeric characters only).
// @param: timeout - the time the function may use in 50ms.
//
// @return: int - 0 on succes.
//
int adhocConnect(int type, const char *name, int timeout);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Get the connection state.
//
// @param: void.
//
// @return: int - The current state.
//	-1 - error.
//	0  - ready.
//	1  - connected.
//	2  - scanning.
//
int adhocGetState();
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Disconnect a adhoc connection.
//
// @param: timeout - the time the function may use in 50ms.
//
// @return: int - 0 on succes.
//
int adhocDisconnect(int timeout);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Get the Ethernet Address of the wlan controller.
//
// @param: etherAddr - pointer to a buffer of u8.
//
// @return: int - 0 on succes.
//
int adhocGetMac(unsigned char *etherAddr);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Set a PDP packet to a destination.
//
// @param: destMac - The destination MAC address, can be set to all 0xFF for broadcast.
// @param: data - The data to send.
// @param: len - The length of the data.
// @param: timeout - Timeout in microseconds.
//
// @return: int - Bytes sent, < 0 on error.
//
int adhocSend(unsigned char *destMac, void *data, unsigned int len, unsigned int timeout);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
//  Receive a PDP packet.
//
// @param: srcMac - Buffer to hold the source mac address of the sender .
// @param: data - Data buffer.
// @param: len - The length of the data buffer.
// @param: timeout - Timeout in microseconds.
//
// @return: int - Number of bytes received, < 0 on error.
//
int adhocRecv(unsigned char *srcMac, void *data, unsigned int *len, unsigned int timeout);
// -----------------------------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif

