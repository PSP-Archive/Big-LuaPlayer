// PSP Lua Sound ( FileName: Lua_Sound.cpp )
// ------------------------------------------------------------------------
// Header file for LuaPlayer.
// Version: 1.00
// Copyright (c) 2012 M4MSOFT
// ------------------------------------------------------------------------

// Include the soundlib Header file.
#include "soundlib.h"

// Include the Lua header files for C++.
#include "luaplayer.hpp"
#include "Lua_Sound.hpp"

// Select the luaplayer objects to use in this file.
using luaplayer::luaL_newusermatatable;
using luaplayer::clamp;

// -----------------------------------------------------------------------------------------------------
// Define the lua function prototypes
static int lua_SoundInit (lua_State *L);
static int lua_SoundSetVolume (lua_State *L);
static int lua_SoundStopAll (lua_State *L);
static int lua_SoundTerm (lua_State *L);

static int lua_SoundNew (lua_State *L);
static int lua_SoundLoad (lua_State *L);
static int lua_SoundLoadRaw (lua_State *L);
static int lua_SoundLoadPol (lua_State *L);
static int lua_SoundLoadWav (lua_State *L);
static int lua_SoundSave (lua_State *L);

static int lua_SoundBuffer (lua_State *L);
static int lua_SoundPlay (lua_State *L);
static int lua_SoundLoop (lua_State *L);
static int lua_SoundStop (lua_State *L);
static int lua_SoundPause (lua_State *L);
static int lua_SoundSeek (lua_State *L);
static int lua_SoundRewind (lua_State *L);
static int lua_SoundAmplification (lua_State *L);

