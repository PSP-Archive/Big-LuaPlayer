// PSP Lua Texture ( FileName: Lua_Texture.cpp )
// ------------------------------------------------------------------------
// Version: 1.00
// Copyright (c) 2012 M4MSOFT
// ------------------------------------------------------------------------

// Include C/C++ header files
#include <malloc.h>

// Include the Lua header files for C++.
#include "luaplayer.hpp"
#include "Lua_Texture.hpp"
#include "Lua_Palette.hpp"
#include "Lua_Color.hpp"

// Select the luaplayer objects to use in this file.
using luaplayer::luaL_newusermatatable;
using luaplayer::clamp;

// -----------------------------------------------------------------------------------------------------
// Define the function prototypes
static int lua_TextureNew (lua_State * L);
static int lua_TextureLoad (lua_State * L);

static int lua_TextureBegin (lua_State * L);
static int lua_TextureAdd (lua_State * L);
static int lua_TextureEnd (lua_State * L);
static int lua_TextureRender (lua_State * L);

static int lua_TexturePalette (lua_State * L);
static int lua_TextureWidth (lua_State * L);
static int lua_TextureHeight (lua_State * L);
static int lua_TextureIsSwizzled (lua_State * L);
static int lua_TextureToRam (lua_State * L);
static int lua_TextureToVram (lua_State * L);
static int lua_TextureSwizzle (lua_State * L);
static int lua_TextureUnSwizzle (lua_State * L);

static int lua_TextureBlit (lua_State * L);
static int lua_TextureClear (lua_State * L);
static int lua_TexturePixel (lua_State * L);
static int lua_TextureDrawLine (lua_State * L);
static int lua_TextureDrawRect (lua_State * L);
static int lua_TextureFillRect (lua_State * L);

static int lua_TextureSave (lua_State * L);

