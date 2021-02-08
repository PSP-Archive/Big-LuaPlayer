// PSP Lua Color ( FileName: Lua_Color.cpp )
// ------------------------------------------------------------------------
// Version: 1.00
// Copyright (c) 2012 M4MSOFT
// ------------------------------------------------------------------------

// Include the Lua header files for C++.
#include "luaplayer.hpp"
#include "Lua_Color.hpp"

// Select the luaplayer objects to use in this file.
using luaplayer::luaL_newusermatatable;
using luaplayer::clamp;

// -----------------------------------------------------------------------------------------------------
// Define the function prototypes
static int lua_ColorNew (lua_State * L);
static int lua_ColorFromString (lua_State * L);
static int lua_ColorRed (lua_State * L);
static int lua_ColorGreen (lua_State * L);
static int lua_ColorBlue (lua_State * L);
static int lua_ColorAlpha (lua_State * L);
static int lua_ColorClone (lua_State * L);
static int lua_ColorToNumber (lua_State * L);
static int lua_ColorToString (lua_State * L);
static int lua_ColorToStringf (lua_State * L);
static int lua_ColorEqual (lua_State * L);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Create a new color.
static int lua_ColorNew (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg > 4) return luaL_error(L, "Argument error: Color.new([,red [,green [,blue [,alpha]]]]) takes zero to four arguments.");

	// Reset the Psp Color Data Buffer.
	lua_Color clrData = 0x00000000;

	// Read the RGB and or the Alpha values.
	int i = 0;
	while (i < narg)
	{
		clrData |= clamp(luaL_checkint(L, i+1), 0, 255) << (8*i);
		++i;
	}

	// Set the Alpha Value to 255 if Alpha isn't defined.
	if (narg < 4) clrData |= 0xffffffff << (8*narg);

	// Create the Color.
	(*lua_pushColor(L)) = clrData;

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Create a new color from a string.
static int lua_ColorFromString (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "Argument error: Color.fromString(value) takes one argument.");

	// Reset the Psp Color Data Buffer.
	lua_Color clrData = luaL_checkunsigned(L, 1);

	// Create the Color.
	(*lua_pushColor(L)) = clrData;

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Get or set the red color component.
static int lua_ColorRed (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1 && narg != 2) return luaL_error(L, "Argument error: Color.red(color [,red]) takes one or two arguments.");

	// get the Color Pointer.
	lua_Color *clrPtr = luaL_checkColor(L, 1);

	// Set the red color component.
	if (narg == 2)
	{
		// turn the red color component off.
		*clrPtr &= ~0x000000ff;
		// turn the new red color component on.
		*clrPtr |= clamp(luaL_checkint(L, 2), 0, 255);
	}

	// Return the red color component as a integer.
	lua_pushinteger(L, *clrPtr & 0xff); // First result.

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Get or set the red color component.
static int lua_ColorGreen (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1 && narg != 2) return luaL_error(L, "Argument error: Color.green(color [,green]) takes one or two arguments.");

	// get the Color Pointer.
	lua_Color *clrPtr = luaL_checkColor(L, 1);

	// Set the green color component.
	if (narg == 2)
	{
		// turn the green color component off.
		*clrPtr &= ~0x0000ff00;
		// turn the new green color component on.
		*clrPtr |= clamp(luaL_checkint(L, 2), 0, 255) << 8;
	}

	// Return the green color component as a integer.
	lua_pushinteger(L, (*clrPtr >> 8) & 0xff); // First result.

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Get or set the red color component.
static int lua_ColorBlue (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1 && narg != 2) return luaL_error(L, "Argument error: Color.blue(color [,blue]) takes one or two arguments.");

	// get the Color Pointer.
	lua_Color *clrPtr = luaL_checkColor(L, 1);

	// Set the blue color component. 11000011
	if (narg == 2)
	{
		// turn the blue color component off.
		*clrPtr &= ~0x00ff0000;
		// turn the new blue color component on.
		*clrPtr |= clamp(luaL_checkint(L, 2), 0, 255) << 16;
	}

	// Return the blue color component as a integer.
	lua_pushinteger(L, (*clrPtr >> 16) & 0xff); // First result.

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Get or set the red color component.
static int lua_ColorAlpha (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1 && narg != 2) return luaL_error(L, "Argument error: Color.alpha(color [,alpha]) takes one or two arguments.");

	// get the Color Pointer.
	lua_Color *clrPtr = luaL_checkColor(L, 1);

	// Set the alpha color component. 11000011
	if (narg == 2)
	{
		// turn the alpha color component off.
		*clrPtr &= ~0xff000000;
		// turn the new alpha color component on.
		*clrPtr |= clamp(luaL_checkint(L, 2), 0, 255) << 24;
	}

	// Return the alpha color component as a integer.
	lua_pushinteger(L, (*clrPtr >> 24) & 0xff); // First result.

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_ColorClone (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "Argument error: Color.clone(color) takes one argument.");

	// get the Color Pointer.
	lua_Color *clrPtr = luaL_checkColor(L, 1);

	// Create the Color.
	(*lua_pushColor(L)) = *clrPtr;

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Convert the color to a number.
static int lua_ColorToNumber (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "Argument error: Color.tonumber(color) takes one argument.");

	// get the Color Pointer.
	lua_Color *clrPtr = luaL_checkColor(L, 1);

	// Return the color as a number.
	lua_pushnumber(L, *clrPtr);

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Convert the color to a string.
static int lua_ColorToString (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "Argument error: Color.toString(color) takes one argument.");

	// get the Color Pointer.
	lua_Color *clrPtr = luaL_checkColor(L, 1);

	// Return the color info string.
	lua_pushfstring(L, "%p", *clrPtr);

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Convert the color to a string.
static int lua_ColorToStringf (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "Argument error: Color.tostring(color) takes one argument.");

	// get the Color Pointer.
	lua_Color *clrPtr = luaL_checkColor(L, 1);

	// Return the color info string.
	lua_pushfstring(L, "Color (r %d, g %d, b %d, a %d)", (*clrPtr       ) & 0xff,
														 (*clrPtr >> 8  ) & 0xff,
														 (*clrPtr >> 16 ) & 0xff,
														 (*clrPtr >> 24 ) & 0xff );

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Check if two color are equal.
static int lua_ColorEqual (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 2) return luaL_error(L, "Argument error: Color.equal(color_A, color_B) takes two arguments.");

	// Return the text result.
	lua_pushboolean(L, *luaL_checkColor(L, 1) == *luaL_checkColor(L, 2));

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Define the module table.
static const luaL_Reg mod_lib[] =
{
	{"new", lua_ColorNew},
	{"fromString", lua_ColorFromString},
	{"red", lua_ColorRed},
	{"green", lua_ColorGreen},
	{"blue", lua_ColorBlue},
	{"alpha", lua_ColorAlpha},
	{"clone", lua_ColorClone},
	{"tonumber", lua_ColorToNumber},
	{"toString", lua_ColorToString},
	{NULL, NULL}
};
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Push the Color userdata.
lua_Color* lua_pushColor(lua_State * L)
{
	lua_Color* newvalue = (lua_Color*) lua_newuserdata(L, sizeof(lua_Color));
	luaL_setmetatable(L, LUA_COLORHANDLE);
	return newvalue;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Define the Color userdata metatable.
static const luaL_Reg Color_lib[] =
{
	{"red", lua_ColorRed},
	{"green", lua_ColorGreen},
	{"blue", lua_ColorBlue},
	{"alpha", lua_ColorAlpha},
	{"clone", lua_ColorClone},
	{"tonumber", lua_ColorToNumber},
	{"toString", lua_ColorToString},
	{"__tostring", lua_ColorToStringf},
	{"__eq", lua_ColorEqual},
	{NULL, NULL}
};
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Open the PSP lua Color libraray.
LUAMOD_API int luaopen_Color(lua_State * L)
{
	// Create the module.
	luaL_newlib(L, mod_lib);

	// Create the Color user metatable.
	luaL_newusermatatable(L, LUA_COLORHANDLE, Color_lib);

	// Create a dummy object.
	(*lua_pushColor(L)) = 0x00000000;
	lua_setfield(L, -2, "object");

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
