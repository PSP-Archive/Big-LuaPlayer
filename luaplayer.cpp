// luaplayer ( FileName: luaplayer.cpp )
// ------------------------------------------------------------------------
// <<extern "C">> not supplied automatically because Lua also compiles as C++
// Version: 1.00
// Copyright (c) 2012 M4MSOFT
// ------------------------------------------------------------------------

// Include C/C++ header files
//#include <cstring>

// Include the Lua header files for C++.
#include "luaplayer.hpp"

namespace luaplayer
{

	// -----------------------------------------------------------------------------------------------------
    // Create a new usermetatable.
	void luaL_newusermatatable(lua_State *L, const char *tname, const luaL_Reg *l)
	{
		//if (!luaL_newmetatable(L, tname))  /* create metatable for file handles */
		//{
		//	lua_pop(L, 1);  /* pop new metatable */
		//	luaL_error(L, "The type " LUA_QS "already exits.", tname);
		//}
		luaL_newmetatable(L, tname);
		lua_pushvalue(L, -1);  /* push metatable */
		lua_setfield(L, -2, "__index");  /* metatable.__index = metatable */
		luaL_setfuncs(L, l, 0);  /* add file methods to new metatable */
		lua_pop(L, 1);  /* pop new metatable */
	}
	// -----------------------------------------------------------------------------------------------------

	// -----------------------------------------------------------------------------------------------------
	// Generates an error with a message.
	int luaL_typerror (lua_State *L, int narg, const char *tname)
	{
		const char *msg = lua_pushfstring(L, "%s expected, got %s", tname, luaL_typename(L, narg));
		return luaL_argerror(L, narg, msg);  // return the error code.
	}
	// -----------------------------------------------------------------------------------------------------

	// -----------------------------------------------------------------------------------------------------
	// Check if the value is a boolean value.
	bool luaL_checkboolean (lua_State * L, int narg)
	{
		if (lua_isboolean(L, narg))
			return lua_toboolean(L, narg); // return the boolean value.
		else
			return luaL_typerror(L, narg, lua_typename(L, LUA_TBOOLEAN)); // return the error code.
	}
	// -----------------------------------------------------------------------------------------------------
}
