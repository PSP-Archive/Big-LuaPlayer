// PSP Lua utility ( FileName: Lua_Utility.cpp )
// ------------------------------------------------------------------------
// Version: 1.00
// Copyright (c) 2012 M4MSOFT
// ------------------------------------------------------------------------

// Include C/C++ header files
#include <string.h>
#include <stdio.h>

// Include commen psp Header file.
#include "Common/callbacks.h"

// Include the PSP glib2D Header file.
#include "glib2d.h"

// Include the PSP Utilitylib Header files.
#include "utilitylib.h"

// Include the adhoc Header files.
#include "adhoc.h"

// Include the Lua header files for C++.
#include "luaplayer.hpp"
#include "Lua_Utility.hpp"

// Select the luaplayer objects to use in this file.
using luaplayer::luaL_newusermatatable;
using luaplayer::clamp;

// -----------------------------------------------------------------------------------------------------
// Define the function prototypes
static int lua_Utility_MsgInit (lua_State * L);
static int lua_Utility_MsgUpdate (lua_State * L);
static int lua_Utility_MsgAbort (lua_State * L);
static int lua_Utility_MsgShow (lua_State * L);

static int lua_Utility_OskInit (lua_State * L);
static int lua_Utility_OskUpdate (lua_State * L);
static int lua_Utility_OskAbort (lua_State * L);
static int lua_Utility_OskShow (lua_State * L);

static int lua_Utility_AdhocInit (lua_State *L);
static int lua_Utility_AdhocTerm (lua_State *L);
static int lua_Utility_AdhocScan (lua_State* L);
static int lua_Utility_AdhocConnect (lua_State* L);
static int lua_Utility_AdhocGetState (lua_State* L);
static int lua_Utility_AdhocDisconnect (lua_State* L);
static int lua_Utility_AdhocSend (lua_State *L);
static int lua_Utility_AdhocRecv (lua_State *L);
static int lua_Utility_AdhocGetMac (lua_State* L);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Initialize the message dialog.
static int lua_Utility_MsgInit (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1 && narg != 2) return luaL_error(L, "Argument error: utility.msgInit(text [,options]) takes one or two arguments.");

	// Initialize the message dialog and return the result.
	lua_pushboolean(L, !utility_msgInit(luaL_checkstring(L, 1), (narg == 2 ? luaL_checkunsigned(L, 2) & 0x110 : 0)));

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Update the message dialog.
static int lua_Utility_MsgUpdate (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "Argument error: utility.msgUpdate() takes no arguments.");

	// Return the current message dialog state.
	lua_pushinteger(L, utility_msgUpdate());

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Abort the message dialog.
static int lua_Utility_MsgAbort (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "Argument error: utility.msgAbort() takes no arguments.");

	// Finish the GU and Sync.
	g2dCleanFinish();

	// Abort the currently active message dialog.
	utility_msgAbort();

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Show the message dialog.
static int lua_Utility_MsgShow (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg < 1 || narg > 3) return luaL_error(L, "Argument error: utility.msgShow(text [,options [,drawFunc]]) takes one to three arguments.");

	// Initialize the message dialog
	int dlgState = 0;
	int blnDrawFunc = ( narg == 3 ? true : false);
	if (blnDrawFunc) luaL_checktype(L, 3, LUA_TFUNCTION);
	if (!utility_msgInit(luaL_checkstring(L, 1), (narg > 1 ? luaL_checkunsigned(L, 2) & 0x110 : 0)))
	{
		dlgState = -1;
		while (dlgState < 0 && running())
		{
			// Use the drawFunc to draw the screen or clear the screen black.
			if (blnDrawFunc)
			{
				lua_pushvalue(L, 3); // push the draw function on to the stack.
				if (lua_pcall(L, 0, LUA_MULTRET, 0))
				{
					// Finish the GU and Sync.
					g2dCleanFinish();
					// Abort the message dialog.
					utility_msgAbort();
					// Return the error message.
					return lua_error(L);
				}
			}
			else
				// Clear the screen.
				g2dClear(BLACK);
			// Finish the GU and Sync.
			g2dFinish();
			// Update the message dialog.
			dlgState = utility_msgUpdate();
			// Swap buffers
			g2dFlip(G2D_VSYNC);
		}
		// Abort the message dialog if the dialog is stil active.
		if (dlgState < 0)
			utility_msgAbort();
	}
	// Return the dialog result.
	lua_pushboolean(L, dlgState == 1);

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Initialize the osk dialog.
static int lua_Utility_OskInit (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg < 1 || narg > 3) return luaL_error(L, "Argument error: utility.oskInit(desc [,init [,type]]) takes one to three arguments.");

	// Initialize the osk dialog and return the result.
	lua_pushboolean(L,
		!utility_oskInit(
			(narg > 1 ? luaL_checkstring(L, 2) : ""), luaL_checkstring(L, 1),
			(narg > 2 ? luaL_checkunsigned(L, 3) & 0xfff0f : 0), 0)
	);

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Update the osk dialog.
static int lua_Utility_OskUpdate (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "Argument error: utility.oskUpdate() takes no arguments.");

	// Get the current osk dialog state.
	int dlgState = utility_oskUpdate();

	// Return the current osk dialog state.
	lua_pushinteger(L, dlgState);

	// Return the osk dialog state output text.
	if (dlgState >= 0)
		{char output[UTILITY_OSK_MAXLEN]; utility_oskGetOutput(output, UTILITY_OSK_MAXLEN); lua_pushstring(L, output);}
	else
		lua_pushstring(L, "");

	return 2; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Abort the osk dialog.
