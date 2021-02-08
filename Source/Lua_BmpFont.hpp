// PSP Lua BmpFont ( FileName: Lua_BmpFont.hpp )
// ------------------------------------------------------------------------
// Header file for LuaPlayer.
// Version: 1.00
// Copyright (c) 2012 M4MSOFT
// ------------------------------------------------------------------------

#ifndef __lua_BmpFont_hpp
#define __lua_BmpFont_hpp

// Include GL font header file.
#include "glib2d.h"
#include "glibf.h"

// -----------------------------------------------------------------------------------------------------
// define the Color userdata.
typedef gfBmpFont* lua_BmpFont;

#define LUA_BMPFONTHANDLE "BmpFont"
#define lua_isBmpFont(L, index) (luaL_testudata(L, index, LUA_BMPFONTHANDLE) ? 1 : 0)
#define lua_toBmpFont(L, index) ((lua_BmpFont*)luaL_testudata(L, index, LUA_BMPFONTHANDLE))
#define luaL_checkBmpFont(L, index) ((lua_BmpFont*)luaL_checkudata(L, index, LUA_BMPFONTHANDLE))

lua_BmpFont* lua_pushBmpFont(lua_State * L);
// -----------------------------------------------------------------------------------------------------

#endif
