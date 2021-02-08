// PSP Lua Palette ( FileName: Lua_Palette.cpp )
// ------------------------------------------------------------------------
// Version: 1.00
// Copyright (c) 2012 M4MSOFT
// ------------------------------------------------------------------------

// Include the Lua header files for C++.
#include "luaplayer.hpp"
#include "Lua_Palette.hpp"
#include "Lua_Color.hpp"
#include "Lua_Texture.hpp"

// Select the luaplayer objects to use in this file.
using luaplayer::luaL_newusermatatable;
using luaplayer::clamp;

// -----------------------------------------------------------------------------------------------------
// Define the function prototypes
static int lua_PaletteNew (lua_State * L);
static int lua_PaletteLoad (lua_State * L);
static int lua_PaletteFromImage (lua_State * L);
static int lua_PaletteSave (lua_State * L);
static int lua_PaletteColor (lua_State * L);
static int lua_PaletteClone (lua_State * L);
static int lua_PaletteSort (lua_State * L);
static int lua_PaletteLenght (lua_State * L);
static int lua_PaletteFree (lua_State * L);
static int lua_PaletteToString (lua_State * L);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Create a new Palette.
static int lua_PaletteNew (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "Argument error: Palette.new(size) takes one argument.");

	// Create the new Palette.
	lua_Palette pltPtr = NULL;
	int size = luaL_checkunsigned(L, 1);
	pltPtr = g2dPaletteCreate((size == 16 ? G2D_PALETTE_T4 : (size == 256 ? G2D_PALETTE_T8 : G2D_PALETTE_NONE)), size);

	// Return the Palette.
	if (pltPtr)
		(*lua_pushPalette(L)) = pltPtr;
	else
		lua_pushnil(L);

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Load the Palette.
static int lua_PaletteLoad (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "Argument error: Palette.load(path) takes one argument.");

	// Create the new Palette.
	lua_Palette pltPtr = NULL;
	pltPtr = g2dPaletteLoad (luaL_checkstring(L, 1));

	// Return the Palette.
	if (pltPtr)
		(*lua_pushPalette(L)) = pltPtr;
	else
		lua_pushnil(L);

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Build a palette from a image.
static int lua_PaletteFromImage (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 2) return luaL_error(L, "Argument error: Palette.fromImage(texture, size) takes two arguments.");

	// Create the new Palette.
	lua_Palette pltPtr = NULL;
	g2dPalette_Type type = G2D_PALETTE_NONE;
	int size = luaL_checkunsigned(L, 2);
	switch (size)
	{
		case 16 :
			type = G2D_PALETTE_T4;
			break;
		case 256 :
			type = G2D_PALETTE_T8;
			break;
		default:
			return luaL_error(L, "Palette.fromImage: invalid palette size (range: 16 or 256)");
	}
	pltPtr = g2dTexCreatePalette(*luaL_checkTexture(L, 1), type);

	// Return the Palette.
	if (pltPtr)
		(*lua_pushPalette(L)) = pltPtr;
	else
		lua_pushnil(L);

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Save the Palette.
static int lua_PaletteSave (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 2) return luaL_error(L, "Argument error: Palette.save(plt, path) takes two arguments.");

	// Save the Palette.
	g2dPaletteSave(luaL_checkstring(L, 2), *luaL_checkPalette(L, 1));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Get or Set the palette color.
