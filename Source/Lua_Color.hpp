// PSP Lua Color ( FileName: Lua_Color.hpp )
// ------------------------------------------------------------------------
// Header file for LuaPlayer.
// Version: 1.00
// Copyright (c) 2012 M4MSOFT
// ------------------------------------------------------------------------

#ifndef __lua_Color_hpp
#define __lua_Color_hpp

// Include the PSP gLib2D Header file.
#include "glib2d.h"

// -----------------------------------------------------------------------------------------------------
// define the Color userdata.
typedef g2dColor lua_Color;

#define LUA_COLORHANDLE "Color"
#define lua_isColor(L, index) (luaL_testudata(L, index, LUA_COLORHANDLE) ? 1 : 0)
#define lua_toColor(L, index) ((lua_Color*)luaL_testudata(L, index, LUA_COLORHANDLE))
#define luaL_checkColor(L, index) ((lua_Color*)luaL_checkudata(L, index, LUA_COLORHANDLE))

lua_Color* lua_pushColor(lua_State * L);
// -----------------------------------------------------------------------------------------------------

#endif
