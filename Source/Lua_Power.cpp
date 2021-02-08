// PSP Lua Power ( FileName: Lua_Power.cpp )
// ------------------------------------------------------------------------
// Version: 1.00
// Copyright (c) 2012 M4MSOFT
// ------------------------------------------------------------------------

// Include psp Header files.
#include <psppower.h>

// Include the Lua header files for C++.
#include "luaplayer.hpp"
#include "Lua_Power.hpp"

// Select the luaplayer objects to use in this file.
using luaplayer::clamp;

// -----------------------------------------------------------------------------------------------------
// Define the function prototypes
static int lua_PowerIsPowerOnline (lua_State * L);
static int lua_PowerIsBatteryExist (lua_State * L);
static int lua_PowerIsBatteryCharging (lua_State * L);
static int lua_PowerGetBatteryChargingStatus (lua_State * L);
static int lua_PowerIsLowBattery (lua_State * L);
static int lua_PowerGetBatteryLifePercent (lua_State * L);
static int lua_PowerGetBatteryLifeTime (lua_State * L);
static int lua_PowerGetBatteryTemp (lua_State * L);
static int lua_PowerGetBatteryVolt (lua_State * L);
static int lua_PowerClockFrequency (lua_State * L);
static int lua_PowerLock(lua_State * L);
static int lua_PowerUnlock(lua_State * L);
static int lua_PowerTick(lua_State * L);
static int lua_PowerStandby(lua_State * L);
static int lua_PowerSuspend(lua_State * L);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Check if unit is plugged in.
static int lua_PowerIsPowerOnline(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "Argument error: Power.online() takes no arguments.");

	// Return true if the unit is plugged in.
	lua_pushboolean(L, scePowerIsPowerOnline() == 1);

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Check if a battery is present.
static int lua_PowerIsBatteryExist(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "Argument error: Power.batteryExist() takes no arguments.");

	// Return true if the battery is present.
	lua_pushboolean(L, scePowerIsBatteryExist() == 1);

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Check if the battery is charging.
static int lua_PowerIsBatteryCharging(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "Argument error: Power.batteryCharging() takes no arguments.");

	// Return true if the battery is charging.
	lua_pushboolean(L, scePowerIsBatteryCharging() == 1);

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Get the status of the battery charging.
static int lua_PowerGetBatteryChargingStatus(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "Argument error: Power.batteryChargingStatus() takes no arguments.");

	// Return the battery charging status.
	lua_pushinteger(L, scePowerGetBatteryChargingStatus());

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Check if the battery is low.
static int lua_PowerIsLowBattery(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "Argument error: Power.batteryLow() takes no arguments.");

	// Return true if the battery is low.
	lua_pushboolean(L, scePowerIsLowBattery() == 1);

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Get battery life as integer percent.
static int lua_PowerGetBatteryLifePercent(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "Argument error: Power.batteryLifePercent() takes no arguments.");

	// Return battery life as integer percent.
	lua_pushinteger(L, scePowerGetBatteryLifePercent());

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Get battery life as time.
static int lua_PowerGetBatteryLifeTime(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "Argument error: Power.batteryLifeTime() takes no arguments.");

	// Return battery life in minutes.
	lua_pushinteger(L, scePowerGetBatteryLifeTime());

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Get temperature of the battery.
static int lua_PowerGetBatteryTemp(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "Argument error: Power.batteryTemp() takes no arguments.");

	// Return temperature of the battery.
	lua_pushinteger(L, scePowerGetBatteryTemp());

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Get battery volt level.
static int lua_PowerGetBatteryVolt(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "Argument error: Power.batteryVolt() takes no arguments.");

	// Return volt level of the battery.
	lua_pushinteger(L, scePowerGetBatteryVolt());

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Get or Set Clock Frequencies.
static int lua_PowerClockFrequency (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg > 1) return luaL_error(L, "Argument error: Power.cpuSpeed([mHz]) takes zero or one argument.");

	// Set the Cpu Clock Frequencies.
	if (narg)
	{
		int imHz = clamp(luaL_checkint(L, 1), 19, 333);
		scePowerSetClockFrequency(imHz, imHz, imHz>>1);
	}

	// Return the Cpu Clock Frequencies as a Lua_integer.
	lua_pushinteger(L, scePowerGetCpuClockFrequency()); // First result.

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Lock the power switch.
static int lua_PowerLock(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "Argument error: Power.lock() takes no arguments.");

	// Return true if the switch has been locked.
	lua_pushboolean(L, scePowerLock(0) == 0);

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Unlock the power switch.
static int lua_PowerUnlock(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "Argument error: Power.unlock() takes no arguments.");

	// Return true if the switch has been unlocked.
	lua_pushboolean(L, scePowerUnlock(0) == 0);

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Generate a power tick, preventing unit from powering off and turning off display.
static int lua_PowerTick(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "Argument error: Power.tick([type]) takes no arguments.");

	// Check the power tick type and generate the power tick.
	int type = (narg == 1 ? luaL_checkint(L, 1) : PSP_POWER_TICK_ALL);
	if ((type && PSP_POWER_TICK_ALL) == type || (type && PSP_POWER_TICK_SUSPEND) == type || (type && PSP_POWER_TICK_DISPLAY) == type)
		scePowerTick(type);
	else
		return luaL_error(L, "Invalid power tick type.");

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Request the PSP to go into standby.
static int lua_PowerStandby(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "Argument error: Power.standby() takes no arguments.");
	scePowerRequestStandby();
	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Request the PSP to go into suspend.
static int lua_PowerSuspend(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "Argument error: Power.suspend() takes no arguments.");
	scePowerRequestSuspend();
	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Define the module table.
static const luaL_Reg mod_lib[] =
{
	{"online", lua_PowerIsPowerOnline},
	{"batteryExist", lua_PowerIsBatteryExist},
	{"batteryCharging", lua_PowerIsBatteryCharging},
	{"batteryChargingStatus", lua_PowerGetBatteryChargingStatus},
	{"batteryLow", lua_PowerIsLowBattery},
	{"batteryLifePercent", lua_PowerGetBatteryLifePercent},
	{"batteryLifeTime", lua_PowerGetBatteryLifeTime},
	{"batteryTemp", lua_PowerGetBatteryTemp},
	{"batteryVolt", lua_PowerGetBatteryVolt},
	{"cpuSpeed", lua_PowerClockFrequency},
	{"lock", lua_PowerLock},
	{"unlock", lua_PowerUnlock},
	{"tick", lua_PowerTick},
	{"standby", lua_PowerStandby},
	{"suspend", lua_PowerSuspend},
	{NULL, NULL}
};
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Open the PSP lua Power libraray.
LUAMOD_API int luaopen_Power(lua_State * L)
{
	// Set the Cpu Clock Frequencies to 100 mhz.
	scePowerSetClockFrequency(100, 100, 50);

	// Create the module.
	luaL_newlib(L, mod_lib);

	// Define the Enum Values.
	SetFieldValue("TICK_ALL", PSP_POWER_TICK_ALL)
	SetFieldValue("TICK_SUSPEND", PSP_POWER_TICK_SUSPEND)
	SetFieldValue("TICK_DISPLAY", PSP_POWER_TICK_DISPLAY)

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