static int lua_TextureFree (lua_State * L);
static int lua_TextureToString(lua_State * L);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Load a texture from a file.
static int lua_TextureNew(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg < 2 || narg > 4) return luaL_error(L, "Texture.new(w, h, [vram [,format]]): takes 2 to 4 arguments.");
	lua_gc(L, LUA_GCCOLLECT, 0);

	// Load the texture.
	g2dTexture *src = g2dTexCreate(
		luaL_checkint(L, 1), luaL_checkint(L, 2),
		(narg > 3 ? clamp(luaL_checkint(L, 4), 2, 5) : G2D_PSM_8888),
		(narg > 2 ? lua_toboolean(L, 3) : false)
	);

	// Return the Texture.
	if (src)
		(*lua_pushTexture(L)) = src;
	else
		lua_pushnil(L);

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Load a texture from a file.
static int lua_TextureLoad(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg < 1 && narg > 4) return luaL_error(L, "Texture.load(path [vram [,format [,swizzle]]]): takes one to four arguments.");
	lua_gc(L, LUA_GCCOLLECT, 0);

	// Load the texture.
	g2dTexture *src = g2dTexLoad(
		luaL_checkstring(L, 1),
		(narg > 2 ? clamp(luaL_checkint(L, 3), 2, 5) : G2D_PSM_8888),
		(narg > 1 ? lua_toboolean(L, 2) : false),
	    (g2dTex_Mode) (narg < 4 ? G2D_SWIZZLE : lua_toboolean(L, 4) ? G2D_SWIZZLE : G2D_VOID)
	);

	// Return the Texture.
	if (src)
		(*lua_pushTexture(L)) = src;
	else
		lua_pushnil(L);

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
static int lua_TextureBegin(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg < 1 || narg > 3) return luaL_error(L, "Texture.Begin(texture [,linear [,integer]]): takes one to three arguments.");

	// Begin texture rendering.
	g2dBeginRects(*luaL_checkTexture(L, 1));
	if (narg == 2) g2dSetTexLinear(lua_toboolean(L, 2));
	if (narg == 3) g2dSetCoordInteger(lua_toboolean(L, 3));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_TextureAdd(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);

	// Remove the texture from the stack.
	if (narg > 0 && lua_isTexture(L, 1))
		{lua_remove(L, 1); narg--;}

	// Select the arguments.
	switch (narg)
	{
		case 10 : g2dSetColor(*luaL_checkColor(L, 10));
		case 9 :
			g2dSetCropWH(luaL_checknumber(L, 7), luaL_checknumber(L, 8));
			g2dSetCropXY(luaL_checknumber(L, 5), luaL_checknumber(L, 6));
			g2dSetScaleWH(luaL_checknumber(L, 3), luaL_checknumber(L, 4));
			g2dSetCoordXYZ(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 9));
			break;
		case 8 : g2dSetCropWH(luaL_checknumber(L, 7), luaL_checknumber(L, 8));
		case 6 : g2dSetCropXY(luaL_checknumber(L, 5), luaL_checknumber(L, 6));
		case 4 : g2dSetScaleWH(luaL_checknumber(L, 3), luaL_checknumber(L, 4));
		case 2 : g2dSetCoordXY(luaL_checknumber(L, 1), luaL_checknumber(L, 2)); break;
		default: return luaL_error(L, "Texture.Add([texture], x, y [,w ,h [,sx, sy [,sw ,sh [,z [,color]]]]]): takes 2, 4, 6, 8, 9 or 10 arguments.");
	}
	g2dAdd();

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_TextureEnd(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg > 1) return luaL_error(L, "Texture.End([texture]): takes zero or one argument.");

	// End texture rendering.
	g2dEnd();

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_TextureRender(lua_State * L) //Quick Draw the texcture function
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg < 1) return luaL_error(L, "Texture._Begin(texture, ...): takes one ore more arguments.");

	// Begin texture rendering.
	g2dBeginRects(*luaL_checkTexture(L, 1));
	// Add the texture.
	lua_TextureAdd(L);
	// End texture rendering.
	g2dEnd();

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
static int lua_TexturePalette (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1 && narg != 2) return luaL_error(L, "Texture.palette(texture [,plt]) takes one or two arguments.");

	// Get the texture Pointer.
	g2dTexture *image = *luaL_checkTexture(L, 1);

	// Get the current texture palette Pointer.
	lua_Palette pltPtr = image->plt;

	if (narg == 2){
		if (lua_isPalette(L, 2))
		{
			// Link the Palette to the texture.
			lua_Palette pltPtrLink = *lua_toPalette(L, 2);
			if (g2dpaletteAddLink(pltPtrLink)) return luaL_error(L, "Texture.palette: can't link the palette.");
			image->plt = pltPtrLink;
		}
		else
			// Remove the palette.
			image->plt = NULL;
		// Return and link the Palette to lua.
		if (!g2dpaletteAddLink(pltPtr)) // Only default or link palettes for lua.
			(*lua_pushPalette(L)) = pltPtr;
		else
			lua_pushnil(L);
		// Free the Palette Pointer.
		g2dPaletteFree(&pltPtr);
	}
	else
		// Return and link the Palette to lua.
		if (!g2dpaletteAddLink(pltPtr))
			(*lua_pushPalette(L)) = pltPtr;
		else
			lua_pushnil(L);

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_TextureWidth(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "Texture.width(texture) takes one argument.");

	// Return the width of the texture.
	lua_pushinteger(L, (*luaL_checkTexture(L, 1))->w);

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_TextureHeight(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "Texture.height(texture) takes one argument");

	// Return the width of the texture.
	lua_pushinteger(L, (*luaL_checkTexture(L, 1))->h);

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_TextureIsSwizzled(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "Texture.isSwizzled(texture) takes one argument.");

		g2dTexture *image = *luaL_checkTexture(L, 1);
		if (image == NULL)
			return luaL_error(L, "texture:isSwizzled(): cannot set 'texture'.");

		if (image->swizzled == true)
			lua_pushboolean(L, true);
		else
			lua_pushboolean(L, false);

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Move the Texture the the ram memory.
static int lua_TextureToRam (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "Texture.toRam(texture) takes one argument.");

	// Move the Texture the the ram.
	lua_pushboolean(L, !g2dTexToRam(*luaL_checkTexture(L, 1)));

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Move the Texture the the vram memory.
static int lua_TextureToVram (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "Texture.toVram(texture) takes one argument.");

	// Move the Texture the the vram.
	lua_pushboolean(L, !g2dTexToVram(*luaL_checkTexture(L, 1)));

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Swizzle the texture data.
static int lua_TextureSwizzle (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "Texture.swizzle(texture) takes one argument.");

	// Swizzle the texture data.
	lua_pushboolean(L, !g2dTexSwizzle(*luaL_checkTexture(L, 1)));

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// UnSwizzle the texture data.
static int lua_TextureUnSwizzle (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "Texture.unSwizzle(texture) takes one argument.");

	// UnSwizzle the texture data.
	lua_pushboolean(L, !g2dTexUnSwizzle(*luaL_checkTexture(L, 1)));

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
static int lua_TextureBlit (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);

	int sx,sy,sw,sh;
	g2dTexture *src = NULL;
	bool blend = false;
	sx = 0; sy = 0;

	// Select the arguments.
	switch (narg)
	{
	    case 9 :
			blend = lua_toboolean(L, 9);
		case 8 :
			sw = luaL_checkint(L, 7); sh = luaL_checkint(L, 8);
			sx = luaL_checkint(L, 5); sy = luaL_checkint(L, 6);
			src = *luaL_checkTexture(L, 4);
			break;
		case 6 :
			sx = luaL_checkint(L, 5); sy = luaL_checkint(L, 6);
		case 4 :
			src = *luaL_checkTexture(L, 4);
			sw = src->w; sh = src->h;
			break;
		default:
			return luaL_error(L, "Texture.blit(destiny, dX, dY, source [,sX ,sY [,sW ,sH [,blend]]]) takes 4,6,8 or 9 arguments.");
	}

	// Draw the texture.
	g2dTexBlitToTex(*luaL_checkTexture(L, 1), luaL_checkint(L, 2), luaL_checkint(L, 3), src, sx, sy, sw, sh, blend);

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_TextureClear (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);

	// Clear the texture.
	switch (narg)
	{
		case 1 :
			g2dTexClear(*luaL_checkTexture(L, 1), 0xffffffff, false); break;
		case 2 :
			g2dTexClear(*luaL_checkTexture(L, 1), *luaL_checkColor(L, 2), false); break;
		case 3 :
			g2dTexClear(*luaL_checkTexture(L, 1), *luaL_checkColor(L, 2), lua_toboolean(L, 3)); break;
		default:
			return luaL_error(L, "Texture.clear(texture [,color [, blend]]) takes one to three arguments.");
	}

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Get or set the pixel color.
static int lua_TexturePixel (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);

	int x,y;
	g2dTexture *image = NULL;
	bool blend = false;

	// Select the arguments.
	switch (narg)
	{
		case 5 :
			blend = lua_toboolean(L, 4);
		case 4 : // Set the pixel.
			image = *luaL_checkTexture(L, 1);
			x = luaL_checkint(L, 2); y = luaL_checkint(L, 3);
			g2dTexSetPixel(image, x, y, *luaL_checkColor(L, 4), blend);
			break;
		case 3 :
			image = *luaL_checkTexture(L, 1);
			x = luaL_checkint(L, 2); y = luaL_checkint(L, 3);
			break;
		default:
			return luaL_error(L, "Texture.pixel(texture, x, y [,color [,blend]]) takes three to five arguments.");
	}

	// Return the color of the pixel.
	(*lua_pushColor(L)) = g2dTexGetPixel(image, x, y);

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_TextureDrawLine (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 6 && narg != 7) return luaL_error(L, "Texture.drawLine(texture, x0, y0, x1, y1, color [,blend]]) takes six or seven arguments.");

	// Draw the line.
	g2dTexDrawLine(*luaL_checkTexture(L, 1), luaL_checkint(L, 2), luaL_checkint(L, 3),
		luaL_checkint(L, 4), luaL_checkint(L, 5), *luaL_checkColor(L, 6),
		(narg == 7 ? lua_toboolean(L, 7) : false));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_TextureDrawRect (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 6 && narg != 7) return luaL_error(L, "Texture.drawRect(texture, x, y, w, h, color [,blend]]) takes six or seven arguments.");

	// Fill the rectangle.
	g2dTexDrawRect(*luaL_checkTexture(L, 1), luaL_checkint(L, 2), luaL_checkint(L, 3),
		luaL_checkint(L, 4), luaL_checkint(L, 5), *luaL_checkColor(L, 6),
		(narg == 7 ? lua_toboolean(L, 7) : false));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_TextureFillRect (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 6 && narg != 7) return luaL_error(L, "Texture.fillRect(texture, x, y, w, h, color [,blend]]) takes six to seven arguments.");

	// Draw the rectangle.
	g2dTexFillRect(*luaL_checkTexture(L, 1), luaL_checkint(L, 2), luaL_checkint(L, 3),
		luaL_checkint(L, 4), luaL_checkint(L, 5), *luaL_checkColor(L, 6),
		(narg == 7 ? lua_toboolean(L, 7) : false));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Save the texture.
