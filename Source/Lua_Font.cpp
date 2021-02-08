// PSP Lua Font ( FileName: Lua_Font.cpp )
// ------------------------------------------------------------------------
// Version: 1.01
// Copyright (c) 2014 M4MSOFT
// ------------------------------------------------------------------------

// Include C/C++ header files.
//#include <malloc.h>
//#include <stdlib.h>

// Include the Lua header files for C++.
#include "luaplayer.hpp"
#include "Lua_Font.hpp"
#include "Lua_Color.hpp"
#include "Lua_Texture.hpp"

// Select the luaplayer objects to use in this file.
using luaplayer::luaL_newusermatatable;
using luaplayer::luaL_checkboolean;

// -----------------------------------------------------------------------------------------------------
// Define the function prototypes
static int lua_FontLoad (lua_State * L);

static int lua_FontMeasureText (lua_State * L);
static int lua_FontRender (lua_State * L);

static int lua_FontPush (lua_State * L);
static int lua_FontPop (lua_State * L);
static int lua_FontClearStack (lua_State * L);

static int lua_FontSet (lua_State * L);
static int lua_FontSetColor (lua_State * L);
static int lua_FontSetAngle (lua_State * L);
static int lua_FontSetDepht (lua_State * L);
static int lua_FontSetSize (lua_State * L);
static int lua_FontReset (lua_State * L);

static int lua_FontName (lua_State * L);
static int lua_FontSize (lua_State * L);
static int lua_FontHeight (lua_State * L);
static int lua_FontyOffset (lua_State * L);
static int lua_FontPrint (lua_State * L);

