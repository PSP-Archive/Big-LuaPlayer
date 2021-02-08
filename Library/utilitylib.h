// Psp Utility library ( FileName: utilitylib.h )
// ------------------------------------------------------------------------
// Version: 1.00
// Copyright (c) 2012 M4MSOFT
// ------------------------------------------------------------------------

#ifndef __utilitylib_h
#define __utilitylib_h

#ifdef __cplusplus
extern "C" {
#endif

// -----------------------------------------------------------------------------------------------------
// Defines.
#define UTILITY_OSK_MAXLEN (128)
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Initialize the message dialog.
//
// @param: message - The message to show.
// @param: opts - The message options.
//	0   - error
//  1   - text
//	16  - Show Yes and No buttons.
//	256 - Set No as the Default button.
//
// @return: int - 0 on succes.
//
int utility_msgInit(const char *message, int opts);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Update the message dialog.
//
// @param: void.
//
// @return: int. the current status of the message dialog.
//	-1 - dialog running.
//	0  - dialog result: unknown 1
//	1  - dialog result: yes
//	2  - dialog result: no
//	3  - dialog result: canceled.
//
int utility_msgUpdate();
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Abort the message dialog.
//
// @param: void.
//
// @return: void.
//
void utility_msgAbort();
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Initialize the Osk dialog.
//
// @param: input - The input text.
// @param: desc - The description text.
// @param: type - The input type. (0x000fff0f)
//	0   - all             ,512   - japanese symbol        ,32768  - japanese kanji
//  1   - latin digit     ,1024  - japanese lowercase     ,65536  - russian lowercase
//	2   - latin symbol    ,2048  - japanese uppercase     ,131072 - russian uppercase
//	4   - latin lowercase ,4096  - japanese hiragane      ,262144 - korean
//	8   - latin uppercase ,8192  - japanese half katakana ,524288 - URL
//	256 - japanese digit  ,16384 - japanese katakana
// @param: language - The input language. (0x0000000a)
// 0 - default  ,5 - german     ,10 - korean
// 1 - japanese ,6 - italian
// 2 - english  ,7 - dutch
// 3 - french   ,8 - portugese
// 4 - spanish  ,9 - russian
//
// @return: void.
//
int utility_oskInit(const char *input, const char *desc, int type, int language);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Update the Osk dialog.
//
// @param: void.
//
// @return: int. the current status of the Osk dialog.
//	-1 - dialog running.
//	0  - dialog result: unchanged
//	1  - dialog result: cancelled
//	2  - dialog result: changed
//	3  - dialog result: unknown
//
int utility_oskUpdate();
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Abort the Osk dialog.
//
// @param: void.
//
// @return: void.
//
void utility_oskAbort();
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Get the output text of the osk dialog.
//
// @param: output - the output buffer.
// @param: size - the output buffer size.
//
// @return: void.
//
void utility_oskGetOutput(char *output, int size);
// -----------------------------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif
