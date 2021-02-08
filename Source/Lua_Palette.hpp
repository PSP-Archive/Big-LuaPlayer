// PSP Lua Palette ( FileName: Lua_Palette.hpp )
// ------------------------------------------------------------------------
// Header file for LuaPlayer.
// Version: 1.00
// Copyright (c) 2012 M4MSOFT
// ------------------------------------------------------------------------

#ifndef __lua_Palette_hpp
#define __lua_Palette_hpp

// Include the PSP gLib2D Header file.
#include "glib2d.h"

// -----------------------------------------------------------------------------------------------------
// define the Color userdata.
typedef g2dPalette* lua_Palette;

#define LUA_PALETTEHANDLE "Palette"
#define lua_isPalette(L, index) (luaL_testudata(L, index, LUA_PALETTEHANDLE) ? 1 : 0)
#define lua_toPalette(L, index) ((lua_Palette*)luaL_testudata(L, index, LUA_PALETTEHANDLE))
#define luaL_checkPalette(L, index) ((lua_Palette*)luaL_checkudata(L, index, LUA_PALETTEHANDLE))

lua_Palette* lua_pushPalette(lua_State * L);
// -----------------------------------------------------------------------------------------------------

#endif
