// PSP Lua System ( FileName: Lua_System.cpp )
// ------------------------------------------------------------------------
// Version: 1.00
// Copyright (c) 2012 M4MSOFT
// ------------------------------------------------------------------------

// Include C/C++ header files
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

// Include psp Header files.
#include <pspkernel.h>
#include <pspsircs.h>
#include <pspwlan.h>

// Include commen psp Header file.
#include "Common/callbacks.h"

// Include the psp usblib Header file.
#include "pspusblib.h"

// Include the psp serial IO Header files.
#include "pspsiolib.h"

// Include the psp irda Header files.
#include "pspirdalib.h"

// Include the Lua header files for C++.
#include "luaplayer.hpp"
#include "Lua_System.hpp"

// -----------------------------------------------------------------------------------------------------
// Define static module objects with external linkage.
// Move g_dir to the stack and MEET CERTAIN DOOM! If the SceIoDirent is found on the STACK instead of among the globals, the PSP WILL CRASH IN A VERY WEIRD WAY. You have been WARNED.
SceIoDirent g_DirBuffer;
namespace // Define static module objects with internal linkage.
{
	// Define the const values.
	const int C_INT_PATH_LEN = 256;

} // End internal namespace
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Define the function prototypes
static int lua_getCurrentDirectory (lua_State * L);
static int lua_setCurrentDirectory (lua_State * L);
static int lua_currentDirectory (lua_State * L);
static int lua_listDirectory (lua_State * L);
static int lua_createDirectory (lua_State * L);
static int lua_directoryExsist(lua_State *L);
static int lua_removeDirectory (lua_State * L);
static int lua_fileExsist(lua_State *L);
static int lua_removeFile (lua_State * L);
static int lua_rename (lua_State * L);

static int lua_Sleep(lua_State *L);

static int lua_Running (lua_State * L);
static int lua_Exit (lua_State * L);

static int lua_Wlan (lua_State * L);

static int lua_sioInit(lua_State *L);
static int lua_sioTerm(lua_State *L);
static int lua_sioWrite(lua_State *L);
static int lua_sioRead(lua_State *L);

static int lua_irdaInit(lua_State *L);
static int lua_irdaTerm(lua_State *L);
static int lua_irdaWrite(lua_State *L);
static int lua_irdaRead(lua_State *L);

