// Psp Utility library ( FileName: utilitylib.c )
// ------------------------------------------------------------------------
// Version: 1.00
// Copyright (c) 2012 M4MSOFT
// ------------------------------------------------------------------------

// Include psp Header files.
#include <pspkernel.h>
#include <psputility.h>

// Include C header files.
//#include <stdio.h>
//#include <stdlib.h>
#include <string.h>
//#include <math.h>
#include <stdbool.h>

// Include the PSP Utilitylib Header files.
#include "utilitylib.h"

// -----------------------------------------------------------------------------------------------------
// Private variables.
static pspUtilityMsgDialogParams G_dialog;
static bool G_dialogInit = false;
static bool G_dialogBack = false;

static SceUtilityOskParams G_osk;
static	SceUtilityOskData G_oskData;
static unsigned short G_osInput[UTILITY_OSK_MAXLEN];
static unsigned short G_osOutput[UTILITY_OSK_MAXLEN];
static unsigned short G_osDesc[UTILITY_OSK_MAXLEN];
static bool G_oskInit = false;
static bool G_oskBack = false;
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Define the function prototypes.
static void _configureDialog(pspUtilityMsgDialogParams *dialog, size_t dialog_size);
static void _configureOsk(SceUtilityOskParams *dialog, size_t dialog_size);
static int _CreateDialog(pspUtilityMsgDialogParams *dialog, const char* message, int mode, int opts);
static void _setcharshort(const char *in, unsigned short *out);
static bool _equalcharshort(unsigned short *a, unsigned short *b);
static int _createOsk(SceUtilityOskParams *dialog, const char *input, const char *desc, int type, int language);
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// configure the message dialog.
static void _configureDialog(pspUtilityMsgDialogParams *dialog, size_t dialog_size)
{
    memset(dialog, 0, dialog_size);

    dialog->base.size = dialog_size;
    sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_LANGUAGE,
								&dialog->base.language); // Prompt language
    sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_UNKNOWN,
								&dialog->base.buttonSwap); // X/O button swap

    dialog->base.graphicsThread = 0x11;
    dialog->base.accessThread = 0x13;
    dialog->base.fontThread = 0x12;
    dialog->base.soundThread = 0x10;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// configure the osk dialog.
