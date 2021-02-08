// PSP Lua GL3D ( FileName: Lua_GL3D.hpp )
// ------------------------------------------------------------------------
// Header file for LuaPlayer.
// Version: 1.00
// Copyright (c) 2014 M4MSOFT
// ------------------------------------------------------------------------

#ifndef __lua_GL3D_hpp
#define __lua_GL3D_hpp

// Include the PSP gl3dVertex Header file.
#include "gl3dVertex.h"

// -----------------------------------------------------------------------------------------------------
// define the vertex userdata.
typedef gumVertices* lua_Vertices;

#define LUA_VERTICESHANDLE "Vertices"
#define lua_isVertices(L, index) (luaL_testudata(L, index, LUA_VERTICESHANDLE) ? 1 : 0)
#define lua_toVertices(L, index) ((lua_Vertices*)luaL_testudata(L, index, LUA_VERTICESHANDLE))
#define luaL_checkVertex(L, index) ((lua_Vertices*)luaL_checkudata(L, index, LUA_VERTICESHANDLE))

lua_Vertices* lua_pushVertices(lua_State * L);
// -----------------------------------------------------------------------------------------------------

#endif