static int lua_irSendCmd(lua_State *L);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Get the current directory.
static int lua_getCurrentDirectory (lua_State * L)
{
	// Get the current directory.
	char strPath[C_INT_PATH_LEN] = {};
	getcwd(strPath, C_INT_PATH_LEN);

	// Return the current directory as a string.
	lua_pushstring(L, strPath);

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Set the current directory.
static int lua_setCurrentDirectory (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "Argument error: System.currentDirectory(path) takes one argument.");

	// Get the path argument.
	const char * strPath = luaL_checkstring(L, 1);
	if(!strPath) return luaL_error(L, "Argument error: System.currentDirectory(path) takes a path name as string as argument.");

	// Get the current Directory.
	lua_getCurrentDirectory(L);

	// Set the current Directory.
	chdir(strPath);

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Get or set the current directory.
static int lua_currentDirectory (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg > 1) return luaL_error(L, "Argument error: System.currentDirectory([path]) takes zero or one argument.");

	if(narg)
		// Set the current Directory
		return lua_setCurrentDirectory(L);
	else
		// Get the current Directory
		return lua_getCurrentDirectory(L);
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
static int lua_listDirectory (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg > 1) return luaL_error(L, "Argument error: System.listDirectory([path]) takes zero or one argument.");

	// Open the directory.
	int dfd = -1;
	if (narg)
	{
		// Get the path argument.
		const char * strPath = luaL_checkstring(L, 1);
		if (strPath)
			dfd = sceIoDopen(strPath);
		else
			dfd = sceIoDopen("");
	}
	else
		dfd = sceIoDopen("");

	// Check if the directory exists.
	if (dfd >= 0)
	{
		lua_newtable(L);
		int i = 1;

		// Read the file Directory.
		while (sceIoDread(dfd, &g_DirBuffer) > 0)
		{
			// push key for file entry.
			lua_pushnumber(L, i++);

			// Return the Directory table.
			lua_newtable(L);

				lua_pushstring(L, "name");
				lua_pushstring(L, g_DirBuffer.d_name);
				lua_settable(L, -3);

				lua_pushstring(L, "size");
				lua_pushnumber(L, g_DirBuffer.d_stat.st_size);
				lua_settable(L, -3);

				lua_pushstring(L, "directory");
				lua_pushboolean(L, FIO_S_ISDIR(g_DirBuffer.d_stat.st_mode));
				lua_settable(L, -3);

				lua_pushstring(L, "time");
				lua_pushfstring(L, "%d-%d-%d %d:%d", (int) g_DirBuffer.d_stat.st_mtime.day,
													 (int) g_DirBuffer.d_stat.st_mtime.month,
													 (int) g_DirBuffer.d_stat.st_mtime.year,
													 (int) g_DirBuffer.d_stat.st_mtime.hour,
													 (int) g_DirBuffer.d_stat.st_mtime.minute);
				lua_settable(L, -3);

			lua_settable(L, -3);
		}

		// Close the file Directory.
		sceIoDclose(dfd);
	}
	else
		// Return a nil value.
		lua_pushnil(L);

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_createDirectory (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "Argument error: System.createDirectory(directory) takes one argument.");

	// Get the directory argument.
	const char * strPath = luaL_checkstring(L, 1);
	if(!strPath) return luaL_error(L, "Argument error: System.createDirectory(directory) takes a directory name as string as argument.");

	// Create the directory.
	mkdir(strPath, 0777);

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_directoryExsist(lua_State *L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "Argument error: System.directoryExsist(directory) takes one argument.");

	// Return true if the directory exsists.
	SceUID exsist = sceIoDopen(luaL_checkstring(L, 1));
	lua_pushboolean(L, exsist >= 0);
	if (exsist >= 0) sceIoDclose(exsist);

    return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_removeDirectory (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "Argument error: System.removeDirectory(directory) takes one argument.");

	// Get the directory argument.
	const char * strPath = luaL_checkstring(L, 1);
	if(!strPath) return luaL_error(L, "Argument error: System.removeDirectory(directory) takes a directory name as string as argument.");

	// Remove the directory.
	rmdir(strPath);

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_fileExsist(lua_State *L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "Argument error: System.fileExsist(filename) takes one argument.");

	// Return true if the file exsists.
	SceUID exsist = sceIoOpen(luaL_checkstring(L, 1), PSP_O_RDONLY, 0777);
	lua_pushboolean(L, exsist >= 0);
	if (exsist >= 0) sceIoClose(exsist);

    return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_removeFile (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "Argument error: System.removeFile(filename) takes one argument.");

	// Get the filename argument.
	const char *strPath = luaL_checkstring(L, 1);
	if(!strPath) return luaL_error(L, "Argument error: System.removeFile(filename) takes a filename as string as argument.");

	// Remove the file.
	remove(strPath);

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_rename (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1 || narg != 2) return luaL_error(L, "Argument error: System.rename(source, destination) takes two arguments.");

	// Get the source and  destination arguments.
	const char *oldName = luaL_checkstring(L, 1);
	const char *newName = luaL_checkstring(L, 2);
	if(!oldName || !newName)
		return luaL_error(L, "Argument error: System.rename(source, destination) takes two filenames as strings as arguments.");

	// TODO: looks like the stdio "rename" doesn't work
	sceIoRename(oldName, newName);

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_Sleep(lua_State *L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "Argument error: System.sleep(ms): takes one argument.");

	// Delay the current thread.
	sceKernelDelayThread(luaL_checkint(L, 1) * 1000);

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Check if the program has requisted a exit.
static int lua_Running (lua_State * L)
{
	// Return the running status.
	lua_pushboolean(L, running());

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Exit the thread.
static int lua_Exit (lua_State * L)
{
	// Exit the thread.
	sceKernelExitGame();

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Determine the state of the Wlan power switch.
static int lua_Wlan (lua_State * L)
{
	// Return the state of the Wlan power switch.
	lua_pushboolean(L, sceWlanGetSwitchState() == 1);

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
static int lua_sioInit(lua_State *L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "Argument error: System.sioInit(baudRate): takes one argument.");

	// Initialise the Serial IO library and return the result.
	lua_pushboolean(L, pspsio_init(luaL_checkint(L, 1)) == 0);

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_sioTerm(lua_State *L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "Argument error: System.sioTerm(): takes no arguments.");

	// Terminate the Serial IO library.
	pspsio_term();

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_sioWrite(lua_State *L)
{
	if (!pspsio_initalized()) return luaL_error(L, "sio not initialized");
	int narg = lua_gettop(L);
	for (int arg = 1; arg <= narg; ++arg)
	{
		if (lua_type(L, arg) == LUA_TNUMBER)
		{
			/* optimization: could be done exactly as for strings */
			char s[32] = "";
			sprintf(s, LUA_NUMBER_FMT, lua_tonumber(L, arg));
			int l = strlen(s);
			pspsio_write(s, l);
		}
		else
		{
			size_t l;
			const char *s = luaL_checklstring(L, arg, &l);
			pspsio_write(s, l);
		}
	}
	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
static int lua_sioRead(lua_State *L)
{
	if (!pspsio_initalized()) return luaL_error(L, "sio not initialized");
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "Argument error: System.sioRead(lenght): takes one argument.");
	int l = luaL_checkint(L, 1);
	if (l > 0)
	{
		int nr;  /* number of chars actually read */
		char *p;
		luaL_Buffer b;
		luaL_buffinit(L, &b);
		p = luaL_prepbuffsize(&b, l);  /* prepare buffer to read whole block */
		nr = pspsio_read(p, l);  /* try to read 'l' chars */
		luaL_addsize(&b, nr);
		luaL_pushresult(&b);  /* close buffer */
	}
	else
		lua_pushnil(L);
	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
static int lua_irdaInit(lua_State *L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "Argument error: System.irdaInit(): takes no arguments.");

	// Initialise the irda library and return the result.
	lua_pushboolean(L, pspirda_init() == 0);

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_irdaTerm(lua_State *L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "Argument error: System.irdaTerm(): takes no arguments.");

	// Terminate the irda library.
	pspirda_term();

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_irdaWrite(lua_State *L)
{
	if (!pspirda_initalized()) return luaL_error(L, "irda not initialized");
	int narg = lua_gettop(L);
	for (int arg = 1; arg <= narg; ++arg)
	{
		if (lua_type(L, arg) == LUA_TNUMBER)
		{
			/* optimization: could be done exactly as for strings */
			char s[32] = "";
			sprintf(s, LUA_NUMBER_FMT, lua_tonumber(L, arg));
			int l = strlen(s);
			pspirda_write(s, l);
		}
		else
		{
			size_t l;
			const char *s = luaL_checklstring(L, arg, &l);
			pspirda_write(s, l);
		}
	}
	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
static int lua_irdaRead(lua_State *L)
{
	if (!pspirda_initalized()) return luaL_error(L, "irda not initialized");
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "Argument error: System.sioRead(lenght): takes one argument.");
	int l = luaL_checkint(L, 1);
	if (l > 0)
	{
		int nr;  /* number of chars actually read */
		char *p;
		luaL_Buffer b;
		luaL_buffinit(L, &b);
		p = luaL_prepbuffsize(&b, l);  /* prepare buffer to read whole block */
		nr = pspirda_read(p, l);  /* try to read 'l' chars */
		luaL_addsize(&b, nr);
		luaL_pushresult(&b);  /* close buffer */
	}
	else
		lua_pushnil(L);
	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
static int lua_irSendCmd(lua_State *L)
{
	int narg = lua_gettop(L);
	if (narg != 3 && narg != 4) return luaL_error(L, "Argument error: System.irSendCmd(type, dev, cmd [,count]): takes three or four arguments.");
	struct sircs_data sd;
	int ret = -1;
	int count = (narg == 4 ? luaL_checkint(L, 4) : 20); // this seems like a good number
	int type = luaL_checkint(L, 1);
	if (count > 0 && (((type & 0x0c) == type) || ((type & 0x0f) == type) || ((type & 0x14) == type)))
	{
		sd.type = type & 0xff; // 12, 15 or 20 bits
		sd.cmd = luaL_checkint(L, 3) & 0x7f; // 7 bit cmd
		sd.dev = luaL_checkint(L, 2) & 0x1fff; // 5, 8 or 13 bit device address
		ret = sceSircsSend(&sd, count);
	}
	lua_pushinteger(L, ret);
	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Define the module table.
static const luaL_Reg mod_lib[] =
{
	{"currentDirectory", lua_currentDirectory},
	{"listDirectory", lua_listDirectory},
	{"createDirectory", lua_createDirectory},
	{"directoryExsist",	lua_directoryExsist},
	{"removeDirectory", lua_removeDirectory},
	{"fileExsist",	lua_fileExsist},
	{"removeFile", lua_removeFile},
	{"rename", lua_rename},

	{"sleep", lua_Sleep},

	{"running", lua_Running},
	{"exit", lua_Exit},

	{"wlan", lua_Wlan},

	{"sioInit", lua_sioInit},
	{"sioTerm", lua_sioTerm},
	{"sioWrite", lua_sioWrite},
	{"sioRead", lua_sioRead},

	{"irdaInit", lua_irdaInit},
	{"irdaTerm", lua_irdaTerm},
	{"irdaWrite", lua_irdaWrite},
	{"irdaRead", lua_irdaRead},

	{"irSendCmd", lua_irSendCmd},
	{NULL, NULL}
};
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Open the PSP lua System libraray.
LUAMOD_API int luaopen_System(lua_State * L)
{
	// Get the current directory path.
	char strPath[C_INT_PATH_LEN] = {};
	getcwd(strPath, C_INT_PATH_LEN);

	// Create the module.
	luaL_newlib(L, mod_lib);

	// Set the SpecialDirectory
	lua_newtable(L);

		SetFieldString("flash", "flash0:")
		SetFieldString("ms", "ms0:")
		SetFieldString("eboot", strPath)
		SetFieldString("saveData", "ms0:/PSP/SAVEDATA")
		SetFieldString("myPhotos", "ms0:/PSP/PHOTO")
		SetFieldString("myMusics", "ms0:/PSP/MUSIC")
		SetFieldString("music", "ms0:/MUSIC")
		SetFieldString("video", "ms0:/VIDEO")
		SetFieldString("picture", "ms0:/PICTURE")
		SetFieldString("intrafont", "flash0:/font")

	lua_setfield(L, -2, "Directory");

    // Open the USB module.
	luaL_requiref(L, LUA_USB_LIBNAME, luaopen_USB, 0);
	lua_setfield(L, -2, LUA_USB_LIBNAME);

	// Open the Memory module.
	luaL_requiref(L, LUA_MEMORY_LIBNAME, luaopen_Memory, 0);
	lua_setfield(L, -2, LUA_MEMORY_LIBNAME);

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
