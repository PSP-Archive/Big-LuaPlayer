// Psp adhoc library ( FileName: adhoc.c )
// ------------------------------------------------------------------------
// Version: 1.00
// Copyright (c) 2012 M4MSOFT
// ------------------------------------------------------------------------

// Include psp Header files.
#include <pspsdk.h>
#include <pspthreadman.h>
#include <pspnet.h>
#include <pspwlan.h>
#include <pspnet_adhoc.h>
#include <pspnet_adhocctl.h>

// Include C header files.
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

// Include the adhoc Header files.
#include "adhoc.h"

// -----------------------------------------------------------------------------------------------------
// Defines.
#define ADHOC_SCANBUFFERSIZE   (0x8)
#define ADHOC_PDPBUFFERSIZE    (0x10)
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Private variables.
static bool init = false;

static unsigned char mac[8] =           {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
static unsigned char broadcast_mac[8] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};

static int bufScanInfoLen = 0;
static struct adhoc_scaninfo_ bufScanInfo[ADHOC_SCANBUFFERSIZE];
static void *bufScanInfoPointer = NULL;

static int pdpID = -1;
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Define the function prototypes.
static void _adhocResetScanInfo();
static void _adhocSetScanInfo(int index, const char *name, unsigned char *bssid);
static adhoc_scaninfo _adhocGetChannelByName(const char *name);
//static adhoc_scaninfo _adhocGetChannelByNumber(int channel);

