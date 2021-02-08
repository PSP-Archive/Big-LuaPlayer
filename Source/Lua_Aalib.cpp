// PSP Lua Aalib ( FileName: Lua_Aalib.cpp )
// ------------------------------------------------------------------------
// Header file for LuaPlayer.
// Version: 1.00
// Copyright (c) 2012 M4MSOFT
// ------------------------------------------------------------------------

// Include the PSP Aalib Header file.
#include "pspaalib.h"

// Include the Lua header files for C++.
#include "luaplayer.hpp"
#include "Lua_Aalib.hpp"

// Select the luaplayer objects to use in this file.
using luaplayer::luaL_newusermatatable;

// -----------------------------------------------------------------------------------------------------
// Define the lua function prototypes
static int lua_Aalib_load(lua_State *L);
static int lua_Aalib_play(lua_State *L);
static int lua_Aalib_pause(lua_State *L);
static int lua_Aalib_rewind(lua_State *L);
static int lua_Aalib_stop(lua_State *L);
static int lua_Aalib_setVolume(lua_State *L);
static int lua_Aalib_forward(lua_State *L);
static int lua_Aalib_unload(lua_State *L);

static int lua_Aalib_stopAll(lua_State *L);
static int lua_Aalib_unloadAll(lua_State *L);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Define Private Data.
static const int C_INT_CHANNELS = 49;
static char channels[C_INT_CHANNELS] = {
	0,                                                                 // Empty Channels
	0,0,                                                               // MP3 Channels
	0,0,                                                               // Empty Channels
	0,0,                                                               // AT3 Channels
	0,0,0,0,0,0,0,0,0,0,                                               // Ogg Channels
	0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0 // Wav Channels
};
static lua_Channel check_audioChannel(lua_Channel channel)
{
	if (channel >= PSPAALIB_CHANNEL_SCEMP3_1 && channel <= PSPAALIB_CHANNEL_SCEMP3_2) return channel;
	if (channel >= PSPAALIB_CHANNEL_AT3_1 && channel <= PSPAALIB_CHANNEL_AT3_2) return channel;
	if (channel >= PSPAALIB_CHANNEL_OGG_1 && channel <= PSPAALIB_CHANNEL_OGG_10) return channel;
	if (channel >= PSPAALIB_CHANNEL_WAV_1 && channel <= PSPAALIB_CHANNEL_WAV_32) return channel;
	return 0;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_Aalib_load(lua_State *L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 2 && narg != 3) return luaL_error(L, "Aalib.load(path, channel [,ram]): takes 2 or 3 arguments.");

	// Get the arguments.
	const char *path = luaL_checkstring(L, 1);
	lua_Channel channel = luaL_checkint(L, 2);

	// Check the channel.
	if (!check_audioChannel(channel) && channels[channel])
		//return luaL_error(L, "Aalib.load: channel isn't an available channel.");
		return 0; // Number of results.

	lua_gc(L, LUA_GCCOLLECT, 0);
	if (AalibLoad((char*)path, channel, narg == 3 ? (lua_toboolean(L, 3)) : true))
		//return luaL_error(L, "Aalib.load: Error loading sound.");
		return 0; // Number of results.

	// Return the Channel userdata.
	channels[channel] = 1;
	(*lua_pushChannel(L)) = channel;

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_Aalib_play(lua_State *L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1 && narg != 2) return luaL_error(L, "Aalib.play(channel [,loop]): takes 1 or 2 arguments.");

	// Get the channel.
	lua_Channel channel = lua_isuserdata(L, 1) ? *luaL_checkChannel(L, 1) : luaL_checkint(L, 1);

	// Check the channel.
	if (!channels[check_audioChannel(channel)]) return 0; // Number of results.

	// Set whether a stream should repeat forever.
	AalibSetAutoloop(channel, narg == 1 ? false : lua_toboolean(L, 2));

	// Reserve a hardware channel and start playing an audio stream.
	AalibPlay(channel);

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_Aalib_pause(lua_State *L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "Aalib.pause(channel): takes 1 argument.");

	// Get the channel.
	lua_Channel channel = lua_isuserdata(L, 1) ? *luaL_checkChannel(L, 1) : luaL_checkint(L, 1);

	// Check the channel.
	if (!channels[check_audioChannel(channel)]) return 0; // Number of results.

	// Pause/unpause a stream.
	AalibPause(channel);

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_Aalib_rewind(lua_State *L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "Aalib.rewind(channel): takes 1 argument.");

	// Get the channel.
	lua_Channel channel = lua_isuserdata(L, 1) ? *luaL_checkChannel(L, 1) : luaL_checkint(L, 1);

	// Check the channel.
	if (!channels[check_audioChannel(channel)]) return 0; // Number of results.

	// Rewind a stream.
	AalibRewind(channel);

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_Aalib_stop(lua_State *L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "Aalib.stop(channel): takes 1 argument.");

	// Get the channel.
	lua_Channel channel = lua_isuserdata(L, 1) ? *luaL_checkChannel(L, 1) : luaL_checkint(L, 1);

	// Check the channel.
	if (!channels[check_audioChannel(channel)]) return 0; // Number of results.

	// Stop playing a stream and release the hardware channel.
	AalibStop(channel);

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_Aalib_setVolume(lua_State *L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg < 1 || narg > 3) return luaL_error(L, "Aalib.setVolume(channel, left, right): takes 3 arguments.");

	// Get the channel.
	lua_Channel channel = lua_isuserdata(L, 1) ? *luaL_checkChannel(L, 1) : luaL_checkint(L, 1);

	// Check the channel.
	if (!channels[check_audioChannel(channel)]) return 0; // Number of results.

	// Get the volume.
	AalibVolume volume = {(float)CLAMP(luaL_checkint(L, 2), 0, 100) / 100, (float)CLAMP(luaL_checkint(L, 3), 0, 100) / 100};

	// Set the stream volume.
	AalibEnable(channel, PSPAALIB_EFFECT_VOLUME_MANUAL);
	AalibSetVolume(channel, volume);

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_Aalib_forward(lua_State *L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg < 1 || narg > 2) return luaL_error(L, "Aalib.forward(channel, speed): takes 2 arguments.");

	// Get the channel.
	lua_Channel channel = lua_isuserdata(L, 1) ? *luaL_checkChannel(L, 1) : luaL_checkint(L, 1);

	// Check the channel.
	if (!channels[check_audioChannel(channel)]) return 0; // Number of results.

	// Set the stream play speed.
	AalibEnable(channel, PSPAALIB_EFFECT_PLAYSPEED);
	AalibSetPlaySpeed(channel, CLAMP(luaL_checknumber(L, 2), 0.1f, 50.0f));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_Aalib_unload(lua_State *L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "Aalib.unload(channel): takes 1 argument.");

	// Get the channel.
	lua_Channel channel = lua_isChannel(L, 1) ? *lua_toChannel(L, 1) : lua_tointeger(L, 1);

	// Check and unluad the channel.
	if (channels[check_audioChannel(channel)])
		if (!AalibUnload(channel))
			channels[channel] = 0;

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
static int lua_Aalib_stopAll(lua_State *L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "Aalib.stopAll(): takes no arguments.");

	// Stop playing all streams.
	for (int i = 1; i < C_INT_CHANNELS; i++)
		AalibStop(i);

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_Aalib_unloadAll(lua_State *L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "Aalib.unloadAll(): takes no arguments.");

	// unluad all streams.
	for (int i = 1; i < C_INT_CHANNELS; i++)
		if (channels[check_audioChannel(i)])
			if (!AalibUnload(i))
				channels[i] = 0;

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Define the module table.
static const luaL_Reg mod_lib[] =
{
	{"load", lua_Aalib_load},
	{"play", lua_Aalib_play},
	{"pause", lua_Aalib_pause},
	{"rewind", lua_Aalib_rewind},
	{"stop", lua_Aalib_stop},
	{"setVolume", lua_Aalib_setVolume},
	{"forward", lua_Aalib_forward},
	{"unload", lua_Aalib_unload},

	{"stopAll", lua_Aalib_stopAll},
	{"unloadAll", lua_Aalib_unloadAll},
	{NULL, NULL}
};
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Push the Channel userdata.
lua_Channel* lua_pushChannel(lua_State * L)
{
	lua_Channel* newvalue = (lua_Channel*) lua_newuserdata(L, sizeof(lua_Channel));
	luaL_setmetatable(L, LUA_CHANNELHANDLE);
	return newvalue;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Define the Channel userdata metatable.
static const luaL_Reg Channel_lib[] =
{
	{"play", lua_Aalib_play},
	{"pause", lua_Aalib_pause},
	{"rewind", lua_Aalib_rewind},
	{"stop", lua_Aalib_stop},
	{"setVolume", lua_Aalib_setVolume},
	{"forward", lua_Aalib_forward},
	{"unload", lua_Aalib_unload},

	{"__gc", lua_Aalib_unload},
	{NULL, NULL}
};
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Open the PSP lua Aalib libraray.
LUAMOD_API int luaopen_Aalib(lua_State *L)
{
	// Initalize the PSP Advanced Audio Library.
	AalibInit();

	// Create the module.
	luaL_newlib(L, mod_lib);

	// Create the Color user metatable.
	luaL_newusermatatable(L, LUA_CHANNELHANDLE, Channel_lib);

	// Define the Enum Values.
	SetFieldValue("MP3_1", PSPAALIB_CHANNEL_SCEMP3_1)
	SetFieldValue("MP3_2", PSPAALIB_CHANNEL_SCEMP3_2)

	SetFieldValue("AT3_1", PSPAALIB_CHANNEL_AT3_1)
	SetFieldValue("AT3_2", PSPAALIB_CHANNEL_AT3_2)

	SetFieldValue("OGG_1", PSPAALIB_CHANNEL_OGG_1)
	SetFieldValue("OGG_2", PSPAALIB_CHANNEL_OGG_2)
	SetFieldValue("OGG_3", PSPAALIB_CHANNEL_OGG_3)
	SetFieldValue("OGG_4", PSPAALIB_CHANNEL_OGG_4)
	SetFieldValue("OGG_5", PSPAALIB_CHANNEL_OGG_5)
	SetFieldValue("OGG_6", PSPAALIB_CHANNEL_OGG_6)
	SetFieldValue("OGG_7", PSPAALIB_CHANNEL_OGG_7)
	SetFieldValue("OGG_8", PSPAALIB_CHANNEL_OGG_8)
	SetFieldValue("OGG_9", PSPAALIB_CHANNEL_OGG_9)
	SetFieldValue("OGG_10", PSPAALIB_CHANNEL_OGG_10)

	SetFieldValue("WAV_1", PSPAALIB_CHANNEL_WAV_1)
	SetFieldValue("WAV_2", PSPAALIB_CHANNEL_WAV_2)
	SetFieldValue("WAV_3", PSPAALIB_CHANNEL_WAV_3)
	SetFieldValue("WAV_4", PSPAALIB_CHANNEL_WAV_4)
	SetFieldValue("WAV_5", PSPAALIB_CHANNEL_WAV_5)
	SetFieldValue("WAV_6", PSPAALIB_CHANNEL_WAV_6)
	SetFieldValue("WAV_7", PSPAALIB_CHANNEL_WAV_7)
	SetFieldValue("WAV_8", PSPAALIB_CHANNEL_WAV_8)
	SetFieldValue("WAV_9", PSPAALIB_CHANNEL_WAV_9)
	SetFieldValue("WAV_10", PSPAALIB_CHANNEL_WAV_10)
	SetFieldValue("WAV_11", PSPAALIB_CHANNEL_WAV_11)
	SetFieldValue("WAV_12", PSPAALIB_CHANNEL_WAV_12)
	SetFieldValue("WAV_13", PSPAALIB_CHANNEL_WAV_13)
	SetFieldValue("WAV_15", PSPAALIB_CHANNEL_WAV_15)
	SetFieldValue("WAV_16", PSPAALIB_CHANNEL_WAV_16)
	SetFieldValue("WAV_17", PSPAALIB_CHANNEL_WAV_17)
	SetFieldValue("WAV_18", PSPAALIB_CHANNEL_WAV_18)
	SetFieldValue("WAV_19", PSPAALIB_CHANNEL_WAV_19)
	SetFieldValue("WAV_20", PSPAALIB_CHANNEL_WAV_20)
	SetFieldValue("WAV_21", PSPAALIB_CHANNEL_WAV_21)
	SetFieldValue("WAV_22", PSPAALIB_CHANNEL_WAV_22)
	SetFieldValue("WAV_23", PSPAALIB_CHANNEL_WAV_23)
	SetFieldValue("WAV_24", PSPAALIB_CHANNEL_WAV_24)
	SetFieldValue("WAV_25", PSPAALIB_CHANNEL_WAV_25)
	SetFieldValue("WAV_26", PSPAALIB_CHANNEL_WAV_26)
	SetFieldValue("WAV_27", PSPAALIB_CHANNEL_WAV_27)
	SetFieldValue("WAV_28", PSPAALIB_CHANNEL_WAV_28)
	SetFieldValue("WAV_29", PSPAALIB_CHANNEL_WAV_29)
	SetFieldValue("WAV_30", PSPAALIB_CHANNEL_WAV_30)
	SetFieldValue("WAV_31", PSPAALIB_CHANNEL_WAV_31)
	SetFieldValue("WAV_32", PSPAALIB_CHANNEL_WAV_32)

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