static int lua_SoundFree (lua_State *L);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Initializes the psp sound library.
static int lua_SoundInit(lua_State *L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg > 1) return luaL_error(L, "Sound.init([channels]): takes zero or one argument.");

	// Initializes the psp sound channels.
	SoundInit(narg == 1 ? luaL_checkunsigned(L, 1) : 2);

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_SoundSetVolume(lua_State *L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg < 1 || narg > 3) return luaL_error(L, "Sound.setVolume(right [,left [channel]]): takes one to three argument.");

	// Set the volume of a channel.
	pspSoundSetVolume(
		(narg > 2 ? clamp(luaL_checkint(L, 3), 0x0, 0x3) : 0x0),
		(narg > 1 ? clamp(luaL_checkint(L, 2), 0x0, 0x8000) : 0x8000),
		clamp(luaL_checkint(L, 1), 0x0, 0x8000)
	);

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Stop Playing all sound objects.
static int lua_SoundStopAll(lua_State *L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "Sound.stopAll(): takes no arguments.");

	// Stop Playing all sound objects.
	SoundStopAllObject();

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Shutdowns the sound library.
static int lua_SoundTerm(lua_State *L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "Sound.term(): takes no arguments.");

	// Shutdowns the sound library.
	SoundTerm();

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
static int lua_SoundNew(lua_State *L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 2) return luaL_error(L, "Sound.new(type, size): takes two arguments.");

	// Create a new sound object.
	lua_Voice objPtr = SoundCreateObject(luaL_checkunsigned(L, 1) & 0x2, luaL_checkunsigned(L, 2));

	// Return the Voice userdata.
	if (objPtr)
		(*lua_pushVoice(L)) = objPtr;
	else
		lua_pushnil(L);


	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_SoundLoad(lua_State *L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "Sound.load(path): takes one argument.");

	// Load the wav object.
	lua_gc(L, LUA_GCCOLLECT, 0);
	lua_Voice objPtr = SoundLoadObject(luaL_checkstring(L, 1));

	// Return the Voice userdata.
	if (objPtr)
		(*lua_pushVoice(L)) = objPtr;
	else
		lua_pushnil(L);

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_SoundLoadRaw(lua_State *L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "Sound.loadRaw(path): takes one argument.");

	// Load the Raw object.
	lua_gc(L, LUA_GCCOLLECT, 0);
	lua_Voice objPtr = SoundLoadRaw(luaL_checkstring(L, 1));

	// Return the Voice userdata.
	if (objPtr)
		(*lua_pushVoice(L)) = objPtr;
	else
		lua_pushnil(L);

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_SoundLoadPol(lua_State *L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "Sound.loadPol(path): takes one argument.");

	// Load the wav object.
	lua_gc(L, LUA_GCCOLLECT, 0);
	lua_Voice objPtr = SoundLoadPol(luaL_checkstring(L, 1));

	// Return the Voice userdata.
	if (objPtr)
		(*lua_pushVoice(L)) = objPtr;
	else
		lua_pushnil(L);

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_SoundLoadWav(lua_State *L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "Sound.loadWav(path): takes one argument.");

	// Load the wav object.
	lua_gc(L, LUA_GCCOLLECT, 0);
	lua_Voice objPtr = SoundLoadWav(luaL_checkstring(L, 1));

	// Return the Voice userdata.
	if (objPtr)
		(*lua_pushVoice(L)) = objPtr;
	else
		lua_pushnil(L);

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_SoundSave(lua_State *L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 2) return luaL_error(L, "Sound.save(voice, path): takes two arguments.");

	// Save the sound object.
	SoundSaveObject(*luaL_checkVoice(L, 1), luaL_checkstring(L, 2));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_SoundBuffer(lua_State *L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg < 2) return luaL_error(L, "Sound.buffer(voice, pos [,...]): takes two or more arguments.");

	// Get the sound object pointer.
	lua_Voice objPtr = *luaL_checkVoice(L, 1);

	// Select the object type.
	switch (objPtr->type)
	{
		case SOUNDLIB_TYPE_RAW: // raw sound stream.
			if (narg != 2 && narg != 4) return luaL_error(L, "Sound._bufferRaw(voice, pos [,left ,right]): takes two or four arguments.");
			if (narg > 2)
			{
				// Set the sound data
				soundBuf_ buf = {0,0};
				buf.l = luaL_checkint(L, 3);
				buf.r = luaL_checkint(L, 4);
				SoundSetBufferObject(objPtr, luaL_checkunsigned(L, 2), &buf);
				return 0; // Number of results.
			}
			else
			{
				// Get the sound data
				soundBuf buf = (soundBuf)SoundGetBufferObject(objPtr, luaL_checkunsigned(L, 2));
				if (buf == NULL)
					return 0; // Number of results.

				// Return the sound data.
				lua_pushinteger(L, buf->l);
				lua_pushinteger(L, buf->r);

				return 2; // Number of results.
			}
		case SOUNDLIB_TYPE_POL: // polyphonic sound stream.
			if (narg != 2 && narg != 6) return luaL_error(L, "Sound._bufferPol(voice, pos [,note ,octave, duration, volume]): takes two or six arguments.");
			if (narg > 2)
			{
				// Set the sound data
				sound_note_ buf = {0,0};
				buf.note = clamp(luaL_checkint(L, 3), 0, 12);
				buf.octave = clamp(luaL_checkint(L, 4), 0, 5);
				buf.duration = luaL_checkunsigned(L, 5);
				buf.volume = clamp(luaL_checkint(L, 6), -32768, 32767);
				SoundSetBufferObject(objPtr, luaL_checkunsigned(L, 2), &buf);
				return 0; // Number of results.
			}
			else
			{
				// Get the sound data
				sound_note buf = (sound_note)SoundGetBufferObject(objPtr, luaL_checkunsigned(L, 2));
				if (buf == NULL)
					return 0; // Number of results.

				// Return the sound data.
				lua_pushinteger(L, buf->note);
				lua_pushinteger(L, buf->octave);
				lua_pushinteger(L, buf->duration);
				lua_pushinteger(L, buf->volume);

				return 4; // Number of results.
			}
		default:
			return 0; // Number of results.
	}
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Start playing the sound object.
static int lua_SoundPlay(lua_State *L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg < 1 || narg > 3) return luaL_error(L, "Sound.play(voice [,rewind [,loop]]): takes one to three arguments.");

	// Start playing the sound object.
	SoundPlayObject(*luaL_checkVoice(L, 1), (narg > 1 ? lua_toboolean(L, 2) : false), (narg > 2 ? lua_toboolean(L, 3) : false));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_SoundLoop(lua_State *L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 2) return luaL_error(L, "Sound.loop(voice, loop): takes two arguments.");

	// Enable or disable the sound loop.
	SoundLoopObject(*luaL_checkVoice(L, 1), lua_toboolean(L, 2));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_SoundStop(lua_State *L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "Sound.stop(voice): takes one argument.");

	// Stop Playing a sound object.
	SoundStopObject(*luaL_checkVoice(L, 1));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_SoundPause(lua_State *L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "Sound.pause(voice): takes one argument.");

	// Pause a playing sound object.
	SoundPauseObject(*luaL_checkVoice(L, 1));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_SoundSeek(lua_State *L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1 && narg != 2) return luaL_error(L, "Sound.seek(voice [,time]): takes one or two arguments.");

	// the the time of a sound object.
	SoundSeekObject(*luaL_checkVoice(L, 1), (narg == 2 ? luaL_checkunsigned(L, 1) : 0));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_SoundRewind(lua_State *L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "Sound.rewind(voice): takes one argument.");

	// Rewind a sound object.
	SoundRewindObject(*luaL_checkVoice(L, 1));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_SoundAmplification(lua_State *L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 2) return luaL_error(L, "Sound.amplification(voice [,amp]): takes one or two arguments.");

	// Set the amplification value of a sound object.
	SoundAmplification(*luaL_checkVoice(L, 1), (narg == 2 ? luaL_checknumber(L, 2) : 1.0f));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
