// PSP Lua Font ( FileName: Lua_Font.hpp )
// ------------------------------------------------------------------------
// Header file for LuaPlayer.
// Version: 1.00
// Copyright (c) 2012 M4MSOFT
// ------------------------------------------------------------------------

#ifndef __lua_Font_hpp
#define __lua_Font_hpp

// Include GL font header file.
#include "glib2d.h"
#include "glibf.h"

// -----------------------------------------------------------------------------------------------------
// define the Font userdata.
typedef gfFont* lua_Font;

#define LUA_FONTHANDLE "Font"
#define lua_isFont(L, index) (luaL_testudata(L, index, LUA_FONTHANDLE) ? 1 : 0)
#define lua_toFont(L, index) ((lua_Font*)luaL_testudata(L, index, LUA_FONTHANDLE))
#define luaL_checkFont(L, index) ((lua_Font*)luaL_checkudata(L, index, LUA_FONTHANDLE))

lua_Font* lua_pushFont(lua_State * L);
// -----------------------------------------------------------------------------------------------------

#endif
