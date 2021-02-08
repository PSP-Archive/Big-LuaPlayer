// PSP Lua Sound ( FileName: Lua_Sound.hpp )
// ------------------------------------------------------------------------
// Header file for LuaPlayer.
// Version: 1.00
// Copyright (c) 2012 M4MSOFT
// ------------------------------------------------------------------------

#ifndef __lua_Sound_hpp
#define __lua_Sound_hpp

// -----------------------------------------------------------------------------------------------------
// define the Voice userdata.
typedef sound_object lua_Voice;

#define LUA_VOICEHANDLE "Voice"
#define lua_isVoice(L, index) (luaL_testudata(L, index, LUA_VOICEHANDLE) ? 1 : 0)
#define lua_toVoice(L, index) ((lua_Voice*)luaL_testudata(L, index, LUA_VOICEHANDLE))
#define luaL_checkVoice(L, index) ((lua_Voice*)luaL_checkudata(L, index, LUA_VOICEHANDLE))

lua_Voice* lua_pushVoice(lua_State * L);
// -----------------------------------------------------------------------------------------------------

#endif