static int _adhocSetupPdp();
static void _adhocFreePdp();
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Reset the adhoc scan info.
void _adhocResetScanInfo()
{
	bufScanInfoLen = 0;
	int i;
	for (i = 0; i < ADHOC_SCANBUFFERSIZE; i++)
	{
		bufScanInfo[i].next = (i+1 < ADHOC_SCANBUFFERSIZE ? bufScanInfo+i+1 : NULL);
		bufScanInfo[i].channel = i;
		memset(bufScanInfo[i].name, 0, 8);
		memset(bufScanInfo[i].bssid, 0, 6);
		memset(bufScanInfo[i].unknown, 0, 2);
		bufScanInfo[i].unknown2 = 0;
	}
	if (bufScanInfoPointer)
	{
		free(bufScanInfoPointer);
		bufScanInfoPointer = NULL;
	}
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Set the adhoc scan info.
static void _adhocSetScanInfo(int index, const char *name, unsigned char *bssid)
{
	int i, len = strlen(name);
	for (i = 0; i < 7 && i < len; i++)
		bufScanInfo[index].name[i] = name[i];
	for (i = 0; i < 6; i++)
		bufScanInfo[index].bssid[i] = bssid[i];
	for (i = 0; i < 2; i++)
		bufScanInfo[index].bssid[i] = bssid[6+i];
	bufScanInfo[index].unknown2 = (int)bssid;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Get the pointer of a channel by name.
static adhoc_scaninfo _adhocGetChannelByName(const char *name)
{
	if (!bufScanInfoLen)
		return NULL;
	adhoc_scaninfo scaninfo = bufScanInfo;
	int i;
	for (i = 0; (i < bufScanInfoLen) && scaninfo; i += 8, scaninfo = scaninfo->next)
	{
		if (!stricmp(scaninfo->name, name))
			return scaninfo;
	}
	return bufScanInfo;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Get the pointer of a channel by number.
//static adhoc_scaninfo _adhocGetChannelByNumber(int channel)
//{
//	if (!bufScanInfoLen)
//		return NULL;
//  adhoc_scaninfo scaninfo = bufScanInfo;
//	int i;
//	for (i = 0; (i < bufScanInfoLen) && scaninfo; i += 8, scaninfo = scaninfo->next)
//	{
//		if (scaninfo->channel == channel)
//			return scaninfo;
//	}
//	return NULL;
//}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Initializes the adhoc library.
int adhocInit()
{
	if (init)
		return 0;

	// Set the product information to connect to the CTL
	struct productStruct product;
	strcpy(product.product, "ULUS99999");
	product.unknown = 0;

	// Init the net module for WLAN
	if (sceNetInit(0x20000, 0x20, 0x1000, 0x20, 0x1000))
	{
		return -1;
	}

	// Init the adhoc libraries
	if (sceNetAdhocInit())
	{
		sceNetTerm();
		return -2;
	}

	// Init the adhoc ctl libraries
	if (sceNetAdhocctlInit(0x2000, 0x30, &product))
	{
		sceNetAdhocTerm();
		sceNetTerm();
		return -3;
	}

	// Get the MAC address
	if (sceWlanGetEtherAddr(mac))
	{
		sceNetAdhocctlTerm();
		sceNetAdhocTerm();
		sceNetTerm();
		return -4;
	}

	pdpID = -1;
	init = true;
	return 0;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Shutdowns the adhoc library.
void adhocTerm()
{
	if (!init)
		return;

	// Free the PDP.
	_adhocFreePdp();

	sceNetAdhocctlTerm();
	sceNetAdhocTerm();
	sceNetTerm();

	// Reset the adhoc scan info.
	_adhocResetScanInfo();

	init = false;
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Scan the adhoc channels.
int adhocGetScan(adhoc_scaninfo *scaninfo, int *len, int type, int timeout)
{
	if (scaninfo == NULL || len == NULL)
		return -1;

	// Get the adhoc state.
	if (adhocGetState() != 0 && adhocGetState() != 2)
		return -2;

	// Reset the adhoc scan info.
	_adhocResetScanInfo();
	*scaninfo = NULL;
	*len = 0;

	if (type & ADHOC_SCAN)
	{
		// Scan the adhoc channels.
		if (sceNetAdhocctlScan())
			return -3;

		// Get the adhoc state.
		if (adhocGetState() != 2)
			sceKernelDelayThread(50*1000); // 50ms
	}

	if (timeout >= 0)
	{
		int state = adhocGetState();
		timeout--;
		while (state != -1 && state != 0 && timeout)
		{
			if (timeout > 0) timeout--;
			sceKernelDelayThread(50*1000); // 50ms
			state = adhocGetState();
		}
		if (state < 0)
			return -5;
	}

	// Get the adhoc state.
	if (adhocGetState() == 0)
	{
		// Get the scan result.
		if (sceNetAdhocctlGetScanInfo(&bufScanInfoLen, NULL))
		{
			_adhocResetScanInfo();
			return -6;
		}
		// Check the result.
		if (bufScanInfoLen > 0)
		{
			bufScanInfoPointer = malloc(bufScanInfoLen);
			if (bufScanInfoPointer == NULL)
			{
				_adhocResetScanInfo();
				return -7;
			}
			if (sceNetAdhocctlGetScanInfo(&bufScanInfoLen, bufScanInfoPointer))
			{
				_adhocResetScanInfo();
				return -8;
			}
			// -----------------------------------------------------------------------------------------------
			// Fill the scan structure.
			// FIXME: Get the SceNetAdhocctlScanInfo structure from the sceNetAdhocctlGetScanInfo() function.
			adhoc_scaninfo scaninfo = bufScanInfo;
			char str[8] = "ADHOCx0";
			int i;
			for (i = 0; (i < bufScanInfoLen) && scaninfo; i += 8, scaninfo = scaninfo->next)
			{
				str[6] = '0'+i;
				_adhocSetScanInfo(i>>3, str, ((unsigned char*)bufScanInfoPointer)+i);
			}
			// -----------------------------------------------------------------------------------------------
		}
		else
		{
			_adhocResetScanInfo();
			return -9;
		}
	}
	else
		return -10;

	// return the result.
	*scaninfo = bufScanInfo;
	*len = bufScanInfoLen;

	return 0;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Create a adhoc connection.
int adhocConnect(int type, const char *name, int timeout)
{
	if (!name)
		return -1;
	if (sceWlanGetSwitchState() != 1)
		return -2;
	if (adhocGetState() == 1)
		return 0;

	int ret= -1;

	switch (type)
	{
		case ADHOC_CONNECT: // Connect to adhoc control
			ret = sceNetAdhocctlConnect(name);
			break;
		case ADHOC_CREATE: // Connect to adhoc control as a host.
			ret = sceNetAdhocctlCreate(name);
			break;
		case ADHOC_JOIN: // Connect to adhoc control as a client.
			// Get the scaninfo pointer by name.
			{
				adhoc_scaninfo scaninfo = NULL;
				if ((scaninfo = _adhocGetChannelByName(name)) == NULL)
					return -4;
				void *buf = (void*)(scaninfo->unknown2);
				if (buf)
					ret = sceNetAdhocctlJoin(buf);
			}
			break;
		default:
			return -3;
	}

	if (timeout >= 0 && ret == 0)
	{
		int state = adhocGetState();
		timeout--;
		while (state != -1 && state != 1 && timeout)
		{
			if (timeout > 0) timeout--;
			sceKernelDelayThread(50*1000); // 50ms
			state = adhocGetState();
		}
		if (state < 0)
			return -5;
	}

	return (ret == 0 ? 0 : -4);
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Get the connection state.
int adhocGetState()
{
	int state;
	if (sceNetAdhocctlGetState(&state) < 0)
		return -1;
	else
		return state;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Disconnect a adhoc connection.
int adhocDisconnect(int timeout)
{
	// Get the adhoc state.
	if (adhocGetState() != 1)
		return -1;

	// Free the PDP.
	_adhocFreePdp();

	// Disconnect a adhoc connection.
    if (sceNetAdhocctlDisconnect() < 0);
		return -2;

	if (timeout >= 0)
	{
		int state = adhocGetState();
		timeout--;
		while (state != -1 && state != 0 && timeout)
		{
			if (timeout > 0) timeout--;
			sceKernelDelayThread(50*1000); // 50ms
			state = adhocGetState();
		}
		if (state < 0)
			return -3;
	}

	return 0;
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Setup a PDP.
static int _adhocSetupPdp()
{
	if (!init) return -1;
	if (adhocGetState() != 1) return  -2;
	if (pdpID != -1) return 0;
	// Create a PDP
	if ((pdpID = sceNetAdhocPdpCreate(mac, 0x309, 0x400, 0)) < 0)
		pdpID = -1;
	return (pdpID < 0 ? -3: 0);
}
// -----------------------------------------------------------------------------------------------------
// Free a PDP.
static void _adhocFreePdp()
{
	if (pdpID == -1) return;
	// Delete the PDP.
	if (!sceNetAdhocPdpDelete(pdpID, 0))
		pdpID = -1;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Set a PDP packet to a destination.
int adhocSend(unsigned char *destMac, void *data, unsigned int len, unsigned int timeout)
{
	if (_adhocSetupPdp()) return -1;
	if (destMac == NULL) destMac = broadcast_mac;
	int ret = sceNetAdhocPdpSend(pdpID, destMac, 0x309, data, len, timeout, 0);
	return (ret < 0 ? -1 : ret);
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
//  Receive a PDP packet.
int adhocRecv(unsigned char *srcMac, void *data, unsigned int *len, unsigned int timeout)
{
	if (_adhocSetupPdp()){*len = 0; return -1;}
	unsigned short port = 0;
	int ret = sceNetAdhocPdpRecv(pdpID, srcMac, &port, data, len, timeout, 0);
	return (ret < 0 ? -1 : ret);
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Get the Ethernet Address of the wlan controller.
int adhocGetMac(unsigned char *etherAddr)
{
	if (etherAddr == NULL)
		return -1;
	return sceWlanGetEtherAddr(etherAddr);
}
// -----------------------------------------------------------------------------------------------------