static int lua_PaletteColor (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 2 && narg != 3) return luaL_error(L, "Argument error: Palette.color(plt, index, [color]) takes two or three arguments.");

	// Get the Palette Pointer.
	lua_Palette pltPtr = *luaL_checkPalette(L, 1);

	// Get the key value.
	int index = clamp(luaL_checkint(L, 2), 0, pltPtr->count - 1);

	// Set the new color.
	if (narg == 3)
		*(pltPtr->data + index) = *lua_toColor(L, 3);

	// Return the color.
	(*lua_pushColor(L)) = *(pltPtr->data + index);

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Clone the palette.
static int lua_PaletteClone (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "Argument error: Palette.clone(plt) takes one argument.");

	// Get the Palette Pointer.
	lua_Palette pltPtr = *luaL_checkPalette(L, 1);

	// Create the new Palette.
	lua_Palette pltCPtr = NULL;
	pltCPtr = g2dPaletteCreate(pltPtr->type, pltPtr->count);
	if (pltCPtr == NULL) return luaL_error(L, "Palette.clone: can't clone the palette");

	// Clone the palette.
	for (unsigned int n = 0; n < pltPtr->count; ++n)
		*(pltCPtr + n) = *(pltPtr + n);

	// Return the Palette.
	(*lua_pushPalette(L)) = pltCPtr;

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Sort the color in the Palette.
static int lua_PaletteSort (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "Argument error: Palette.sort(plt) takes one argument.");

	// Sort the Palette.
	g2dPaletteSort(*luaL_checkPalette(L, 1));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Get the lenght of the color palette.
static int lua_PaletteLenght (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "Argument error: Palette.len(plt) takes one argument.");

	// Get the Palette Pointer.
	lua_Palette pltPtr = *luaL_checkPalette(L, 1);

	// Return the lenght.
	lua_pushinteger(L,  pltPtr->count);

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// free the Palette.
static int lua_PaletteFree (lua_State * L)
{
	// Get the Palette Pointer.
	lua_Palette *pltPtr = lua_toPalette(L, 1);

	// Free the Palette Pointer and set the Pointer to NULL.
	if (pltPtr && *pltPtr)
		g2dPaletteFree(pltPtr);

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Convert the Palette object toa string.
static int lua_PaletteToString (lua_State * L)
{
	// Get the Palette Pointer.
	lua_Palette *pltPtr = lua_toPalette(L, 1);

	// Return the Palette info string.
	if (pltPtr && *pltPtr)
		lua_pushfstring(L, "Palette: (%p) [Colors = %d L = %d]", (*pltPtr)->data, (*pltPtr)->count, (*pltPtr)->memCount);
	else
		lua_pushstring(L, "nil");

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Define the module table.
static const luaL_Reg mod_lib[] =
{
	{"new", lua_PaletteNew},
	{"load", lua_PaletteLoad},
	{"fromImage", lua_PaletteFromImage},
	{"save", lua_PaletteSave},
	{"color", lua_PaletteColor},
	{"clone", lua_PaletteClone},
	{"sort",  lua_PaletteSort},
	{"len", lua_PaletteLenght},
	//{"free", lua_PaletteFree},
	{NULL, NULL}
};
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Push the Palette userdata.
lua_Palette* lua_pushPalette(lua_State * L)
{
	lua_Palette* newvalue = (lua_Palette*) lua_newuserdata(L, sizeof(lua_Palette));
	luaL_setmetatable(L, LUA_PALETTEHANDLE);
	return newvalue;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Define the Palette userdata metatable.
static const luaL_Reg Palette_lib[] =
{
	{"save", lua_PaletteSave},
	{"color", lua_PaletteColor},
	{"clone", lua_PaletteClone},
	{"sort",  lua_PaletteSort},
	{"len", lua_PaletteLenght},
	//{"free", lua_PaletteFree},
	{"__gc", lua_PaletteFree},
	{"__tostring", lua_PaletteToString},
	{NULL, NULL}
};
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Open the PSP lua Palette libraray.
LUAMOD_API int luaopen_Palette(lua_State * L)
{
	// Create the module.
	luaL_newlib(L, mod_lib);

	// Create the Color user metatable.
	luaL_newusermatatable(L, LUA_PALETTEHANDLE, Palette_lib);

	// Create a dummy object.
	(*lua_pushPalette(L)) = NULL;
	lua_setfield(L, -2, "object");

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
