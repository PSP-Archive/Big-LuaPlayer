// PSP Lua Template ( FileName: Lua_Template.cpp )
// ------------------------------------------------------------------------
// Version: 1.00
// Copyright (c) 2012 M4MSOFT
// ------------------------------------------------------------------------

// Include C/C++ header files
#include <malloc.h>

// Include the Lua header files for C++.
#include "luaplayer.hpp"
#include "Lua_Template.hpp"

// Select the luaplayer objects to use in this file.
using luaplayer::clamp;
using luaplayer::luaL_newusermatatable;

// -----------------------------------------------------------------------------------------------------
// Define the function prototypes
static int lua_TemplateNew (lua_State * L);
static int lua_TemplateFree (lua_State * L);
static int lua_TemplateToString (lua_State * L);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Create a new Template.
static int lua_TemplateNew (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "Argument error: Template.new() takes no arguments.");

	// Create the new Template.
	lua_Template tmpPtr = (lua_Template) malloc(sizeof(Template));
	if (!tmpPtr) return luaL_error(L, "Error: Can't create the structure");

	// Return the Template.
	(*lua_pushTemplate(L)) = tmpPtr;

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// free the Template.
static int lua_TemplateFree (lua_State * L)
{
	// Get the Template Pointer.
	lua_Template *tmpPtr = lua_toTemplate(L, 1);

	// Free the Template Pointer and set the Pointer to NULL.
	if (tmpPtr && *tmpPtr)
	{
		free(*tmpPtr);
		*tmpPtr = NULL;
	}

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Convert the Template object toa string.
static int lua_TemplateToString (lua_State * L)
{
	// Get the Template Pointer.
	lua_Template *tmpPtr = lua_toTemplate(L, 1);

	// Return the Template info string.
	if (tmpPtr && *tmpPtr)
		lua_pushfstring(L, "Template (%p)", *tmpPtr);
	else
		lua_pushstring(L, "nil");

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Define the module table.
static const luaL_Reg mod_lib[] =
{
	{"new", lua_TemplateNew},
	{"free", lua_TemplateFree},
	{NULL, NULL}
};
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Push the Template userdata.
lua_Template* lua_pushTemplate(lua_State * L)
{
	lua_Template* newvalue = (lua_Template*) lua_newuserdata(L, sizeof(lua_Template));
	luaL_setmetatable(L, LUA_TEMPLATEHANDLE);
	return newvalue;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Define the Template userdata metatable.
static const luaL_Reg Template_lib[] =
{
	{"__gc", lua_TemplateFree},
	{"__tostring", lua_TemplateToString},
	{NULL, NULL}
};
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Open the PSP lua Template libraray.
LUAMOD_API int luaopen_Template(lua_State * L)
{
	// Create the module.
	luaL_newlib(L, mod_lib);

	// Create the Color user metatable.
	luaL_newusermatatable(L, LUA_TEMPLATEHANDLE, Template_lib);

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
