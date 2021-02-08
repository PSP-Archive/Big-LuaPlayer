// PSP Lua BmpFont ( FileName: Lua_BmpFont.hpp )
// ------------------------------------------------------------------------
// Header file for LuaPlayer.
// Version: 1.00
// Copyright (c) 2012 M4MSOFT
// ------------------------------------------------------------------------

// Include the Lua header files for C++.
#include "luaplayer.hpp"
#include "Lua_Font.hpp"
#include "Lua_BmpFont.hpp"
#include "Lua_Color.hpp"
#include "Lua_Texture.hpp"

// Select the luaplayer objects to use in this file.
using luaplayer::luaL_newusermatatable;
using luaplayer::luaL_typerror;
using luaplayer::min;
using luaplayer::clamp;

// -----------------------------------------------------------------------------------------------------
// Define the function prototypes
static int lua_BmpFontNew (lua_State * L);
static int lua_BmpFontLoad (lua_State * L);
static int lua_BmpFontFromFont (lua_State * L);
static int lua_BmpFontFromImage (lua_State * L);

static int lua_BmpFontTexture (lua_State * L);
static int lua_BmpFontHeight (lua_State * L);
static int lua_BmpFontOffsetX (lua_State * L);
static int lua_BmpFontOffsetY (lua_State * L);
static int lua_BmpFontLen (lua_State * L);
static int lua_BmpFontCharMap (lua_State * L);
static int lua_BmpFontGlyph (lua_State * L);
static int lua_BmpFontPrint (lua_State * L);
static int lua_BmpFontMeasureText (lua_State * L);

