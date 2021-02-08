// lua player library's ( FileName: Lua_Library.hpp )
// ------------------------------------------------------------------------
// Version: 1.00
// Copyright (c) 2012 M4MSOFT
// ------------------------------------------------------------------------

#ifndef __Lua_Library_hpp
#define __Lua_Library_hpp


#define LUA_DEBUGSCREEN_LIBNAME	"DebugScreen"
#define LUA_CONTROLS_LIBNAME	"Controls"

#define LUA_SYSTEM_LIBNAME	    "System"
#define LUA_UTILITY_LIBNAME	    "Utility"
#define LUA_MEMORY_LIBNAME	    "Memory"
#define LUA_USB_LIBNAME	        "usb"
#define LUA_POWER_LIBNAME	    "Power"
#define LUA_TIMER_LIBNAME	    "Timer"

#define LUA_COLOR_LIBNAME	    "Color"
#define LUA_PALETTE_LIBNAME	    "Palette"
#define LUA_FONT_LIBNAME	    "Font"
#define LUA_BMPFONT_LIBNAME	    "BmpFont"
#define LUA_TEXTURE_LIBNAME	    "Texture"
#define LUA_GL_LIBNAME	        "GL"

//#define LUA_AALIB_LIBNAME	    "Aalib"
#define LUA_SOUND_LIBNAME	    "Sound"



LUAMOD_API int (luaopen_DebugScreen) (lua_State *L);
LUAMOD_API int (luaopen_Controls) (lua_State *L);

LUAMOD_API int (luaopen_System) (lua_State *L);
LUAMOD_API int (luaopen_Utility) (lua_State *L);
LUAMOD_API int (luaopen_Memory) (lua_State *L);
LUAMOD_API int (luaopen_USB) (lua_State * L);
LUAMOD_API int (luaopen_Power) (lua_State *L);
LUAMOD_API int (luaopen_Timer) (lua_State *L);

LUAMOD_API int (luaopen_Color) (lua_State *L);
LUAMOD_API int (luaopen_Palette) (lua_State * L);
LUAMOD_API int (luaopen_Font) (lua_State *L);
LUAMOD_API int (luaopen_BmpFont) (lua_State *L);
LUAMOD_API int (luaopen_Texture) (lua_State *L);
LUAMOD_API int (luaopen_GL) (lua_State *L);

//LUAMOD_API int (luaopen_Aalib) (lua_State *L);
LUAMOD_API int (luaopen_Sound) (lua_State *L);


namespace luaplayer
{

	// ------------------------------------------------------------------------
	// Open all Lua player libraries.
	LUALIB_API void (luaL_openplayerlibs) (lua_State *L);
	// ------------------------------------------------------------------------

}

#endif
