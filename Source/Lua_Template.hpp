// PSP Lua Template ( FileName: Lua_Template.hpp )
// ------------------------------------------------------------------------
// Header file for LuaPlayer.
// Version: 1.00
// Copyright (c) 2012 M4MSOFT
// ------------------------------------------------------------------------

#ifndef __lua_Template_hpp
#define __lua_Template_hpp

// -----------------------------------------------------------------------------------------------------
// define the Color userdata.
typedef Template lua_Template;

#define LUA_TEMPLATEHANDLE "Template"
#define lua_isTemplate(L, index) (luaL_testudata(L, index, LUA_TEMPLATEHANDLE) ? 1 : 0)
#define lua_toTemplate(L, index) ((lua_Template*)luaL_testudata(L, index, LUA_TEMPLATEHANDLE))
#define luaL_checkTemplate(L, index) ((lua_Template*)luaL_checkudata(L, index, LUA_TEMPLATEHANDLE))

lua_Template* lua_pushTemplate(lua_State * L);
// -----------------------------------------------------------------------------------------------------

#endif