static int lua_BmpFontFree(lua_State * L);
static int lua_BmpFontToString (lua_State * L);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Create a new empty bmpFont object.
static int lua_BmpFontNew(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "Argument error: BmpFont.new() takes no arguments.");

	// Build the bmpFont.
	lua_BmpFont bfntPtr = NULL;
	bfntPtr = gfBmpFontCreate();
	if (bfntPtr == NULL) return luaL_error(L, "BmpFont.new: out of memory");
	bfntPtr->tex = g2dTexCreate(1, 1, G2D_PSM_8888, false);
	if (bfntPtr->tex == NULL) {gfBmpFontFree(&bfntPtr); return luaL_error(L, "BmpFont.new: out of memory");}

	// Return the bmpFont.
	(*lua_pushBmpFont(L)) = bfntPtr;

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Create a new bmpFont from a file.
static int lua_BmpFontLoad(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg < 2 || narg > 7) return luaL_error(L, "Argument error: BmpFont.load(path, size [,angle [,points [,charset [,vram [,format]]]]]) takes two to seven arguments.");

	// Build the bmpFont.
	lua_BmpFont bfntPtr = NULL;
	bfntPtr = gfBmpFontLoad(luaL_checkstring(L, 1), luaL_checkint(L, 2),
							(narg > 2 ? luaL_checknumber(L, 3) : 0.0),
							(narg > 3 ? lua_toboolean(L, 4) : false),
							(narg > 4 ? lua_tostring(L, 5) : NULL),
						    (narg > 6 ? clamp(luaL_checkint(L, 7), 2, 5) : G2D_PSM_T8),
							(narg > 5 ? lua_toboolean(L, 6) : false));

	// Return the bmpFont.
	if (bfntPtr)
		(*lua_pushBmpFont(L)) = bfntPtr;
	else
		lua_pushnil(L);

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Create a new bmpFont from the current font.
static int lua_BmpFontFromFont(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg > 4) return luaL_error(L, "Argument error: BmpFont.fromFont([charset [,vram [,format]]]) takes zero to three arguments.");

	// Build the bmpFont.
	lua_BmpFont bfntPtr = NULL;
	bfntPtr = gfBmpFontFromFont((narg > 0 ? lua_tostring(L, 1) : NULL),
							    (narg > 2 ? clamp(luaL_checkint(L, 3), 2, 5) : G2D_PSM_T8),
								(narg > 1 ? lua_toboolean(L, 2) : false));

	// Return the bmpFont.
	if (bfntPtr)
		(*lua_pushBmpFont(L)) = bfntPtr;
	else
		lua_pushnil(L);

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Create a new bmpFont from a texture or texture file.
static int lua_BmpFontFromImage(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg < 3 || narg > 5) return luaL_error(L, "Argument error: BmpFont.fromImage(texture or path, width, height [,charset [,upper]]) takes three to five arguments.");

	// Build the bmpFont.
	lua_Texture  texPtr = NULL;
	lua_BmpFont bfntPtr = NULL;
	lua_Texture *pTexPtr = lua_toTexture(L, 1);
	if (pTexPtr)
		bfntPtr = gfBmpFontFromTex(*pTexPtr, luaL_checkint(L, 2), luaL_checkint(L, 3),
			(narg > 3 ? lua_tostring(L, 4) : NULL), (narg > 4 ? lua_toboolean(L, 5) : false), true);
	else
	{
		texPtr = g2dTexLoad(luaL_checkstring(L, 1), G2D_PSM_8888, false, G2D_SWIZZLE);
		if (texPtr == NULL) return luaL_error(L, "BmpFont.fromImage: can't load the texture file.");
		bfntPtr = gfBmpFontFromTex(texPtr, luaL_checkint(L, 2), luaL_checkint(L, 3),
			(narg > 3 ? lua_tostring(L, 4) : NULL), (narg > 4 ? lua_toboolean(L, 5) : false), false);
	}

	// Return the bmpFont.
	if (bfntPtr)
		(*lua_pushBmpFont(L)) = bfntPtr;
	else
		lua_pushnil(L);

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Get or set the BmpFont texture.
static int lua_BmpFontTexture(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1 && narg != 2) return luaL_error(L, "Argument error: BmpFont.texture(bmpFont [,texture]) takes one or two argument.");

	// Get the BmpFont Pointer.
	lua_BmpFont bfntPtr = *luaL_checkBmpFont(L, 1);

	// Get the current bmpFont texture Pointer.
	lua_Texture texPtr = bfntPtr->tex;

	if (narg == 2){
		// Link The Texture to the bmpFont.
		lua_Texture texPtrLink = *luaL_checkTexture(L, 2);
		if (g2dTexAddLink(texPtrLink)) return luaL_error(L, "BmpFont.texture: can't link the texture.");
		bfntPtr->tex = texPtrLink;
		// Return the Texture.
		if (texPtr)
			(*lua_pushTexture(L)) = texPtr;
		else
			lua_pushnil(L);
	}
	else
		// Return and link the Texture to lua.
		if (!g2dTexAddLink(texPtr))
			(*lua_pushTexture(L)) = texPtr;
		else
			lua_pushnil(L);

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Get or set the bmpFont height.
static int lua_BmpFontHeight(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1 && narg != 2) return luaL_error(L, "Argument error: BmpFont.height(bmpFont [,height]) takes one or two argument.");

	// Get the BmpFont Pointer.
	lua_BmpFont bfntPtr = *luaL_checkBmpFont(L, 1);

	// Set the BmpFont height.
	if (narg == 2)
		bfntPtr->height = luaL_checkint(L, 2);

	// Return the BmpFont height as a Lua_integer.
	lua_pushinteger(L, bfntPtr->height); // First result.

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Get or set the bmpFont x offset.
static int lua_BmpFontOffsetX(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1 && narg != 2) return luaL_error(L, "Argument error: BmpFont.offsetX(bmpFont [,x0]) takes one or two argument.");

	// Get the BmpFont Pointer.
	lua_BmpFont bfntPtr = *luaL_checkBmpFont(L, 1);

	// Set the BmpFont x offset.
	if (narg == 2)
		bfntPtr->offset.x = luaL_checkint(L, 2);

	// Return the BmpFont x offset as a Lua_integer.
	lua_pushinteger(L, bfntPtr->offset.x); // First result.

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Get or set the bmpFont y offset.
static int lua_BmpFontOffsetY(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1 && narg != 2) return luaL_error(L, "Argument error: BmpFont.offsetY(bmpFont [,y0]) takes one or two argument.");

	// Get the BmpFont Pointer.
	lua_BmpFont bfntPtr = *luaL_checkBmpFont(L, 1);

	// Set the BmpFont x offset.
	if (narg == 2)
		bfntPtr->offset.y = luaL_checkint(L, 2);

	// Return the BmpFont x offset as a Lua_integer.
	lua_pushinteger(L, bfntPtr->offset.y); // First result.

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Get the lenght of the glyph slot array.
static int lua_BmpFontLen(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "Argument error: BmpFont.len(bmpFont) takes one argument.");

	// Get the BmpFont Pointer.
	lua_BmpFont bfntPtr = *luaL_checkBmpFont(L, 1);

	// Return the lenght of the glyph slot array.
	lua_pushinteger(L, bfntPtr->num_charmaps); // First result.

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Get or set the character slot position.
static int lua_BmpFontCharMap(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 2 && narg != 3) return luaL_error(L, "Argument error: BmpFont.charmap(bmpFont, char [,slot]) takes two or three argument.");

	// Get the BmpFont Pointer.
	lua_BmpFont bfntPtr = *luaL_checkBmpFont(L, 1);
	unsigned char uChar = (unsigned char) *(luaL_checkstring(L, 2));

	// Set the charmap.
	if (narg == 3)
		bfntPtr->charmaps[uChar] = clamp(luaL_checkint(L, 3), 0, bfntPtr->num_charmaps-1);

	// Return the character slot position.
	lua_pushinteger(L, bfntPtr->charmaps[uChar]); // First result.

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Get or set the bmpFont glyph slot. slot (range: 0 - len), tGlyph = {x, y, w, h, x0, y0, adv_x, adv_y}
static int lua_BmpFontGlyph(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 2 && narg != 3) return luaL_error(L, "Argument error: BmpFont.glyph(bmpFont, slot [,tGlyph]) takes two or three argument.");

	// Get the BmpFont Pointer.
	lua_BmpFont bfntPtr = *luaL_checkBmpFont(L, 1);
	int iSlot = clamp(luaL_checkint(L, 2), 0, 255);

	// Set the glyph slot.
	if (narg == 3)
	{
		if (lua_istable(L, 3)){
			int len = lua_rawlen(L, 3);
			if (len != 8) return luaL_error(L, "wrong number of glyph components {x, y, w, h, x0, y0, adv_x, adv_y}");
			for (int i = 1; i <= len; ++i){
				lua_rawgeti(L, 3, i);
				if (!lua_isnumber(L, -1)){
					for (int iStart = i; iStart > 0; --iStart) lua_remove(L, -1);
					return luaL_typerror(L, narg, lua_typename(L, LUA_TNUMBER));}
			}
			// Add a glyph slot to the top of the glyph array.
			if (iSlot > bfntPtr->num_charmaps-1) iSlot = bfntPtr->num_charmaps++;
			bfntPtr->glyph[iSlot].advance.y =   lua_tointeger(L, -1); lua_remove(L, -1);
			bfntPtr->glyph[iSlot].advance.x =   lua_tointeger(L, -1); lua_remove(L, -1);
			bfntPtr->glyph[iSlot].offset.y =    lua_tointeger(L, -1); lua_remove(L, -1);
			bfntPtr->glyph[iSlot].offset.x =    lua_tointeger(L, -1); lua_remove(L, -1);
			bfntPtr->glyph[iSlot].size.height = lua_tointeger(L, -1); lua_remove(L, -1);
			bfntPtr->glyph[iSlot].size.width =  lua_tointeger(L, -1); lua_remove(L, -1);
			bfntPtr->glyph[iSlot].size.y =      lua_tointeger(L, -1); lua_remove(L, -1);
			bfntPtr->glyph[iSlot].size.x =      lua_tointeger(L, -1); lua_remove(L, -1);
		}
		else
			return luaL_typerror(L, narg, lua_typename(L, LUA_TTABLE));

		return 0; // Number of results.
	}
	iSlot = min(iSlot, bfntPtr->num_charmaps-1);

	// Return the glyph slot.
	lua_newtable(L);
	lua_pushnumber(L, 1); lua_pushinteger(L, bfntPtr->glyph[iSlot].size.x);      lua_settable(L, -3);
	lua_pushnumber(L, 2); lua_pushinteger(L, bfntPtr->glyph[iSlot].size.y);      lua_settable(L, -3);
	lua_pushnumber(L, 3); lua_pushinteger(L, bfntPtr->glyph[iSlot].size.width);  lua_settable(L, -3);
	lua_pushnumber(L, 4); lua_pushinteger(L, bfntPtr->glyph[iSlot].size.height); lua_settable(L, -3);
	lua_pushnumber(L, 5); lua_pushinteger(L, bfntPtr->glyph[iSlot].offset.x);    lua_settable(L, -3);
	lua_pushnumber(L, 6); lua_pushinteger(L, bfntPtr->glyph[iSlot].offset.y);    lua_settable(L, -3);
	lua_pushnumber(L, 7); lua_pushinteger(L,bfntPtr->glyph[iSlot].advance.x);    lua_settable(L, -3);
	lua_pushnumber(L, 8); lua_pushinteger(L,bfntPtr->glyph[iSlot].advance.y);    lua_settable(L, -3);

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Print the text to the screen.
static int lua_BmpFontPrint(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);

	// Select the arguments and print the text.
	switch (narg)
	{
		case 5 :
			gfRenderBmpFont (*luaL_checkBmpFont(L, 1),
		                     luaL_checkint(L, 2), luaL_checkint(L, 3), 0,
		                     luaL_checkstring(L, 4),
		                     *luaL_checkColor(L, 5), 0);
			break;
		case 6 :
			gfRenderBmpFont (*luaL_checkBmpFont(L, 1),
		                     luaL_checkint(L, 2), luaL_checkint(L, 3), 0,
		                     luaL_checkstring(L, 4),
		                     *luaL_checkColor(L, 5), luaL_checkint(L, 6));
			break;
		case 7 :
			gfRenderBmpFont (*luaL_checkBmpFont(L, 1),
		                     luaL_checkint(L, 2), luaL_checkint(L, 3), luaL_checkint(L, 7),
		                     luaL_checkstring(L, 4),
		                     *luaL_checkColor(L, 5), luaL_checkint(L, 6));
			break;
		default:
			return luaL_error(L, "Argument error: BmpFont.print(bmpFont, x, y, text, color [,monoSpace [,z]]) takes five to seven arguments.");
	}

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Mesure the text.
static int lua_BmpFontMeasureText(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 2 && narg != 3) return luaL_error(L, "Argument error: BmpFont.measureText(bmpFont, text [,monoSpace]) takes two or three arguments.");

	gfGlyph glyph = {{0,0,0,0},{0,0},{0,0},0};

	// Measure the text..
	gfMeasureBmpFont(*luaL_checkBmpFont(L, 1), luaL_checkstring(L, 2), &glyph, (narg > 2 ? luaL_checkint(L, 3) : 0));

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
// Free a BmpFont.
static int lua_BmpFontFree(lua_State * L)
{
	// Get the BmpFont Pointer.
	lua_BmpFont *bfntPtr = lua_toBmpFont(L, 1);

	// Free the BmpFont Pointer and set the Pointer to NULL.
	if (bfntPtr && *bfntPtr)
		gfBmpFontFree(bfntPtr);

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Convert the BmpFont to a string.
static int lua_BmpFontToString(lua_State * L)
{
	// Get the BmpFont Pointer.
	lua_BmpFont *bfntPtr = lua_toBmpFont(L, 1);

	// Return the texture info string.
	if (bfntPtr && *bfntPtr)
		lua_pushfstring(L, "BmpFont: %p (height: %d)", *bfntPtr, (*bfntPtr)->height);
	else
		lua_pushstring(L, "nil");

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Define the module table.
static const luaL_Reg mod_lib[] =
{
	{"new", lua_BmpFontNew},
	{"load", lua_BmpFontLoad},
	{"fromFont", lua_BmpFontFromFont},
	{"fromImage", lua_BmpFontFromImage},

	{"texture", lua_BmpFontTexture},
	{"height", lua_BmpFontHeight},
	{"offsetX", lua_BmpFontOffsetX},
	{"offsetY", lua_BmpFontOffsetY},
	{"len", lua_BmpFontLen},
	{"charmap", lua_BmpFontCharMap},
	{"glyph", lua_BmpFontGlyph},

	{"print", lua_BmpFontPrint},
	{"measureText", lua_BmpFontMeasureText},

	//{"free", lua_BmpFontFree},
	{NULL, NULL}
};
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Push the Color userdata.
lua_BmpFont* lua_pushBmpFont(lua_State * L)
{
	lua_BmpFont* newvalue = (lua_BmpFont*) lua_newuserdata(L, sizeof(lua_BmpFont));
	luaL_setmetatable(L, LUA_BMPFONTHANDLE);
	return newvalue;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Define the Color userdata metatable.
static const luaL_Reg BmpFont_lib[] =
{
	{"texture", lua_BmpFontTexture},
	{"height", lua_BmpFontHeight},
	{"offsetX", lua_BmpFontOffsetX},
	{"offsetY", lua_BmpFontOffsetY},
	{"len", lua_BmpFontLen},
	{"charmap", lua_BmpFontCharMap},
	{"glyph", lua_BmpFontGlyph},

	{"print", lua_BmpFontPrint},
	{"measureText", lua_BmpFontMeasureText},

	//{"free", lua_BmpFontFree},
	{"__gc", lua_BmpFontFree},
	{"__tostring", lua_BmpFontToString},
	{NULL, NULL}
};
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Open the PSP lua BmpFont libraray.
LUAMOD_API int luaopen_BmpFont(lua_State * L)
{
	// Create the module.
	luaL_newlib(L, mod_lib);

	// Create the Color user metatable.
	luaL_newusermatatable(L, LUA_BMPFONTHANDLE, BmpFont_lib);

	// Create a dummy object.
	(*lua_pushBmpFont(L)) = NULL;
	lua_setfield(L, -2, "object");

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
