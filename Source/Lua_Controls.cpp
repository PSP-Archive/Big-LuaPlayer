// PSP Lua Controls ( FileName: Lua_Controls.cpp )
// ------------------------------------------------------------------------
// Version: 1.00
// Copyright (c) 2012 M4MSOFT
// ------------------------------------------------------------------------

// Include psp Header files.
#include <pspctrl.h>
#include <pspctrl_kernel.h>

// Include the Lua header files for C++.
#include "luaplayer.hpp"
#include "Lua_Controls.hpp"

// Select the luaplayer objects to use in this file.
using luaplayer::clamp;

// -----------------------------------------------------------------------------------------------------
namespace // Define static module values with internal linkage.
{
	// Declare the sceContol structure.
	struct SceContol
	{
		unsigned int uiDelta;
		unsigned int uiPress;
		unsigned int uiHeld;
		unsigned int uiRelease;
	};

	// Declare the Psp Control Data Buffer.
	SceCtrlData ctrlDataBuffer = {0,0,0,0,{0,0,0,0,0,0}};
	// Declare the Psp Control Memory Buffer.
	SceContol ctrlMemoryBuffer = {0,0,0,0};

} // End internal namespace
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Define the lua function prototypes
static int lua_SamplingCycle (lua_State * L);
static int lua_SamplingMode (lua_State * L);

static int lua_read (lua_State * L);
static int lua_readPeek (lua_State * L);

static int lua_TimeStamp (lua_State * L);
static int lua_analogx (lua_State * L);
static int lua_analogy (lua_State * L);

