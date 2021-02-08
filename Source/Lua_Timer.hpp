// PSP Lua Timer ( FileName: Lua_Timer.hpp )
// ------------------------------------------------------------------------
// Header file for PSP Lua Timer.
// Version: 1.00
// Copyright (c) 2012 M4MSOFT
// ------------------------------------------------------------------------

// Include C/C++ header files
#include <ctime>

#ifndef __lua_Timer_hpp
#define __lua_Timer_hpp

namespace LuaTimer
{
	// -----------------------------------------------------------------------------------------------------
	// Define the Timer structure.
	struct timer {float measuredTime, offset, current, last;};
	// measuredTime is the value of startTime.
	// offset is the offset value of startTime.
	// current is the value of the last call to getCurrentMilliseconds. (used for the delta Function)
	// last is the value current before the the last call to getCurrentMilliseconds. (used for the delta Function)
	// -----------------------------------------------------------------------------------------------------

	// -----------------------------------------------------------------------------------------------------
	// Define the Inline Function.
	inline float getCurrentMilliseconds() {return clock() / float(CLOCKS_PER_SEC / 1000);}
	inline float getDelta(float a, float b) {return (a > b) ? (a - b) : (b - a);}
	// -----------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------
// define the Timer userdata.
typedef LuaTimer::timer* lua_Timer;

#define LUA_TIMERHANDLE "Timer"
#define lua_isTimer(L, index) (luaL_testudata(L, index, LUA_TEXTUREHANDLE) ? 1 : 0)
#define lua_toTimer(L, index) ((lua_Timer*)luaL_testudata(L, index, LUA_TIMERHANDLE))
#define luaL_checkTimer(L, index) ((lua_Timer*)luaL_checkudata(L, index, LUA_TIMERHANDLE))

lua_Timer* lua_pushTimer(lua_State * L);
// -----------------------------------------------------------------------------------------------------

#endif
