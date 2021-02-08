// luaplayer ( FileName: luaplayer.hpp )
// ------------------------------------------------------------------------
// Lua Header file for C++.
// <<extern "C">> not supplied automatically because Lua also compiles as C++
// Version: 1.00
// Copyright (c) 2012 M4MSOFT
// ------------------------------------------------------------------------

#ifndef __luaplayer_hpp
#define __luaplayer_hpp

extern "C" {
#include "Lua/lua.h"
#include "Lua/lualib.h"
#include "Lua/lauxlib.h"
}

// Include the Lua player library header file.
#include "Lua_Library.hpp"

// Define the collect garbage function.
#define lua_collectgarbage(L) lua_gc(L, LUA_GCCOLLECT, 0)

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define CLAMP(val, min, max) ((val)>(max)?(max):((val)<(min)?(min):(val)))

// Set a value to the current mod table.
// Arg1: The field name.
// Arg2: The field value as number.
#define SetFieldValue(fieid, val) \
    lua_pushnumber(L, val); \
    lua_setfield(L, -2, fieid);

// Set a value to the current table.
// Arg1: The field name.
// Arg2: The field value as string.
#define SetFieldString(fieid, str) \
	lua_pushstring(L, str); \
	lua_setfield(L, -2, fieid);

namespace luaplayer
{
	// return the min value of two values.
	inline int min (const int a, const int b)
	{
		return ( (a < b) ? a : b );
	}

	// return the max value of two values.
	inline int max (const int a, const int b)
	{
		return ( (a > b) ? a : b );
	}

	// return the value in the range between min and max value.
	inline int clamp (const int value, const int min, const int max)
	{
		return ( (value > max) ? max : (value < min ? min : value) );
	}

	// -----------------------------------------------------------------------------------------------------
	// Create a new usermetatable.
	//
	// @param: L - the library list.
	// @param: tname - the name of the userdata.
	// @param: l - the metatable list.
	//
	void luaL_newusermatatable(lua_State *L, const char *tname, const luaL_Reg *l);
	// -----------------------------------------------------------------------------------------------------

	// -----------------------------------------------------------------------------------------------------
	// Generates an error with a message.
	//
	// @param: L - A pointer to the current Lua_State.
	// @param narg - the index of the lua value.
	// @param tname - the name of the expected type.
	//
	// @return: This function never returns.
	//
	int luaL_typerror (lua_State *L, int narg, const char *tname);
	// -----------------------------------------------------------------------------------------------------

	// -----------------------------------------------------------------------------------------------------
	// Check if the value is a boolean value.
	//
	// @param: L - A pointer to the current Lua_State.
	// @param narg - the index of the lua value.
	//
	// @return: bool - the result as boolean.
	//
	bool luaL_checkboolean (lua_State * L, int narg);
	// -----------------------------------------------------------------------------------------------------
}

#endif
