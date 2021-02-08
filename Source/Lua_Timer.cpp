// PSP Lua Timer ( FileName: Lua_Timer.cpp )
// ------------------------------------------------------------------------
// Version: 1.00
// Copyright (c) 2012 M4MSOFT
// ------------------------------------------------------------------------

// Include C/C++ header files
#include <malloc.h>

// Include the Lua header files for C++.
#include "luaplayer.hpp"
#include "Lua_Timer.hpp"

// Select the luaplayer objects to use in this file.
using luaplayer::luaL_newusermatatable;

// the lue timer objects to use in this file.
using namespace LuaTimer;

// -----------------------------------------------------------------------------------------------------
// Define the function prototypes
static int lua_TimerNew (lua_State * L);
static int lua_TimerStart (lua_State * L);
static int lua_TimerUpdate (lua_State * L);
static int lua_TimerPeekDelta (lua_State * L);
static int lua_TimerDelta (lua_State * L);
static int lua_TimerTime (lua_State * L);
static int lua_TimerStop (lua_State * L);
static int lua_TimerReset (lua_State * L);
static int lua_TimerFree (lua_State * L);
static int lua_TimerToString (lua_State * L);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Create a new Time. Set the start time and start the timer.
static int lua_TimerNew (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg > 1) return luaL_error(L, "Argument error: Timer.new([startTime]) takes zero or one arguments.");

	// Create the new timer.
	lua_Timer tmrPtr = (lua_Timer) malloc(sizeof(timer));
	if (tmrPtr == NULL) return luaL_error(L, "Timer.new: out of memory.");

	// Set the new Timer
	tmrPtr->measuredTime = getCurrentMilliseconds();
	tmrPtr->offset = narg ? luaL_checknumber(L, 1) : 0;
	tmrPtr->current = tmrPtr->measuredTime;
	tmrPtr->last = 0;

	// Return the Timer.
	(*lua_pushTimer(L)) = tmrPtr;

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Start the timer and return the current time.
static int lua_TimerStart (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1)  return luaL_error(L, "Argument error: Timer.start(timer) takes one argument.");

	// Get the Timer Pointer.
	lua_Timer tmrPtr = *luaL_checkTimer(L, 1);

	// Update the timer
	tmrPtr->last = tmrPtr->current;
	tmrPtr->current = getCurrentMilliseconds();

	if (tmrPtr->measuredTime)
		// Return the time.
		lua_pushnumber(L, tmrPtr->current - tmrPtr->measuredTime + tmrPtr->offset);
	else
	{
		// Start the Timer
		tmrPtr->measuredTime = tmrPtr->current;
		// Return the offset.
		lua_pushnumber(L, tmrPtr->offset);
	}

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Reset and start the timer. return the start time.
static int lua_TimerUpdate (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1 && narg != 2)  return luaL_error(L, "Argument error: Timer.update(timer, [startTime]) takes one or two arguments.");

	// Get the Timer Pointer.
	lua_Timer tmrPtr = *luaL_checkTimer(L, 1);

	// Reset the Timer
	tmrPtr->measuredTime = getCurrentMilliseconds();
	tmrPtr->offset = narg == 2 ? luaL_checknumber(L, 1) : 0;
	tmrPtr->current = tmrPtr->measuredTime;
	tmrPtr->last = 0;

	// Return the offset.
	lua_pushnumber(L, tmrPtr->offset);

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Return delta in ms between to timer calls without updating the timer object.
static int lua_TimerPeekDelta (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1)  return luaL_error(L, "Argument error: Timer.peekDelta(timer) takes one argument.");

	// Get the Timer Pointer.
	lua_Timer tmrPtr = *luaL_checkTimer(L, 1);

	// Return delta in ms.
	lua_pushnumber(L, getDelta(tmrPtr->current, tmrPtr->last));

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Return delta in ms between to timer calls.
static int lua_TimerDelta (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1)  return luaL_error(L, "Argument error: Timer.delta(timer) takes one argument.");

	// Get the Timer Pointer.
	lua_Timer tmrPtr = *luaL_checkTimer(L, 1);

	if (tmrPtr->measuredTime)
	{
		// Update the timer
		tmrPtr->last = tmrPtr->current;
		tmrPtr->current = getCurrentMilliseconds();
	}

	// Return delta in ms.
	lua_pushnumber(L, getDelta(tmrPtr->current, tmrPtr->last));

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Return the current time.
static int lua_TimerTime (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "Argument error: Timer.time(timer) takes one argument.");

	// Get the Timer Pointer.
	lua_Timer tmrPtr = *luaL_checkTimer(L, 1);

	if (tmrPtr->measuredTime)
	{
		// Update the timer
		// tmrPtr->last = tmrPtr->current;
		tmrPtr->current = getCurrentMilliseconds();
		// Return the time.
		lua_pushnumber(L, tmrPtr->current - tmrPtr->measuredTime + tmrPtr->offset);
	}
	else
		// Return the time.
		lua_pushnumber(L, tmrPtr->offset);

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Stop the timer and return the stop time.
static int lua_TimerStop (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "Argument error: Timer.stop(timer) takes one argument.");

	// Get the Timer Pointer.
	lua_Timer tmrPtr = *luaL_checkTimer(L, 1);

	if (tmrPtr->measuredTime)
	{
		// Stop the Timer.
		tmrPtr->current = getCurrentMilliseconds();
		tmrPtr->offset = tmrPtr->current - tmrPtr->measuredTime + tmrPtr->offset;
		tmrPtr->measuredTime = 0;
	}

	// Return the time.
	lua_pushnumber(L, tmrPtr->offset);

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Reset the timer and return the stop time.
static int lua_TimerReset (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1 && narg != 2) luaL_error(L, "Argument error: Timer.reset(timer, [startTime]) takes one or two arguments.");

	// Get the Timer Pointer.
	lua_Timer tmrPtr = *luaL_checkTimer(L, 1);

	if (tmrPtr->measuredTime)
	{
		// timer is running
		tmrPtr->current = getCurrentMilliseconds();
		// Return the time.
		lua_pushnumber(L, tmrPtr->current - tmrPtr->measuredTime + tmrPtr->offset);
	}
	else
		// Return the time.
		lua_pushnumber(L, tmrPtr->offset);


	// Stop the timer and set the offset
	tmrPtr->offset = narg == 2 ? luaL_checknumber(L, 2) : 0;
	tmrPtr->measuredTime = 0;

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Free the timer object.
static int lua_TimerFree (lua_State * L)
{
	// Get the Timer Pointer.
	lua_Timer *tmrPtr = lua_toTimer(L, 1);

	// Free the Timer Pointer and set the Pointer to NULL.
	if (tmrPtr && *tmrPtr)
	{
		free(*tmrPtr);
		*tmrPtr = NULL;
	}

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Convert the timer to a string.
static int lua_TimerToString (lua_State * L)
{
	// Get the Timer Pointer.
	lua_Timer *tmrPtr = lua_toTimer(L, 1);

	// Return the Timer info string.
	if (tmrPtr && *tmrPtr)
	{
		lua_TimerTime(L); // Push the current time on the stack.
		lua_TimerPeekDelta(L); // Push the current delta on the stack.
		lua_pushfstring(L, "Timer (%p) [%d ms, %d T]", *tmrPtr, lua_tonumber(L, 2), lua_tonumber(L, 3));
		lua_remove(L, 3); // Remove the time delta value from the stack.
		lua_remove(L, 2); // Remove the time value from the stack.
	}
	else
		lua_pushstring(L, "nil");

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Define the module table.
static const luaL_Reg mod_lib[] =
{
	{"new", lua_TimerNew},
	{"start", lua_TimerStart},
	{"update", lua_TimerUpdate},
	{"peekDelta", lua_TimerPeekDelta},
	{"delta", lua_TimerDelta},
	{"time", lua_TimerTime},
	{"stop", lua_TimerStop},
	{"reset", lua_TimerReset},
	//{"free", lua_TimerFree},
	{NULL, NULL}
};
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Push the Texture userdata.
lua_Timer* lua_pushTimer(lua_State * L)
{
	lua_Timer* newvalue = (lua_Timer*) lua_newuserdata(L, sizeof(lua_Timer));
	luaL_setmetatable(L, LUA_TIMERHANDLE);
	return newvalue;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Define the Timer userdata metatable.
static const luaL_Reg Timer_lib[] =
{
	{"start", lua_TimerStart},
	{"update", lua_TimerUpdate},
	{"peekDelta", lua_TimerPeekDelta},
	{"delta", lua_TimerDelta},
	{"time", lua_TimerTime},
	{"stop", lua_TimerStop},
	{"reset", lua_TimerReset},
	//{"free", lua_TimerFree},
	{"__gc", lua_TimerFree},
	{"__tostring", lua_TimerToString},
	{NULL, NULL}
};
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Open the PSP lua Timer libraray.
LUAMOD_API int luaopen_Timer(lua_State * L)
{
	// Create the module.
	luaL_newlib(L, mod_lib);

	// Create the Timer user metatable.
	luaL_newusermatatable(L, LUA_TIMERHANDLE, Timer_lib);

	// Create a dummy object.
	(*lua_pushTimer(L)) = NULL;
	lua_setfield(L, -2, "object");

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