static int lua_FontFree (lua_State * L);
static int lua_FontToString (lua_State * L);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Load  the font from a file.
static int lua_FontLoad (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1 && narg != 2) return luaL_error(L, "Argument error: Font.load(filename [,chache]) takes one or two arguments.");

	// Load the font file.
	lua_Font fntPtr = NULL;
	fntPtr = gfFontLoad(luaL_checkstring(L, 1), narg == 1 ? true : lua_toboolean(L, 2));

	// Return the Font.
	if (fntPtr)
		(*lua_pushFont(L)) = fntPtr;
	else
		lua_pushnil(L);

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Measure a length of text if it were to be drawn.
static int lua_FontMeasureText (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "Argument error: Font.measureText(text) takes one argument.");

	gfGlyph glyph = {{0,0,0,0},{0,0},{0,0},0};

	// Measure the text screen text.
	gfMeasureFont(luaL_checkstring(L, 1), &glyph);

	// Return the text position and size.
	lua_newtable(L);
	lua_pushstring(L, "x"); lua_pushinteger(L, glyph.offset.x); lua_settable(L, -3);
	lua_pushstring(L, "y"); lua_pushinteger(L, glyph.offset.y); lua_settable(L, -3);
	lua_pushstring(L, "width"); lua_pushinteger(L, glyph.size.width); lua_settable(L, -3);
	lua_pushstring(L, "height"); lua_pushinteger(L, glyph.size.height); lua_settable(L, -3);
	lua_pushstring(L, "nextX"); lua_pushinteger(L, glyph.advance.x); lua_settable(L, -3);
	lua_pushstring(L, "nextY"); lua_pushinteger(L, glyph.advance.y); lua_settable(L, -3);
	lua_pushstring(L, "error"); lua_pushboolean(L, glyph.error); lua_settable(L, -3);

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Render the font.
static int lua_FontRender (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);

	// Select the arguments.
	switch (narg)
	{
		case 3 : // Print the Font to the screen.
			gfTextToScreen(luaL_checkstring(L, 3), luaL_checkint(L, 1), luaL_checkint(L, 2));
			break;
		case 4 : // Print the Font to a texture.
			gfTextToImage(*luaL_checkTexture(L, 1), luaL_checkstring(L, 4), luaL_checkint(L, 2), luaL_checkint(L, 3));
			break;
		default:
			return luaL_error(L, "Argument error: Font.render([texture,] x, y, text) takes three or four arguments.");
	}

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Saves the current font state to stack.
static int lua_FontPush (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "Argument error: Font.push() takes no arguments.");

	// Saves the font state.
	gfPush();

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Restore the current font state from stack.
static int lua_FontPop (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "Argument error: Font.pop() takes no arguments.");

	// Restore the font state.
	gfPop();

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Clear the font state stack.
static int lua_FontClearStack (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "Argument error: Font.clearStack() takes no arguments.");

	// Clear the stack.
	gfClearStack();

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Set or reset the font.
static int lua_FontSet (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg > 1) return luaL_error(L, "Argument error: Font.set([font]) takes zero or one argument.");

	if (narg)
		// Set the current font.
		gfSetFont(*luaL_checkFont(L, 1));
	else
		// Reset the current font.
		gfResetFont();

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Set or reset the current font color.
static int lua_FontSetColor (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg > 1) return luaL_error(L, "Argument error: Font.setColor([color]) takes zero or one argument.");

	if (narg)
		// Set the current font color.
		gfSetFontColor(*luaL_checkColor(L, 1));
	else
		// Reset the current font color.
		gfResetFontColor();

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Set or reset the current font angle.
static int lua_FontSetAngle (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg > 1) return luaL_error(L, "Argument error: Font.setAngle([angle]) takes zero or one argument.");

	if (narg)
		// Set the current font angle.
		gfSetFontAngle(luaL_checknumber(L, 1));
	else
		// Reset the current font angle.
		gfResetFontAngle();

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Set or reset the current font depht.
static int lua_FontSetDepht (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg > 1) return luaL_error(L, "Argument error: Font.setDepht([z]) takes zero or one argument.");

	if (narg)
		// Set the current font depht.
		gfSetFontDepht(luaL_checknumber(L, 1));
	else
		// Reset the current font depht.
		gfResetFontDepht();

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Set or reset the current font size.
static int lua_FontSetSize (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg > 2) return luaL_error(L, "Argument error: Font.setSize([size [,points]]) takes zero to two argument.");

	if (narg)
		// Set the current font size.
		gfSetFontSize(luaL_checknumber(L, 1), (narg > 1 ? lua_toboolean(L, 2) : true));
	else
		// Reset the current font size.
		gfResetFontSize();

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Reset the current font state.
static int lua_FontReset (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "Argument error: Font.reset() takes no arguments.");

	// Reset the current font state.
	gfReset();

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Get the name of the font.
static int lua_FontName (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "Argument error: Font.name(font) takes no arguments.");

	// Get the Font Pointer.
	lua_Font fntPtr = *luaL_checkFont(L, 1);

	// Return the font name as a string.
	lua_pushstring(L, fntPtr->name ? fntPtr->name : "unknown");

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Get or set the size of the font.
static int lua_FontSize (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg < 1 || narg > 3) return luaL_error(L, "Argument error: Font.size(font [,size [,points]]) takes one to three arguments.");

	// Get the Font Pointer.
	lua_Font fntPtr = *luaL_checkFont(L, 1);

	// Set the size of the font.
	if (narg > 1)
		gfSetCharSize(fntPtr, luaL_checkint(L, 2), (narg > 2 ? lua_toboolean(L, 3) : false));

	// Return the font size as a integer.
	lua_pushinteger(L, fntPtr->size); // First result.

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Get the Height of the font.
static int lua_FontHeight (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "Argument error: Font.height(font) takes one argument.");

	// Get the Font Pointer.
	lua_Font fntPtr = *luaL_checkFont(L, 1);

	// Return the font height as a integer.
	lua_pushinteger(L, (fntPtr->face->size->metrics.height) >> GLIBF_SIZE_SHIFT); // First result.

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Get the y offset of the font.
static int lua_FontyOffset (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "Argument error: Font.offsetY(font) takes one argument.");

	// Get the Font Pointer.
	lua_Font fntPtr = *luaL_checkFont(L, 1);

	// Return the font offset as a integer.
	lua_pushinteger(L, (fntPtr->face->size->metrics.ascender) >> GLIBF_SIZE_SHIFT); // First result.
	//lua_pushinteger(L, (fntPtr->face->size->metrics.descender) >> GLIBF_SIZE_SHIFT); // First result.

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Print the font.
static int lua_FontPrint (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg < 4 || narg > 7) return luaL_error(L, "Argument error: Font.print(font, x, y, text [,clr [,angle [,z]]]) takes four to zeven arguments.");

	// Get the arguments
	lua_Font fntPtr = *luaL_checkFont(L, 1);
	lua_Number x = luaL_checknumber(L, 2);
	lua_Number y = luaL_checknumber(L, 3);
	const char *text = luaL_checkstring(L, 4);
	lua_Color color = (narg > 4 ?  *luaL_checkColor(L, 5): 0xffffffff);
	lua_Number angle = (narg > 5 ? luaL_checknumber(L, 6): 0.0f);
	lua_Number depth = (narg > 6 ? luaL_checknumber(L, 7): 0.0f);

	gfPush(); // Saves the current font state.
	gfSetFontDepht(depth); // Set the depht.
	gfSetFontAngle(angle); // Set the angle.
	gfSetFontColor(color);	// Set the Color.
	gfSetFont(fntPtr); // Set the font
	gfTextToScreen(text, x, y); // Print the font.
	gfPop(); // Restore the font state.

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// free the Font.
static int lua_FontFree (lua_State * L)
{
	// Get the Font Pointer.
	lua_Font *fntPtr = lua_toFont(L, 1);

	// Free the Font Pointer and set the Pointer to NULL.
	if (fntPtr && *fntPtr)
		gfFontFree(fntPtr);

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Convert the Font object toa string.
static int lua_FontToString (lua_State * L)
{
	// Get the Timer Pointer.
	lua_Font *fntPtr = lua_toFont(L, 1);

	// Return the Font info string.
	if (fntPtr && *fntPtr)
		lua_pushfstring(L, "Font: (name: %s, size: %d, chache: %p)", (*fntPtr)->name ? (*fntPtr)->name : "unknown",
														             (*fntPtr)->size,
	                                                                 (*fntPtr)->data);
	else
		lua_pushstring(L, "nil");

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Define the module table.
static const luaL_Reg mod_lib[] =
{
	{"load", lua_FontLoad},

	{"measureText", lua_FontMeasureText},
	{"render", lua_FontRender},

	{"push", lua_FontPush},
	{"pop", lua_FontPop},
	{"clearStack", lua_FontClearStack},

	{"set", lua_FontSet},
	{"setColor", lua_FontSetColor},
	{"setAngle", lua_FontSetAngle},
	{"setDepht", lua_FontSetDepht},
	{"setSize", lua_FontSetSize},
	{"reset", lua_FontReset},

	{"name", lua_FontName},
	{"size", lua_FontSize},
	{"height", lua_FontHeight},
	{"offsetY", lua_FontyOffset},
	{"print", lua_FontPrint},
	//{"free", lua_FontFree},

	{NULL, NULL}
};
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Push the Font userdata.
lua_Font* lua_pushFont(lua_State * L)
{
	lua_Font* newvalue = (lua_Font*) lua_newuserdata(L, sizeof(lua_Font));
	luaL_setmetatable(L, LUA_FONTHANDLE);
	return newvalue;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Define the Font userdata metatable.
static const luaL_Reg Font_lib[] =
{
	{"name", lua_FontName},
	{"size", lua_FontSize},
	{"height", lua_FontHeight},
	{"offsetY", lua_FontyOffset},
	{"print", lua_FontPrint},
	//{"free", lua_FontFree},
	{"__gc", lua_FontFree},
	{"__tostring", lua_FontToString},
	{NULL, NULL}
};
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Open the PSP lua Font libraray.
LUAMOD_API int luaopen_Font(lua_State * L)
{
	// Create the module.
	luaL_newlib(L, mod_lib);

	// Create the Font user metatable.
	luaL_newusermatatable(L, LUA_FONTHANDLE, Font_lib);

	// Create a dummy object.
	(*lua_pushFont(L)) = NULL;
	lua_setfield(L, -2, "object");

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Updates Version 1.01: 05/09-2014
// 	  - Fixed the 'Font.Reset()' to only accept zero arguments.
//    - Fixed the 'Font.name([Font])' to only accept 1 arguments.
// -----------------------------------------------------------------------------------------------------
