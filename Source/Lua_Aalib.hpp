// PSP Lua Aalib ( FileName: Lua_Aalib.hpp )
// ------------------------------------------------------------------------
// Header file for LuaPlayer.
// Version: 1.00
// Copyright (c) 2012 M4MSOFT
// ------------------------------------------------------------------------

#ifndef __lua_Aalib_hpp
#define __lua_Aalib_hpp

// -----------------------------------------------------------------------------------------------------
// define the Sound userdata.
typedef int lua_Channel;

#define LUA_CHANNELHANDLE "Channel"
#define lua_isChannel(L, index) (luaL_testudata(L, index, LUA_CHANNELHANDLE) ? 1 : 0)
#define lua_toChannel(L, index) ((lua_Channel*)luaL_testudata(L, index, LUA_CHANNELHANDLE))
#define luaL_checkChannel(L, index) ((lua_Channel*)luaL_checkudata(L, index, LUA_CHANNELHANDLE))

lua_Channel* lua_pushChannel(lua_State * L);
// -----------------------------------------------------------------------------------------------------

#endif
