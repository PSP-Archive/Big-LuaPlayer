// PSP Lua Texture ( FileName: Lua_Texture.hpp )
// ------------------------------------------------------------------------
// Header file for LuaPlayer.
// Version: 1.00
// Copyright (c) 2012 M4MSOFT
// ------------------------------------------------------------------------

#ifndef __lua_Texture_hpp
#define __lua_Texture_hpp

// Include the PSP gLib2D Header file.
#include "glib2d.h"

// -----------------------------------------------------------------------------------------------------
// define the Texture userdata.
typedef g2dTexture* lua_Texture;

#define LUA_TEXTUREHANDLE "Texture"
#define lua_isTexture(L, index) (luaL_testudata(L, index, LUA_TEXTUREHANDLE) ? 1 : 0)
#define lua_toTexture(L, index) ((lua_Texture*)luaL_testudata(L, index, LUA_TEXTUREHANDLE))
#define luaL_checkTexture(L, index) ((lua_Texture*)luaL_checkudata(L, index, LUA_TEXTUREHANDLE))

lua_Texture* lua_pushTexture(lua_State * L);
// -----------------------------------------------------------------------------------------------------

#endif
