// luaplayer ( FileName: luaplayer.cpp )
// ------------------------------------------------------------------------
// <<extern "C">> not supplied automatically because Lua also compiles as C++
// Version: 1.01
// Copyright (c) 2014 M4MSOFT
// ------------------------------------------------------------------------

// Include C/C++ header files
#include <ctype.h>
#include <string.h>

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

	// -----------------------------------------------------------------------------------------------------
	double luaL_tonumber (lua_State *L, int narg, int base)
	{
		#define SPACECHARS	" \f\n\r\t\v"
		size_t l;
		const char *s = luaL_checklstring(L, narg, &l);
		const char *e = s + l; // end point for 's'
		int neg = 0;
		if (2 > base && base > 36) return luaL_error(L, "Argument error: tonumber(v [,x]) - base out of range");
		s += strspn(s, SPACECHARS); // skip initial spaces
		if (*s == '-') { s++; neg = 1; } // handle signal
		else if (*s == '+') s++;
		if (base == 16 && (s[0] == '0' && (s[1] == 'x' || s[1] == 'X'))) s+=2; // skip initial space for hex numbers
		if (isalnum((unsigned char)*s)){
			double n = 0;
			do{
				int digit = (isdigit((unsigned char)*s)) ? *s - '0'
					: toupper((unsigned char)*s) - 'A' + 10;
				if (digit >= base) break; // invalid numeral; force a fail
				n = n * (double)base + (double)digit;
				s++;
			}
			while (isalnum((unsigned char)*s)); // skip trailing spaces
				s += strspn(s, SPACECHARS);
			if (s == e){ // no invalid trailing characters?
				return ((neg) ? -n : n);
			} // else not a number
		} // else not a number
		return luaL_error(L, "Argument error: tonumber(v [,x]) - value can't be converted to a number.");
	}
	// -----------------------------------------------------------------------------------------------------
}
