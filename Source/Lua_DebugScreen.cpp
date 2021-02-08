// Lua DebugScreen ( FileName: Lua_DebugScreen.cpp )
// ------------------------------------------------------------------------
// Version: 1.00
// Copyright (c) 2012 M4MSOFT
// ------------------------------------------------------------------------

// Include psp Header files.
#include <pspdebug.h>

extern "C" {
#include <string.h>
}

// Include the Lua header files for C++.
#include "luaplayer.hpp"

// Include the Lua module header files for C++.
#include "Lua_DebugScreen.hpp"
#include "Lua_Color.hpp"

// -----------------------------------------------------------------------------------------------------
// Define the function prototypes
static int lua_init (lua_State * L);
static int lua_backColor (lua_State *L);
static int lua_textColor (lua_State *L);
static int lua_position (lua_State *L);
static int lua_printf (lua_State * L);
static int lua_print (lua_State * L);
static int lua_clear (lua_State *L);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Initialise the debug screen.
static int lua_init(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "Argument error: DebugScreen.init() takes no arguments.");

	// Initialise the debug screen.
	pspDebugScreenInit();

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_backColor(lua_State *L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1)  return luaL_error(L, "Argument error: DebugScreen.backColor(color): takes one arguments");

	// Set the background color for the text.
	pspDebugScreenSetBackColor (*luaL_checkColor(L, 1));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_textColor(lua_State *L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1)  return luaL_error(L, "Argument error: DebugScreen.textColor(color): takes one arguments");

	// Set the text color.
	pspDebugScreenSetTextColor(*luaL_checkColor(L, 1));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_position(lua_State *L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 0  && narg != 2)  return luaL_error(L, "Argument error: DebugScreen.position([x, y]): takes zero or two arguments");

	// Set the current X and Y co-ordinate for the screen (in character units).
	if (narg == 2)
		pspDebugScreenSetXY(luaL_checkint(L, 1), luaL_checkint(L, 2));

	// return the current X and Y co-ordinate (in character units).
	lua_pushinteger(L, pspDebugScreenGetX());
	lua_pushinteger(L, pspDebugScreenGetY());

	return 2; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// print all arguments, converted to strings
static int lua_printf(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);

	// Print the text to the debugScreen.
	for (int n = 1; n <= narg; ++n)
		pspDebugScreenPrintf("%s\n", lua_tostring(L, n));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_print(lua_State *L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 3 && narg != 4)  return luaL_error(L, "Argument error: DebugScreen.print(x,y, text [,color]): takes three or four arguments");

	// Print the text to the screen.
	int posx = luaL_checkint(L, 1);
	int posy = luaL_checkint(L, 2);
	const char *s = luaL_checkstring(L, 3);
	int l = strlen(s);
	g2dColor color = (narg == 4 ? *luaL_checkColor(L, 4) : 0xffffffff);
	for (int i = 0; i < l; i++)
		pspDebugScreenPutChar(posx+8*i, posy, color, *(s+i));

	return 0;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_clear(lua_State *L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg)  return luaL_error(L, "Argument error: DebugScreen.clear(): takes no arguments");

	// Clear the debug screen.
	pspDebugScreenClear();

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Define the module table.
static const luaL_Reg mod_lib[] =
{
	{"init", lua_init},
	{"backColor", lua_backColor},
	{"textColor", lua_textColor},
	{"position", lua_position},
	{"printf", lua_printf},
	{"print", lua_print},
	{"clear", lua_clear},
	{NULL, NULL}
};
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Open the PSP lua DebugScreen libraray.
LUAMOD_API int luaopen_DebugScreen(lua_State * L)
{
	// Create the module.
	luaL_newlib(L, mod_lib);

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