static int lua_Utility_OskAbort (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "Argument error: utility.oskAbort() takes no arguments.");

	// Finish the GU and Sync.
	g2dCleanFinish();

	// Abort the currently active osk dialog.
	utility_oskAbort();

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Show the osk dialog.
static int lua_Utility_OskShow (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg < 1 || narg > 4) return luaL_error(L, "Argument error: utility.oskShow(desc [,init [,type [,drawFunc]]]) takes one to four arguments.");

	// Initialize the osk dialog
	int dlgState = 0;
	int blnDrawFunc = ( narg == 4 ? true : false);
	if (blnDrawFunc) luaL_checktype(L, 4, LUA_TFUNCTION);
	if (!utility_oskInit((narg > 1 ? luaL_checkstring(L, 2) : ""), luaL_checkstring(L, 1), (narg > 2 ? luaL_checkunsigned(L, 3) & 0xfff0f : 0), 0))
	{
		dlgState = -1;
		while (dlgState < 0 && running())
		{
			// Use the drawFunc to draw the screen or clear the screen black.
			if (blnDrawFunc)
			{
				lua_pushvalue(L, 4); // push the draw function on to the stack.
				if (lua_pcall(L, 0, LUA_MULTRET, 0))
				{
					// Finish the GU and Sync.
					g2dCleanFinish();
					// Abort the osk dialog.
					utility_oskAbort();
					// Return the error message.
					return lua_error(L);
				}
			}
			else
				// Clear the screen.
				g2dClear(BLACK);
			// Finish the GU and Sync.
			g2dFinish();
			// Update the osk dialog.
			dlgState = utility_oskUpdate();
			// Swap buffers
			g2dFlip(G2D_VSYNC);
		}
		// Abort the osk dialog if the dialog is stil active.
		if (dlgState < 0)
			utility_oskAbort();
	}
	char output[UTILITY_OSK_MAXLEN]; utility_oskGetOutput(output, UTILITY_OSK_MAXLEN);
	// Return the dialog result and text Values.
	lua_pushinteger(L, dlgState);
	lua_pushstring(L, output);

	return 2; // Number of results.
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
static int lua_Utility_AdhocInit(lua_State *L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "Argument error: utility.adhocInit() takes no arguments.");

	// Initialise the adhoc library and return the result.
	lua_pushboolean(L, adhocInit() == 0);

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_Utility_AdhocTerm(lua_State *L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "Argument error: utility.adhocTerm() takes no arguments.");

	// Terminate the adhoc library.
	adhocTerm();

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_Utility_AdhocScan(lua_State* L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg > 2) return luaL_error(L, "Argument error: utility.adhocScan([type [,timeout]]) takes zero to two arguments.");

	// Scan the adhoc channels.
	adhoc_scaninfo scaninfo = NULL;
	int len, err = adhocGetScan(&scaninfo, &len, (narg > 0 ? luaL_checkint(L, 1) & 0x1 : 0x1), (narg > 1 ? luaL_checkint(L, 2) : 0));

	// Return the result.
	if (err == 0 && len > 0)
	{
		lua_newtable(L);
		for (int i = 0; (i < len) && scaninfo; i += 8, scaninfo = scaninfo->next)
		{
			lua_pushnumber(L, i+1);
			lua_newtable(L);
			//lua_pushstring(L, "channel"); lua_pushinteger(L, scaninfo->channel); lua_settable(L, -3);
			lua_pushstring(L, "name"); lua_pushlstring(L, (const char*)scaninfo->name, 8); lua_settable(L, -3);
			lua_pushstring(L, "bssid"); lua_pushlstring(L, (const char*)scaninfo->bssid, 6); lua_settable(L, -3);
			lua_pushstring(L, "unknown"); lua_pushlstring(L, (const char*)scaninfo->unknown, 2); lua_settable(L, -3);
			lua_pushstring(L, "unknown2"); lua_pushinteger(L, scaninfo->unknown2); lua_settable(L, -3);
			lua_settable(L, -3);
		}
	}
	else
		// Return a nil value.
		lua_pushnil(L);

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_Utility_AdhocConnect(lua_State* L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 2 && narg != 3) return luaL_error(L, "Argument error: utility.adhocConnect(type, name [,timeout]) takes two or three arguments.");

	// Get the arguments.
	size_t size;
	const char* name = luaL_checklstring(L, 2, &size);

	// Check the arguments.
	if (size > 8)
		return luaL_error(L, "Invalid conection name string size. (range: 0-8)");

	// Create a adhoc connection and return the result.
	lua_pushboolean(L, adhocConnect(luaL_checkint(L, 1), name, (narg > 2 ? luaL_checkint(L, 3) : 0)) == 0);

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_Utility_AdhocGetState(lua_State* L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "Argument error: utility.adhocGetState() takes no arguments.");

	// Return the adhoc state.
	lua_pushnumber(L, adhocGetState());

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_Utility_AdhocDisconnect(lua_State* L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg > 1) return luaL_error(L, "Argument error: utility.adhocDisconnect([timeout]) takes zero or one argument.");

	// Disconnect a adhoc connection and return the result.
	lua_pushnumber(L, adhocDisconnect(narg > 0 ? luaL_checkint(L, 1) : 0) == 0);

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_Utility_AdhocSend(lua_State *L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg < 1 || narg > 3) return luaL_error(L, "Argument error: utility.adhocSend(data, [timeout [,mac]]) takes one to three arguments.");

	// Get the destiny mac address.
	size_t size = 0;
	const char *etherAddr = (narg > 2 ? luaL_checklstring(L, 3, &size) : NULL);

	// Check destiny mac address.
	if (etherAddr && size != 8)
		return luaL_error(L, "Invalid mac address.");

	// Get the timeout value.
	unsigned int t = ( narg > 1 ? luaL_checkunsigned(L, 2) : 0);

	// Sed the data and return the result.
	int state = 0;
	if (lua_type(L, 1) == LUA_TNUMBER)
	{
		/* optimization: could be done exactly as for strings */
		char s[32] = "";
		sprintf(s, LUA_NUMBER_FMT, lua_tonumber(L, 1));
		int l = strlen(s);
		state = (adhocSend((unsigned char*)etherAddr, (void*)s, l, t) > 0);
	}
	else
	{
		size_t l;
		const char *s = luaL_checklstring(L, 1, &l);
		state = (adhocSend((unsigned char*)etherAddr, (void*)s, l, t) > 0);
	}
	lua_pushboolean(L, state > 0);

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_Utility_AdhocRecv(lua_State *L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1 && narg != 2) return luaL_error(L, "Argument error: utility.adhocRecv(lenght [,timeout]) takes one or two arguments.");

	// Recive the data.
	unsigned int l = luaL_checkunsigned(L, 1);
	unsigned int t = ( narg == 2 ? luaL_checkunsigned(L, 2) : 0x1cbc6500); // 5000 ms
	unsigned char etherAddr[8] = {'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'};
	char *p;
	luaL_Buffer b;
	luaL_buffinit(L, &b);
	p = luaL_prepbuffsize(&b, l);  /* prepare buffer to read whole block */
	if (adhocRecv(etherAddr, p, &l, t) < 0)
		l = 0;
	luaL_addsize(&b, l);
	luaL_pushresult(&b);  /* close buffer */
	if (l == 0)
	{
		lua_pop(L, 1);  /* remove last result */
		lua_pushboolean(L, false);
	}
	lua_pushlstring (L, (const char*)etherAddr, 8);

	return 2; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_Utility_AdhocGetMac(lua_State* L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "Argument error: utility.adhocGetMac() takes no arguments.");

	// Get the Ethernet Address of the wlan controller.
	unsigned char etherAddr[8] = {'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'};
	adhocGetMac(etherAddr);

	// Return the mac Address.
	lua_pushlstring (L, (const char*)etherAddr, 8);

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Define the module table.
static const luaL_Reg mod_lib[] =
{
	{"msgInit", lua_Utility_MsgInit},
	{"msgUpdate", lua_Utility_MsgUpdate},
	{"msgAbort", lua_Utility_MsgAbort},
	{"msgShow", lua_Utility_MsgShow},

	{"oskInit", lua_Utility_OskInit},
	{"oskUpdate", lua_Utility_OskUpdate},
	{"oskAbort", lua_Utility_OskAbort},
	{"oskShow", lua_Utility_OskShow},

	{"adhocInit", lua_Utility_AdhocInit},
	{"adhocTerm", lua_Utility_AdhocTerm},
	{"adhocScan", lua_Utility_AdhocScan},
	{"adhocConnect", lua_Utility_AdhocConnect},
	{"adhocGetState", lua_Utility_AdhocGetState},
	{"adhocDisconnect", lua_Utility_AdhocDisconnect},
	{"adhocSend", lua_Utility_AdhocSend},
	{"adhocRecv", lua_Utility_AdhocRecv},
	{"adhocGetMac",lua_Utility_AdhocGetMac},

	{NULL, NULL}
};
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Open the PSP lua Utility libraray.
LUAMOD_API int luaopen_Utility(lua_State * L)
{
	// Create the module.
	luaL_newlib(L, mod_lib);

	// Define the Enum Values.
	SetFieldValue("DIALOG_RUNNING", -1)

	SetFieldValue("MSG_BTN_YESMO", 16)
	SetFieldValue("MSG_DEFAULT_BTN_NO", 256)

	SetFieldValue("MSG_RESULT_UNKNOW1", 0)
	SetFieldValue("MSG_RESULT_YES", 1)
	SetFieldValue("MSG_RESULT_NO", 2)
	SetFieldValue("MSG_RESULT_BACK", 3)


	SetFieldValue("OSK_TYPE_ALL", 0)
	SetFieldValue("OSK_TYPE_DIGIT", 1)
	SetFieldValue("OSK_TYPE_SYMBOL", 2)
	SetFieldValue("OSK_TYPE_LOWER", 3)
	SetFieldValue("OSK_TYPE_UPPER", 4)
	SetFieldValue("OSK_TYPE_URL", 524288)

	SetFieldValue("OSK_RESULT_UNCHANGED", 0)
	SetFieldValue("OSK_RESULT_CANCELLED", 1)
	SetFieldValue("OSK_RESULT_CHANGED", 2)

	SetFieldValue("ADHOC_CONNECT", ADHOC_CONNECT)
	SetFieldValue("ADHOC_CREATE", ADHOC_CREATE)
	SetFieldValue("ADHOC_JOIN", ADHOC_JOIN)

	SetFieldValue("ADHOC_STATE_READY", 1)
	SetFieldValue("ADHOC_STATE_SCANNING", 2)

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