static int lua_SoundFree(lua_State *L)
{
	// Get the Voice Pointer.
	lua_Voice *voicePtr = lua_toVoice(L, 1);

	// Free the Voice Pointer and set the Pointer to NULL.
	if (voicePtr && *voicePtr)
		SoundFreeObject(voicePtr);

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Define the module table.
static const luaL_Reg mod_lib[] =
{
	{"init", lua_SoundInit},
	{"setVolume",lua_SoundSetVolume},
	{"stopAll", lua_SoundStopAll},
	{"term", lua_SoundTerm},

	{"new", lua_SoundNew},
	{"load", lua_SoundLoad},
	{"loadRaw", lua_SoundLoadRaw},
	{"loadPol", lua_SoundLoadPol},
	{"loadWav", lua_SoundLoadWav},
	{"save", lua_SoundSave},

	{"buffer", lua_SoundBuffer},
	{"play", lua_SoundPlay},
	{"loop", lua_SoundLoop},
	{"stop", lua_SoundStop},
	{"pause", lua_SoundPause},
	{"seek", lua_SoundSeek},
	{"rewind", lua_SoundRewind},
	{"amplification", lua_SoundAmplification},

	//{"free", lua_SoundFree},
	{NULL, NULL}
};
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Push the Channel userdata.
lua_Voice* lua_pushVoice(lua_State * L)
{
	lua_Voice* newvalue = (lua_Voice*) lua_newuserdata(L, sizeof(lua_Voice));
	luaL_setmetatable(L, LUA_VOICEHANDLE);
	return newvalue;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Define the Channel userdata metatable.
static const luaL_Reg Voice_lib[] =
{
	{"save", lua_SoundSave},

	{"buffer", lua_SoundBuffer},
	{"play", lua_SoundPlay},
	{"loop", lua_SoundLoop},
	{"stop", lua_SoundStop},
	{"pause", lua_SoundPause},
	{"seek", lua_SoundSeek},
	{"rewind", lua_SoundRewind},
	{"amplification", lua_SoundAmplification},
	//{"free", lua_SoundFree},

	{"__gc", lua_SoundFree},
	{NULL, NULL}
};
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Open the PSP lua Sound libraray.
LUAMOD_API int luaopen_Sound(lua_State *L)
{
	// Create the module.
	luaL_newlib(L, mod_lib);

	// Create the Voice user metatable.
	luaL_newusermatatable(L, LUA_VOICEHANDLE, Voice_lib);

	// Create a dummy object.
	(*lua_pushVoice(L)) = NULL;
	lua_setfield(L, -2, "object");

	// Define the Enum Values.
	SetFieldValue("TYPE_RAW", SOUNDLIB_TYPE_RAW)
	SetFieldValue("TYPE_POL", SOUNDLIB_TYPE_POL)

	SetFieldValue("NOTE_PAUSE", SOUNDLIB_NOTE_PAUSE)
	SetFieldValue("NOTE_C", SOUNDLIB_NOTE_C)
	SetFieldValue("NOTE_CS", SOUNDLIB_NOTE_CS)
	SetFieldValue("NOTE_D", SOUNDLIB_NOTE_D)
	SetFieldValue("NOTE_DS", SOUNDLIB_NOTE_DS)
	SetFieldValue("NOTE_E", SOUNDLIB_NOTE_E)
	SetFieldValue("NOTE_F", SOUNDLIB_NOTE_F)
	SetFieldValue("NOTE_FS", SOUNDLIB_NOTE_FS)
	SetFieldValue("NOTE_G", SOUNDLIB_NOTE_G)
	SetFieldValue("NOTE_GS", SOUNDLIB_NOTE_GS)
	SetFieldValue("NOTE_A", SOUNDLIB_NOTE_A)
	SetFieldValue("NOTE_AS", SOUNDLIB_NOTE_AS)
	SetFieldValue("NOTE_B", SOUNDLIB_NOTE_B)

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