static int lua_TextureSave(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg < 2 && narg > 4) return luaL_error(L, "Texture.save(texture, filename [,saveAlpha [,usePalette]]): takes two to four arguments.");

    // Save the texture.
	g2dTexSave(luaL_checkstring(L, 2), *luaL_checkTexture(L, 1),
			  (narg > 2 ? lua_toboolean(L, 3) : true),
			  (narg > 3 ? lua_toboolean(L, 4) : true));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Free a texture.
static int lua_TextureFree(lua_State * L)
{
	// Get the texture Pointer.
	lua_Texture *texPtr = lua_toTexture(L, 1);

	// Free the texture Pointer and set the Pointer to NULL.
	if (texPtr && *texPtr)
		g2dTexFree(texPtr);

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_TextureToString(lua_State * L)
{
	// Get the texture Pointer.
	lua_Texture *texPtr = lua_toTexture(L, 1);

	// Return the texture info string.
	if (texPtr && *texPtr)
		lua_pushfstring(L, "Texture: (%p) [%d, %d, %d]", (*texPtr)->data, (*texPtr)->w, (*texPtr)->h, (*texPtr)->psm);
	else
		lua_pushstring(L, "nil");

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Define the module table.
static const luaL_Reg mod_lib[] =
{
	{"new", lua_TextureNew},
	{"load", lua_TextureLoad},

	{"Begin", lua_TextureBegin},
	{"Add", lua_TextureAdd},
	{"End", lua_TextureEnd},
	{"render", lua_TextureRender},

	{"palette", lua_TexturePalette},
	{"width", lua_TextureWidth},
	{"height",lua_TextureHeight},
	{"isSwizzled", lua_TextureIsSwizzled},
	{"swizzle", lua_TextureSwizzle},
	{"unSwizzled", lua_TextureUnSwizzle},
	{"toRam", lua_TextureToRam},
	{"toVram", lua_TextureToVram},

	{"blit", lua_TextureBlit},
	{"clear", lua_TextureClear},
	{"pixel", lua_TexturePixel},
	{"drawLine", lua_TextureDrawLine},
	{"drawRect", lua_TextureDrawRect},
	{"fillRect", lua_TextureFillRect},

	{"save", lua_TextureSave},
	//{"free", lua_TextureFree},
	{NULL, NULL}
};
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Push the Texture userdata.
lua_Texture* lua_pushTexture(lua_State * L)
{
	lua_Texture* newvalue = (lua_Texture*) lua_newuserdata(L, sizeof(lua_Texture));
	luaL_setmetatable(L, LUA_TEXTUREHANDLE);
	return newvalue;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Define the Texture userdata metatable.
static const luaL_Reg Texture_lib[] =
{
	{"Begin", lua_TextureBegin},
	{"Add", lua_TextureAdd},
	{"End", lua_TextureEnd},
	{"render", lua_TextureRender},

	{"palette", lua_TexturePalette},
	{"width", lua_TextureWidth},
	{"height",lua_TextureHeight},
	{"isSwizzled", lua_TextureIsSwizzled},
	{"swizzle", lua_TextureSwizzle},
	{"unSwizzle", lua_TextureUnSwizzle},
	{"toRam", lua_TextureToRam},
	{"toVram", lua_TextureToVram},

	{"blit", lua_TextureBlit},
	{"clear", lua_TextureClear},
	{"pixel", lua_TexturePixel},
	{"drawLine", lua_TextureDrawLine},
	{"drawRect", lua_TextureDrawRect},
	{"fillRect", lua_TextureFillRect},

	{"save", lua_TextureSave},
	//{"free", lua_TextureFree},

	{"__gc", lua_TextureFree},
	{"__tostring", lua_TextureToString},
	{NULL, NULL}
};
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Open the PSP lua Texture libraray.
LUAMOD_API int luaopen_Texture(lua_State * L)
{
	// Create the module.
	luaL_newlib(L, mod_lib);

	// Create the Texture user metatable.
	luaL_newusermatatable(L, LUA_TEXTUREHANDLE, Texture_lib);

	// Create a dummy object.
	(*lua_pushTexture(L)) = NULL;
	lua_setfield(L, -2, "object");

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