static int lua_Buttons (lua_State * L);
static int lua_Delta (lua_State * L);
static int lua_Press (lua_State * L);
static int lua_Held (lua_State * L);
static int lua_Release (lua_State * L);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Get or Set the controller Cycle setting.
static int lua_SamplingCycle (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg > 1) return luaL_error(L, "Argument error: Controls.samplingCycle([cycle]) takes zero or one argument.");

	// Set the controller cycle setting.
	if (narg)
		sceCtrlSetSamplingCycle(luaL_checkint(L, narg));

	// Return the controller cycle setting as a Lua_integer.
	int cycle = 0;
	sceCtrlGetSamplingCycle( &cycle );
	lua_pushinteger(L, cycle); // First result.

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Get or Set the controller Mode setting.
static int lua_SamplingMode (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg > 1) return luaL_error(L, "Argument error: Controls.samplingMode([mode]) takes zero or one argument.");

	// Set the controller Mode setting.
	if (narg)
		sceCtrlSetSamplingMode(clamp(luaL_checkint(L, narg), 0, 1));

	// Return the controller Mode setting as a Lua_integer.
	int cycle = 0;
	sceCtrlGetSamplingMode( &cycle );
	lua_pushinteger(L, cycle); // First result.

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Read the controls and update the ctrlMemoryBuffer and ctrlDataBuffer,
static int lua_read (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "Argument error: Controls.read() takes no arguments.");

	// Read the controls.
	sceCtrlReadBufferPositive(&ctrlDataBuffer, 1);
	// Buffer the controls.
	ctrlMemoryBuffer.uiDelta &= ~ctrlMemoryBuffer.uiRelease; // Turn the release control bit off.
	ctrlMemoryBuffer.uiHeld  = ctrlMemoryBuffer.uiDelta & ctrlDataBuffer.Buttons; // Calculate the control Held bit.
	ctrlMemoryBuffer.uiPress = ~ctrlMemoryBuffer.uiDelta & ctrlDataBuffer.Buttons; // Calculate the control Press bit.
	ctrlMemoryBuffer.uiDelta |= ctrlDataBuffer.Buttons; // Turn the Buttons control bit on.
	ctrlMemoryBuffer.uiRelease = ctrlMemoryBuffer.uiDelta & ~ctrlDataBuffer.Buttons; // Calculate the control Release bit.

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Read the controls and update the ctrlMemoryBuffer and ctrlDataBuffer,
static int lua_readPeek (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "Argument error: Controls.readPeek() takes no arguments.");

	// Read the controls.
	sceCtrlPeekBufferPositive(&ctrlDataBuffer, 1);
	// Buffer the controls.
	ctrlMemoryBuffer.uiDelta &= ~ctrlMemoryBuffer.uiRelease; // Turn the release control bit off.
	ctrlMemoryBuffer.uiHeld  = ctrlMemoryBuffer.uiDelta & ctrlDataBuffer.Buttons; // Calculate the control Held bit.
	ctrlMemoryBuffer.uiPress = ~ctrlMemoryBuffer.uiDelta & ctrlDataBuffer.Buttons; // Calculate the control Press bit.
	ctrlMemoryBuffer.uiDelta |= ctrlDataBuffer.Buttons; // Turn the Buttons control bit on..
	ctrlMemoryBuffer.uiRelease = ctrlMemoryBuffer.uiDelta & ~ctrlDataBuffer.Buttons; // Calculate the control Release bit.

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Get the current read frame.
static int lua_TimeStamp (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "Argument error: Controls.timeStamp() takes no arguments.");

	// Return the current read frame as a Lua_integer.
	lua_pushinteger(L, ctrlDataBuffer.TimeStamp); // First result.

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Get the current Analogue x position.
static int lua_analogx (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "Argument error: Controls.analogx() takes no arguments.");

	// Return the current Analogue x position as a Lua_integer.
	lua_pushinteger(L, ctrlDataBuffer.Lx - 128); // First result.

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Get the current Analogue y position.
static int lua_analogy (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "Argument error: Controls.analogy() takes no arguments.");

	// Return the current Analogue y position as a Lua_integer.
	lua_pushinteger(L,  ctrlDataBuffer.Ly - 128); // First result.

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Return the Buttons state as a integer or boolean. (lua_Buttons == (lua_Press & lua_Held))
static int lua_Buttons (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);

	// Retrun a integer if the function has no arguments else return a boolean.
	switch (narg)
	{
		case 0 :
			// Return the control data from the PspControlBuffer.
			lua_pushinteger(L, ctrlDataBuffer.Buttons); // First result.
			break;
		case 1 :
			// Check the control data and return the result.
			lua_pushboolean(L, (ctrlDataBuffer.Buttons & luaL_checkunsigned(L, 1)) ); // First result.
			break;
		case 2 :
			// Check the control data and return the result.
			lua_pushboolean(L, ((ctrlDataBuffer.Buttons & luaL_checkunsigned(L, 1)) == luaL_checkunsigned(L, 2)) ); // First result.
			break;
		default:
			return luaL_error(L, "Argument error: Controls.buttons([Enum_mask [,Enum_test]]) takes zero to two arguments.");
	}

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Return the Delta state as a integer or boolean. (lua_Delta == (lua_Press & lua_Held & lua_Release))
static int lua_Delta (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);

	// Retrun a integer if the function has no arguments else return a boolean.
	switch (narg)
	{
		case 0 :
			// Return the control data from the PspControlBuffer.
			lua_pushinteger(L, ctrlMemoryBuffer.uiDelta); // First result.
			break;
		case 1 :
			// Check the control data and return the result.
			lua_pushboolean(L, (ctrlMemoryBuffer.uiDelta & luaL_checkunsigned(L, 1)) ); // First result.
			break;
		case 2 :
			// Check the control data and return the result.
			lua_pushboolean(L, ((ctrlMemoryBuffer.uiDelta & luaL_checkunsigned(L, 1)) == luaL_checkunsigned(L, 2)) ); // First result.
			break;
		default:
			return luaL_error(L, "Argument error: Controls.delta([Enum_mask [,Enum_test]]) takes zero to two arguments.");
	}

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Return the Press state as a integer or boolean.
static int lua_Press (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);

	// Retrun a integer if the function has no arguments else return a boolean.
	switch (narg)
	{
		case 0 :
			// Return the control data from the PspControlBuffer.
			lua_pushinteger(L, ctrlMemoryBuffer.uiPress); // First result.
			break;
		case 1 :
			// Check the control data and return the result.
			lua_pushboolean(L, (ctrlMemoryBuffer.uiPress & luaL_checkunsigned(L, 1)) ); // First result.
			break;
		case 2 :
			// Check the control data and return the result.
			lua_pushboolean(L, ((ctrlMemoryBuffer.uiPress & luaL_checkunsigned(L, 1)) == luaL_checkunsigned(L, 2)) ); // First result.
			break;
		default:
			return luaL_error(L, "Argument error: Controls.press([Enum_mask [,Enum_test]]) takes zero to two arguments.");
	}

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Return the Held state as a integer or boolean.
static int lua_Held (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);

	// Retrun a integer if the function has no arguments else return a boolean.
	switch (narg)
	{
		case 0 :
			// Return the control data from the PspControlBuffer.
			lua_pushinteger(L, ctrlMemoryBuffer.uiHeld); // First result.
			break;
		case 1 :
			// Check the control data and return the result.
			lua_pushboolean(L, (ctrlMemoryBuffer.uiHeld & luaL_checkunsigned(L, 1)) ); // First result.
			break;
		case 2 :
			// Check the control data and return the result.
			lua_pushboolean(L, ((ctrlMemoryBuffer.uiHeld & luaL_checkunsigned(L, 1)) == luaL_checkunsigned(L, 2)) ); // First result.
			break;
		default:
			return luaL_error(L, "Argument error: Controls.held([Enum_mask [,Enum_test]]) takes zero to two arguments.");
	}

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Return the Release state as a integer or boolean.
static int lua_Release (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);

	// Retrun a integer if the function has no arguments else return a boolean.
	switch (narg)
	{
		case 0 :
			// Return the control data from the PspControlBuffer.
			lua_pushinteger(L, ctrlMemoryBuffer.uiRelease); // First result.
			break;
		case 1 :
			// Check the control data and return the result.
			lua_pushboolean(L, (ctrlMemoryBuffer.uiRelease & luaL_checkunsigned(L, 1)) ); // First result.
			break;
		case 2 :
			// Check the control data and return the result.
			lua_pushboolean(L, ((ctrlMemoryBuffer.uiRelease & luaL_checkunsigned(L, 1)) == luaL_checkunsigned(L, 2)) ); // First result.
			break;
		default:
			return luaL_error(L, "Argument error: Controls.release([Enum_mask [,Enum_test]]) takes zero to two arguments.");
	}

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Define the module table.
static const luaL_Reg mod_lib[] =
{
	{"samplingCycle", lua_SamplingCycle},
	{"samplingMode",  lua_SamplingMode},
	{"read",  lua_read},
	{"readPeek",  lua_readPeek},
	{"timeStamp",  lua_TimeStamp},
	{"analogx",  lua_analogx},
	{"analogy",  lua_analogy},
	{"buttons",  lua_Buttons},
	{"delta",  lua_Delta},
	{"press",  lua_Press},
	{"held",  lua_Held},
	{"release",  lua_Release},
	{NULL, NULL}
};
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Open the PSP lua Controls libraray.
LUAMOD_API int luaopen_Controls(lua_State * L)
{
	// Set the controls.
	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

	// Create the module.
	luaL_newlib(L, mod_lib);

	// Define the Enum Values.
	SetFieldValue("SELECT", PSP_CTRL_SELECT)
	SetFieldValue("START", PSP_CTRL_START)
	SetFieldValue("UP", PSP_CTRL_UP)
	SetFieldValue("RIGHT", PSP_CTRL_RIGHT)
	SetFieldValue("DOWN", PSP_CTRL_DOWN)
	SetFieldValue("LEFT", PSP_CTRL_LEFT)
	SetFieldValue("LTRIGGER", PSP_CTRL_LTRIGGER)
	SetFieldValue("RTRIGGER", PSP_CTRL_RTRIGGER)
	SetFieldValue("TRIANGLE", PSP_CTRL_TRIANGLE)
 	SetFieldValue("CIRCLE", PSP_CTRL_CIRCLE)
	SetFieldValue("CROSS", PSP_CTRL_CROSS)
	SetFieldValue("SQUARE", PSP_CTRL_SQUARE)
	SetFieldValue("PAD", PSP_CTRL_UP + PSP_CTRL_RIGHT + PSP_CTRL_DOWN + PSP_CTRL_LEFT)
	SetFieldValue("BUTTON", PSP_CTRL_TRIANGLE + PSP_CTRL_CIRCLE + PSP_CTRL_CROSS + PSP_CTRL_SQUARE)
	SetFieldValue("ALL", PSP_CTRL_SELECT + PSP_CTRL_START +
	                     PSP_CTRL_UP + PSP_CTRL_RIGHT + PSP_CTRL_DOWN + PSP_CTRL_LEFT +
						 PSP_CTRL_LTRIGGER + PSP_CTRL_RTRIGGER +
						 PSP_CTRL_TRIANGLE + PSP_CTRL_CIRCLE + PSP_CTRL_CROSS + PSP_CTRL_SQUARE)
	SetFieldValue("HOME", PSP_CTRL_HOME)

	SetFieldValue("MODE_DIGITAL", PSP_CTRL_MODE_DIGITAL)
	SetFieldValue("MODE_ANALOG", PSP_CTRL_MODE_ANALOG)


	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
