// lua player library's ( FileName: Lua_Library.cpp )
// ------------------------------------------------------------------------
// Version: 1.00
// Copyright (c) 2012 M4MSOFT
// ------------------------------------------------------------------------

// Include the Lua header files for C++.
#include "luaplayer.hpp"

namespace luaplayer
{

	// ------------------------------------------------------------------------
	static const luaL_Reg playerlibs[] =
	{
		{LUA_DEBUGSCREEN_LIBNAME, luaopen_DebugScreen},
		{LUA_CONTROLS_LIBNAME, luaopen_Controls},

		{LUA_SYSTEM_LIBNAME, luaopen_System},
		{LUA_UTILITY_LIBNAME, luaopen_Utility},
		{LUA_POWER_LIBNAME, luaopen_Power},
		{LUA_TIMER_LIBNAME, luaopen_Timer},

		{LUA_COLOR_LIBNAME, luaopen_Color},
		{LUA_PALETTE_LIBNAME, luaopen_Palette},
		{LUA_FONT_LIBNAME, luaopen_Font},
		{LUA_BMPFONT_LIBNAME, luaopen_BmpFont},
		{LUA_TEXTURE_LIBNAME, luaopen_Texture},
		{LUA_GL_LIBNAME, luaopen_GL},

		//{LUA_AALIB_LIBNAME, luaopen_Aalib},
		{LUA_SOUND_LIBNAME, luaopen_Sound},

		{NULL, NULL}
	};
	// ------------------------------------------------------------------------

	// ------------------------------------------------------------------------
	// Open all Lua player libraries.
	LUALIB_API void (luaL_openplayerlibs) (lua_State *L)
	{
		const luaL_Reg *lib;
		// call open functions from 'playerlibslibs' and set results to global table
		for (lib = playerlibs; lib->func; lib++)
		{
			luaL_requiref(L, lib->name, lib->func, 1);
			lua_pop(L, 1);  /* remove lib */
		}
	}
	// ------------------------------------------------------------------------

}