static void _configureOsk(SceUtilityOskParams *dialog, size_t dialog_size)
{
    memset(dialog, 0, dialog_size);

    dialog->base.size = dialog_size;
    sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_LANGUAGE,
								&dialog->base.language); // Prompt language
    sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_UNKNOWN,
								&dialog->base.buttonSwap); // X/O button swap

    dialog->base.graphicsThread = 0x11;
    dialog->base.accessThread = 0x13;
    dialog->base.fontThread = 0x12;
    dialog->base.soundThread = 0x10;
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// messageDialog Functions:
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Create a message dialog.
int _CreateDialog(pspUtilityMsgDialogParams *dialog, const char* message, int mode, int opts)
{
	_configureDialog(dialog, sizeof(pspUtilityMsgDialogParams));
    dialog->mode = mode;
	dialog->options = opts;

	if (strlen(message) > 512)
		return -1;

    strcpy(dialog->message, message);
    return sceUtilityMsgDialogInitStart(dialog);
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Initialize the message dialog.
int utility_msgInit(const char *message, int opts)
{
	if (G_dialogInit)
		return -1;

	if (_CreateDialog(&G_dialog, message, PSP_UTILITY_MSGDIALOG_MODE_TEXT, opts))
		return -2;

	G_dialogInit = true;
	G_dialogBack = false;

	return 0;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Update the message dialog.
int utility_msgUpdate()
{
	if (!G_dialogInit)
		return 0;

	switch(sceUtilityMsgDialogGetStatus())
	{
		case PSP_UTILITY_DIALOG_INIT:  // The dialog is currently being initialized.
			return -1;
		case PSP_UTILITY_DIALOG_VISIBLE:  // The dialog is visible and ready for use.
			sceUtilityMsgDialogUpdate(1); return -1;
		case PSP_UTILITY_DIALOG_QUIT:  // The dialog has been canceled and should be shut down.
			sceUtilityMsgDialogShutdownStart(); G_dialogBack = true; return -1;
		case PSP_UTILITY_DIALOG_FINISHED:  // The dialog has successfully shut down.
			G_dialogInit = false; G_dialogBack = false; return G_dialog.buttonPressed;
		case PSP_UTILITY_DIALOG_NONE:  // No dialog is currently active.
			G_dialogInit = false; G_dialogBack = false; return 0;
		default:
			utility_msgAbort(); return 0;
	}
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Abort the message dialog.
void utility_msgAbort()
{
	if (!G_dialogInit)
		return;

	if (!G_dialogBack)
	{
		sceUtilityMsgDialogAbort();
		//sceUtilityMsgDialogShutdownStart();
		//G_dialogBack = true;
	}

	int maxTime = 1024;
	while (G_dialogInit && maxTime-- > 0)
	{
		switch(sceUtilityMsgDialogGetStatus())
		{
			case PSP_UTILITY_DIALOG_VISIBLE:  // The dialog is visible and ready for use.
				sceUtilityMsgDialogUpdate(1); break;
			case PSP_UTILITY_DIALOG_QUIT:  // The dialog has been canceled and should be shut down.
				sceUtilityMsgDialogShutdownStart(); G_dialogBack = true; break;
			case PSP_UTILITY_DIALOG_INIT:  // The dialog is currently being initialized.
			case PSP_UTILITY_DIALOG_FINISHED:  // The dialog has successfully shut down.
			case PSP_UTILITY_DIALOG_NONE:  // No dialog is currently active.
			default:
				G_dialogInit = false;
				G_dialogBack = false;
				break;
		}
	}
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// messageDialog Functions:
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Convert a const char pointer to a unsigned short pointer.
static void _setcharshort(const char *in, unsigned short *out)
{
	int i, len; for (i = 0, len = strlen(in); i < len && i < UTILITY_OSK_MAXLEN-1; i++) out[i] = (unsigned short) in[i];
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Convert a const char pointer to a unsigned short pointer.
static bool _equalcharshort(unsigned short *a, unsigned short *b)
{
	int i;
	for (i = 0; i < UTILITY_OSK_MAXLEN; i++)
		if (a[i] != b[i])
			return false;
	return true;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Create a Osk dialog.
static int _createOsk(SceUtilityOskParams *dialog, const char *input, const char *desc, int type, int language)
{
	_configureOsk(dialog, sizeof(SceUtilityOskParams));

	// Reset the string pointers.
	memset(G_osInput, 0, UTILITY_OSK_MAXLEN);
	memset(G_osOutput, 0, UTILITY_OSK_MAXLEN);
	memset(G_osDesc, 0, UTILITY_OSK_MAXLEN);

	// Set the string pointers.
	_setcharshort(input, G_osInput);
	_setcharshort(input, G_osOutput);
	_setcharshort(desc, G_osDesc);

	SceUtilityOskData *data = &G_oskData;
	memset(data, 0, sizeof(SceUtilityOskData));
	data->language = language;
	data->lines = 1;
	data->unk_24 = 1;
	data->inputtype = type;
	data->desc = G_osDesc;
	data->intext = G_osInput;
	data->outtextlength = UTILITY_OSK_MAXLEN;
	data->outtextlimit = UTILITY_OSK_MAXLEN;
	data->outtext = G_osOutput;

	dialog->datacount = 1;
	dialog->data = data;

	return sceUtilityOskInitStart(dialog);
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Initialize the Osk dialog.
int utility_oskInit(const char *input, const char *desc, int type, int language)
{
	if (G_oskInit)
		return -1;

	if (_createOsk(&G_osk, input, desc,
			PSP_UTILITY_OSK_INPUTTYPE_ALL | type,
			PSP_UTILITY_OSK_LANGUAGE_DEFAULT | language))
		return -2;

	G_oskInit = true;
	G_oskBack = false;

	return 0;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Update the Osk dialog.
int utility_oskUpdate()
{
	if (!G_oskInit)
		return 3;

	switch(sceUtilityOskGetStatus())
	{
		case PSP_UTILITY_DIALOG_INIT:  // The dialog is currently being initialized.
			return -1;
		case PSP_UTILITY_DIALOG_VISIBLE:  // The dialog is visible and ready for use.
			sceUtilityOskUpdate(1); return -1;
		case PSP_UTILITY_DIALOG_QUIT:  // The dialog has been canceled and should be shut down.
			sceUtilityOskShutdownStart(); G_oskBack = true; return -1;
		case PSP_UTILITY_DIALOG_FINISHED:  // The dialog has successfully shut down.
			G_oskInit = false; G_oskBack = false;
			return (G_oskData.result == 2 ? (_equalcharshort(G_osInput, G_osOutput) ? 0 : 2) : G_oskData.result);
		case PSP_UTILITY_DIALOG_NONE:  // No dialog is currently active.
		default:
			G_oskInit = false; G_oskBack = false; return 3;
	}
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Abort the Osk dialog.
void utility_oskAbort()
{
	if (!G_oskInit)
		return;

	if (!G_oskBack)
	{
		sceUtilityOskShutdownStart();
		G_oskBack = true;
	}

	int maxTime = 1024;
	while (G_oskInit && maxTime-- > 0)
	{
		switch(sceUtilityOskGetStatus())
		{
			case PSP_UTILITY_DIALOG_VISIBLE:  // The dialog is visible and ready for use.
				sceUtilityOskUpdate(1); break;
			case PSP_UTILITY_DIALOG_QUIT:  // The dialog has been canceled and should be shut down.
				sceUtilityOskShutdownStart(); G_oskBack = true; break;
			case PSP_UTILITY_DIALOG_INIT:  // The dialog is currently being initialized.
			case PSP_UTILITY_DIALOG_FINISHED:  // The dialog has successfully shut down.
			case PSP_UTILITY_DIALOG_NONE:  // No dialog is currently active.
			default:
				G_oskInit = false;
				G_oskBack = false;
				break;
		}
	}
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Get the output text of the osk dialog.
void utility_oskGetOutput(char *output, int size)
{
	if (!output || !size) return;
	int i, len;
	for (i = 0, len = G_oskData.outtextlength; i < len && i < size; i++)
		output[i] = (char) G_osOutput[i];
	G_osOutput[size - 1] = 0;
}
// -----------------------------------------------------------------------------------------------------
