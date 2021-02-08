// PSP Lua GL ( FileName: Lua_GL.hpp )
// ------------------------------------------------------------------------
// Header file for LuaPlayer.
// Version: 1.00
// Copyright (c) 2012 M4MSOFT
// ------------------------------------------------------------------------

// Include the PSP glib2D Header file.
#include "glib2d.h"

// Include the Lua header files for C++.
#include "luaplayer.hpp"
#include "Lua_Texture.hpp"
#include "Lua_Palette.hpp"
#include "Lua_Color.hpp"

// Select the luaplayer objects to use in this file.
using luaplayer::clamp;

// -----------------------------------------------------------------------------------------------------
// Define the lua function prototypes
static int lua_GL_start (lua_State * L);
static int lua_GL_setDrawTarget (lua_State * L);
static int lua_GL_resetDrawTarget (lua_State * L);
static int lua_GL_clear (lua_State * L);
static int lua_GL_clearZ (lua_State * L);

static int lua_GL_beginRects (lua_State * L);
static int lua_GL_beginLines (lua_State * L);
static int lua_GL_beginQuads (lua_State * L);
static int lua_GL_beginPoints (lua_State * L);
static int lua_GL_beginTriangles (lua_State * L);

static int lua_GL_end (lua_State * L);
static int lua_GL_reset (lua_State * L);
static int lua_GL_cleanFinish (lua_State * L);
static int lua_GL_finish (lua_State * L);
static int lua_GL_flip (lua_State * L);
static int lua_GL_waitVblankStart (lua_State * L);
static int lua_GL_add (lua_State * L);
static int lua_GL_push (lua_State * L);
static int lua_GL_pop (lua_State * L);
static int lua_GL_clearStack(lua_State * L);

static int lua_GL_resetGlobalScale(lua_State * L);
static int lua_GL_setGlobalScale(lua_State * L);
static int lua_GL_setCoordInteger(lua_State * L);

static int lua_GL_resetCoord (lua_State * L);
static int lua_GL_setCoordMode (lua_State * L);
static int lua_GL_setCoordXY (lua_State * L);
static int lua_GL_setCoordXYZ (lua_State * L);
static int lua_GL_setCoordXYRelative (lua_State * L);
static int lua_GL_setCoordXYZRelative (lua_State * L);

static int lua_GL_resetCrop (lua_State * L);
static int lua_GL_setCropXY (lua_State * L);
static int lua_GL_setCropWH (lua_State * L);
static int lua_GL_setCrop (lua_State * L);
static int lua_GL_setCropXYRelative (lua_State * L);
static int lua_GL_setCropWHRelative (lua_State * L);
static int lua_GL_setCropRelative (lua_State * L);

static int lua_GL_resetScale(lua_State *L);
static int lua_GL_setScale (lua_State * L);
static int lua_GL_setScaleWH (lua_State * L);
static int lua_GL_setScaleRelative (lua_State * L);
static int lua_GL_setScaleWHRelative (lua_State * L);

static int lua_GL_resetColor (lua_State * L);
static int lua_GL_resetAlpha (lua_State * L);
static int lua_GL_setColor (lua_State * L);
static int lua_GL_setAlpha (lua_State * L);
static int lua_GL_setAlphaRelative (lua_State * L);

static int lua_GL_resetRotation (lua_State * L);
static int lua_GL_setRotationRad (lua_State * L);
static int lua_GL_setRotation (lua_State * L);
static int lua_GL_setRotationRadRelative (lua_State * L);
static int lua_GL_setRotationRelative (lua_State * L);

static int lua_GL_resetRotationCenter (lua_State * L);
static int lua_GL_setRotationCenter (lua_State * L);
static int lua_GL_setRotationCenterRelative (lua_State * L);

static int lua_GL_resetTex (lua_State * L);
static int lua_GL_resetPalette(lua_State * L);
static int lua_GL_setPalette(lua_State * L);
static int lua_GL_setTexRepeat (lua_State * L);
static int lua_GL_setTexTFX(lua_State * L);
static int lua_GL_setTexAlpha(lua_State * L);
static int lua_GL_setTexLinear (lua_State * L);

static int lua_GL_resetScissor (lua_State * L);
static int lua_GL_setScissor (lua_State * L);

static int lua_GL_getCoordXYZ (lua_State * L);
static int lua_GL_getCropXY (lua_State * L);
static int lua_GL_getCropWH (lua_State * L);
static int lua_GL_getScale (lua_State * L);
static int lua_GL_getAlpha (lua_State * L);
static int lua_GL_getRotationRad (lua_State * L);
static int lua_GL_getRotation (lua_State * L);
static int lua_GL_getRotationCenter (lua_State * L);

static int lua_GL_addTex (lua_State * L);
static int lua_GL_addRect (lua_State * L);
static int lua_GL_addLine (lua_State * L);
static int lua_GL_addQuad (lua_State * L);
static int lua_GL_addPoint (lua_State * L);
static int lua_GL_addTriangle (lua_State * L);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Initialize the GU (Note : Any graphical functions MUST be placed AFTER this)
static int lua_GL_start (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "GL.Start(): takes no arguments.");

	// Start filling a new display-context.
	g2dStart();

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Set the GU draw target to a texture.
static int lua_GL_setDrawTarget (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "GL.SetDrawTarget(texture): takes one argument.");

	// Set the GU draw target and return the result.
	lua_pushboolean(L, (g2dSetDrawTarget(*luaL_checkTexture(L, 1)) ? false : true));

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Reset the GU draw target.
static int lua_GL_resetDrawTarget (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "GL.ResetDrawTarget(): takes no arguments.");

	// Reset the GU draw target and return the result.
	lua_pushboolean(L, (g2dResetDrawTarget() ? false : true));

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Clear the screen.
static int lua_GL_clear(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg > 1) return luaL_error(L, "GL.Clear([color]): takes 0 or 1 argument.");

	// Clear the screen.
	g2dColor color = narg == 0 ? 0 : *luaL_checkColor(L, 1);
	g2dClear(color);

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Clear the z buffer.
static int lua_GL_clearZ(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "GL.ClearZ(): takes no arguments.");

	// Clear the z buffer.
	g2dClearZ();

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
static int lua_GL_beginRects(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg > 1) return luaL_error(L, "GL.BeginRects([texture]): takes 0 or 1 argument.");

	if (narg == 1)
	{
		g2dTexture *image = *luaL_checkTexture(L, 1);
		g2dBeginRects(image);
	}
	else
		g2dBeginRects(NULL);

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_GL_beginLines(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg > 1) return luaL_error(L, "GL.BeginLines([line_mode]): takes 0 or 1 argument.");

	int strip = (narg == 0 ? G2D_VOID : (luaL_checkint(L, 1) & G2D_STRIP ? G2D_STRIP : G2D_VOID));
	g2dBeginLines((g2dLine_Mode) strip);

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_GL_beginQuads(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg > 1) return luaL_error(L, "GL.BeginQuads([texture]): takes 0 or 1 argument.");

	if (narg == 1)
	{
		g2dTexture *image = *luaL_checkTexture(L, 1);
		g2dBeginQuads(image);
	}
	else
		g2dBeginQuads(NULL);

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_GL_beginPoints(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "GL.BeginPoints(): takes no arguments.");

	g2dBeginPoints();

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_GL_beginTriangles (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg > 1) return luaL_error(L, "GL.BeginTriangles([triangle_mode]): takes 0 or 1 argument.");

	int fan = (narg == 0 ? G2D_VOID : (luaL_checkint(L, 1) & G2D_FAN ? G2D_FAN : G2D_VOID));
	g2dBeginTriangles((g2dTriangle_Mode) fan);

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
static int lua_GL_end(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "GL.End(): takes no arguments.");

	g2dEnd();

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_GL_reset(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "GL.Reset(): takes no arguments.");

	g2dReset();

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Clean finish the GU and Sync.
static int lua_GL_cleanFinish (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "GL.CleanFinish(): takes no arguments.");

	// Clean finish current display list and go back to the parent context.
	g2dCleanFinish();

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Finish the GU and Sync.
static int lua_GL_finish (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "GL.Finish(): takes no arguments.");

	// Finish current display list and go back to the parent context.
	g2dFinish();

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Flip the screen.
static int lua_GL_flip(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg > 1) return luaL_error(L, "GL.Flip([n]): takes 0 or 1 argument.");

	// Flip the screen.
	g2dFlip(narg == 1 ? luaL_checkint(L, 1) : 1);

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Wait for vertical blank start.
static int lua_GL_waitVblankStart (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg > 1) return luaL_error(L, "GL.WaitVblankStart([n]): takes zero or one argument.");

	// Wait for vertical blank start.
	g2dWaitVblankStart(narg == 1 ? luaL_checkint(L, 1) : 1);

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_GL_add(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "GL.Add(): takes no arguments.");

	g2dAdd();

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_GL_push(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "GL.Push(): takes no arguments.");

	g2dPush();

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_GL_pop(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "GL.Pop(): takes no arguments.");

	g2dPop();

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_GL_clearStack(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "GL.ClearStack(): takes no arguments.");

	g2dClearStack();

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
static int lua_GL_resetGlobalScale(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "GL.ResetGlobalScale((): takes no arguments.");

	g2dResetGlobalScale();

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_GL_setGlobalScale(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "GL.SetGlobalScale(scale): takes one argument.");

	g2dSetGlobalScale(luaL_checknumber(L, 1));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_GL_setCoordInteger(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "GL.SetCoordInteger(use): takes one argument.");

	g2dSetCoordInteger(lua_toboolean(L, 1));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
static int lua_GL_resetCoord(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "GL.ResetCoord: takes no argument.");

	g2dResetCoord();

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_GL_setCoordMode(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "GL.SetCoordMode(mode): takes one argument.");

	int mode = luaL_checkint(L, 1);
	if (mode < G2D_UP_LEFT || mode > G2D_CENTER) return luaL_error(L, "g2d.SetCoordMode(mode): mode must be either g2d.UP_LEFT, g2d.UP_RIGHT, g2d.DOWN_LEFT, g2d.DOWN_RIGHT or g2d.CENTER.");
	g2dSetCoordMode((g2dCoord_Mode) mode);

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_GL_getCoordXYZ(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "GL.GetCoordXYZ(): takes no arguments.");

	// Get x,y,z.
	float x, y, z;
	g2dGetCoordXYZ(&x,&y,&z);

	// Return x,y,z
	lua_pushnumber(L, x);
	lua_pushnumber(L, y);
	lua_pushnumber(L, z);

	return 3; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_GL_setCoordXY(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 2) return luaL_error(L, "GL.SetCoordXY(x,y): takes 2 arguments.");

	g2dSetCoordXY(luaL_checknumber(L, 1), luaL_checknumber(L, 2));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_GL_setCoordXYZ(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 3) return luaL_error(L, "GL.SetCoordXYZ(x,y,z): takes 3 arguments.");

	g2dSetCoordXYZ(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_GL_setCoordXYRelative(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg  != 2) return luaL_error(L, "GL.SetCoordXYRelative(x,y): takes 2 arguments.");

	g2dSetCoordXYRelative(luaL_checknumber(L, 1), luaL_checknumber(L, 2));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_GL_setCoordXYZRelative(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 3) return luaL_error(L, "GL.SetCoordXYZRelative(x,y,z): takes 3 arguments.");

	g2dSetCoordXYZRelative(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
static int lua_GL_resetCrop(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "GL.ResetCrop(): takes no arguments.");

	g2dResetCrop();

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_GL_getCropXY(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "GL.GetCropXY(): takes no arguments.");

	int x, y;
	g2dGetCropXY(&x,&y);

	lua_pushinteger(L, x);
	lua_pushinteger(L, y);

	return 2; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_GL_getCropWH(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "GL.GetCropWH(): takes no arguments.");

	int w, h;
	g2dGetCropWH(&w,&h);

	lua_pushnumber(L, w);
	lua_pushnumber(L, h);

	return 2; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_GL_setCropXY(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 2) return luaL_error(L, "GL.SetCropXY(x, y): takes 2 arguments.");

	g2dSetCropXY(luaL_checknumber(L, 1), luaL_checknumber(L, 2));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_GL_setCropWH(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 2) return luaL_error(L, "GL.SetCropWH(w, h): takes 2 arguments.");

	g2dSetCropWH(luaL_checknumber(L, 1), luaL_checknumber(L, 2));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_GL_setCrop(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 4) return luaL_error(L, "GL.SetCrop(x, y, w, h): takes 4 arguments.");

	g2dSetCropXY(luaL_checknumber(L, 1), luaL_checknumber(L, 2));
	g2dSetCropWH(luaL_checknumber(L, 3), luaL_checknumber(L, 4));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_GL_setCropXYRelative(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 2) return luaL_error(L, "GL.SetCropXYRelative(x, y): takes 2 arguments.");

	g2dSetCropXYRelative(luaL_checknumber(L, 1), luaL_checknumber(L, 2));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_GL_setCropWHRelative(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 2) return luaL_error(L, "GL.SetCropWHRelative(w, h): takes 2 arguments.");

	g2dSetCropWHRelative(luaL_checknumber(L, 1), luaL_checknumber(L, 2));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_GL_setCropRelative(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 2) return luaL_error(L, "GL.SetCropRelative(x, y, w, h): takes 4 arguments.");

	g2dSetCropXYRelative(luaL_checknumber(L, 1), luaL_checknumber(L, 2));
	g2dSetCropWHRelative(luaL_checknumber(L, 3), luaL_checknumber(L, 4));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
static int lua_GL_resetScale(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "GL.ResetScale(): takes no arguments.");

	g2dResetScale();

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_GL_getScale(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "GL.GetScale(): takes no arguments.");

	float w, h;
	g2dGetScaleWH(&w,&h);

	lua_pushnumber(L, w);
	lua_pushnumber(L, h);

	return 2; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_GL_setScale(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 2) return luaL_error(L, "GL.SetScale(w, h): takes 2 arguments.");

	g2dSetScale(luaL_checknumber(L, 1), luaL_checknumber(L, 2));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_GL_setScaleWH(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 2) return luaL_error(L, "GL.SetScaleWH(w, h): takes 2 arguments.");

	g2dSetScaleWH(luaL_checknumber(L, 1), luaL_checknumber(L, 2));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_GL_setScaleRelative(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 2) return luaL_error(L, "GL.SetScaleRelative(w, h): takes 2 arguments.");

	g2dSetScaleRelative(luaL_checknumber(L, 1), luaL_checknumber(L, 2));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_GL_setScaleWHRelative(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 2) return luaL_error(L, "GL.SetScaleWHRelative(w, h): takes 2 arguments.");

	g2dSetScaleWHRelative(luaL_checknumber(L, 1), luaL_checknumber(L, 2));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
static int lua_GL_resetColor(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "GL.ResetColor(): takes no arguments.");

	g2dResetColor();

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_GL_resetAlpha(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "GL.ResetAlpha(): takes no arguments.");

	g2dResetAlpha();

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_GL_getAlpha(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "GL.GetAlpha(): takes no arguments.");

	g2dAlpha alpha;
	g2dGetAlpha(&alpha);

	lua_pushinteger(L, alpha);

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_GL_setColor(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "GL.SetColor(color): takes one argument.");

	g2dSetColor(*luaL_checkColor(L, 1));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_GL_setAlpha(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "GL.SetAlpha(alpha): takes one argument.");

	g2dSetAlpha(0);
	g2dSetAlpha(luaL_checkint(L, 1));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_GL_setAlphaRelative(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "GL.SetAlphaRelative(alpha): takes one argument.");

	g2dSetAlphaRelative(luaL_checkint(L, 1));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
static int lua_GL_resetRotation(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "GL.ResetRotation(): takes no arguments.");

	g2dResetRotation();

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_GL_resetRotationCenter(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "GL.ResetRotationCenter(): takes no arguments.");

	g2dResetRotationCenter();

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_GL_getRotationRad(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "GL.GetRotationRad(): takes no arguments.");

	float rad;
	g2dGetRotationRad(&rad);

	lua_pushnumber(L, rad);

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_GL_getRotation(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "GL.GetRotation(): takes no arguments.");

	float deg;
	g2dGetRotation(&deg);

	lua_pushnumber(L, deg);

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_GL_getRotationCenter(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "GL.GetRotationCenter(): takes no arguments.");

	float rot_x,rot_y;
	g2dGetRotationCenter(&rot_x, &rot_y);

	lua_pushnumber(L, rot_x);
	lua_pushnumber(L, rot_y);

	return 2; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_GL_setRotationCenter(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 2) return luaL_error(L, "GL.SetRotationCenter(x, y): takes one argument.");

	g2dSetRotationCenter(luaL_checknumber(L, 1), luaL_checknumber(L, 2));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_GL_setRotationCenterRelative(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 2) return luaL_error(L, "GL.SetRotationCenterRelative(x, y): takes one argument.");

	g2dSetRotationCenterRelative(luaL_checknumber(L, 1), luaL_checknumber(L, 2));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_GL_setRotationRad(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "GL.SetRotationRad(angle): takes one argument.");

	g2dSetRotationRad(luaL_checknumber(L, 1));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_GL_setRotation(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "GL.SetRotation(angle): takes one argument.");

	g2dSetRotation(luaL_checknumber(L, 1));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_GL_setRotationRadRelative(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "GL.SetRotationRadRelative(angle): takes one argument.");

	g2dSetRotationRadRelative(luaL_checknumber(L, 1));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_GL_setRotationRelative(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "GL.SetRotationRelative(angle): takes one argument.");

	g2dSetRotationRelative(luaL_checknumber(L, 1));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
static int lua_GL_resetTex(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "GL.ResetTex(): takes no arguments.");

	g2dResetTex();

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_GL_resetPalette(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "GL.ResetPalette(): takes no arguments.");

	g2dResetPalette();

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_GL_setPalette(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "GL.ResetTex(plt): takes one argument.");

	// WARNING: Don't free the palette during rendering.
	g2dSetPalette(*luaL_checkPalette(L, 1));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
static int lua_GL_setTexRepeat(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "GL.SetTexRepeat(use): takes one argument (boolean).");

	g2dSetTexRepeat(lua_toboolean(L, 1));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_GL_setTexTFX(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "GL.SetTexTFX(tex_mode): takes one argument.");

	g2dSetTexTFX((g2dTFX_Mode) clamp(luaL_checkint(L, 1), 0, 4));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_GL_setTexAlpha(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "GL.SetTexAlpha(use): takes one argument (boolean).");

	g2dSetTexAlpha(lua_toboolean(L, 1));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_GL_setTexLinear(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "GL.SetTexLinear(use): takes one argument (boolean).");

	g2dSetTexLinear(lua_toboolean(L, 1));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
static int lua_GL_resetScissor(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg) return luaL_error(L, "GL.ResetScissor(): takes no arguments.");

	g2dResetScissor();

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_GL_setScissor(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 4) return luaL_error(L, "GL.SetScissor(x,y,w,h): takes 4 arguments.");

	g2dSetScissor(luaL_checkint(L, 1), luaL_checkint(L, 2), luaL_checkint(L, 3), luaL_checkint(L, 4));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_GL_addTex (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);

	// Select the arguments.
	switch (narg)
	{
		case 10 : g2dSetColor(*luaL_checkColor(L, 10));
		case 9 :
			g2dSetCropWH(luaL_checknumber(L, 7), luaL_checknumber(L, 8));
			g2dSetCropXY(luaL_checknumber(L, 5), luaL_checknumber(L, 6));
			g2dSetScaleWH(luaL_checknumber(L, 3), luaL_checknumber(L, 4));
			g2dSetCoordXYZ(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 9));
			break;
		case 8 : g2dSetCropWH(luaL_checknumber(L, 7), luaL_checknumber(L, 8));
		case 6 : g2dSetCropXY(luaL_checknumber(L, 5), luaL_checknumber(L, 6));
		case 4 : g2dSetScaleWH(luaL_checknumber(L, 3), luaL_checknumber(L, 4));
		case 2 : g2dSetCoordXY(luaL_checknumber(L, 1), luaL_checknumber(L, 2)); break;
		default: return luaL_error(L, "GL.AddTex(x, y [,w ,h [,sx, sy [,sw ,sh [,z [,color]]]]]): takes 2, 4, 6, 8, 9 or 10 arguments.");
	}
	g2dAdd();

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_GL_addRect (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);

	// Select the arguments.
	switch (narg)
	{
		case 6 : g2dSetColor(*luaL_checkColor(L, 6));
		case 5 :
			g2dSetScaleWH(luaL_checknumber(L, 3), luaL_checknumber(L, 4));
			g2dSetCoordXYZ(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 5));
			break;
		case 4 : g2dSetScaleWH(luaL_checknumber(L, 3), luaL_checknumber(L, 4));
		case 2 : g2dSetCoordXY(luaL_checknumber(L, 1), luaL_checknumber(L, 2)); break;
		default: return luaL_error(L, "GL.AddRect(x, y [,w ,h [,z [,color]]]): takes 2, 4, 5 or 6 arguments.");
	}
	g2dAdd();

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_GL_addLine (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);

	// Select the arguments.
	switch (narg)
	{
		case 6 : g2dSetColor(*luaL_checkColor(L, 6));
		case 5 : g2dSetCoordXYZ(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 5));	g2dAdd(); break;
		case 4 : g2dSetCoordXY(luaL_checknumber(L, 1), luaL_checknumber(L, 2)); g2dAdd(); break;
		default: return luaL_error(L, "GL.AddLine(x0, y0, x1, y1 [,z [,color]]): takes 4 to 6 arguments.");
	}
	g2dSetCoordXY(luaL_checknumber(L, 3), luaL_checknumber(L, 4)); g2dAdd();

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_GL_addQuad (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);

	// Select the arguments.
	lua_Number x,y,w,h;
	switch (narg)
	{
		case 6 : g2dSetColor(*luaL_checkColor(L, 6));
		case 5 :
			x = luaL_checknumber(L, 1); y = luaL_checknumber(L, 2);
			w = luaL_checknumber(L, 3); h = luaL_checknumber(L, 4);
			g2dSetCoordXYZ(x, y, luaL_checknumber(L, 5)); g2dAdd();
			g2dSetCoordXY(x+w, y  ); g2dAdd();
			g2dSetCoordXY(x+w, y+h); g2dAdd();
			g2dSetCoordXY(x,   y+h); g2dAdd();
			break;
		case 4 :
			x = luaL_checknumber(L, 1); y = luaL_checknumber(L, 2);
			w = luaL_checknumber(L, 3); h = luaL_checknumber(L, 4);
			g2dSetCoordXY(x,   y  ); g2dAdd();
			g2dSetCoordXY(x+w, y  ); g2dAdd();
			g2dSetCoordXY(x+w, y+h); g2dAdd();
			g2dSetCoordXY(x,   y+h); g2dAdd();
			break;
		default: return luaL_error(L, "GL.AddQuad(x, y ,w ,h [,z [,color]]): 4 to 6 arguments.");
	}

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_GL_addPoint (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);

	// Select the arguments.
	switch (narg)
	{
		case 4 : g2dSetColor(*luaL_checkColor(L, 4));
		case 3 : g2dSetCoordXYZ(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3)); break;
		case 2 : g2dSetCoordXY(luaL_checknumber(L, 1), luaL_checknumber(L, 2)); break;
		default: return luaL_error(L, "GL.AddPoint(x, y [,z [,color]]): takes 2 to 4 arguments.");
	}
	g2dAdd();

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_GL_addTriangle (lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);

	// Select the arguments.
	switch (narg)
	{
		case 8 : g2dSetColor(*luaL_checkColor(L, 8));
		case 7 : g2dSetCoordXYZ(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 7)); g2dAdd(); break;
		case 6 : g2dSetCoordXY(luaL_checknumber(L, 1), luaL_checknumber(L, 2)); g2dAdd(); break;
		default: return luaL_error(L, "GL.AddTriangle(x0, y0, x1, y1, x2, y2 [,z [,color]]): takes 6 to 8 arguments.");
	}
	g2dSetCoordXY(luaL_checknumber(L, 3), luaL_checknumber(L, 4)); g2dAdd();
	g2dSetCoordXY(luaL_checknumber(L, 5), luaL_checknumber(L, 6)); g2dAdd();

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Define the module table.
static const luaL_Reg GL_lib[] =
{
	{"Start", lua_GL_start},
	{"SetDrawTarget", lua_GL_setDrawTarget},
	{"ResetDrawTarget", lua_GL_resetDrawTarget},
	{"Clear", lua_GL_clear},
	{"ClearZ", lua_GL_clearZ},

	{"BeginRects", lua_GL_beginRects},
	{"BeginLines", lua_GL_beginLines},
	{"BeginQuads", lua_GL_beginQuads},
	{"BeginPoints", lua_GL_beginPoints},
	{"BeginTriangles", lua_GL_beginTriangles},

	{"End", lua_GL_end},
	{"Reset", lua_GL_reset},
	{"CleanFinish", lua_GL_cleanFinish},
	{"Finish", lua_GL_finish},
	{"Flip", lua_GL_flip},
	{"WaitVblankStart", lua_GL_waitVblankStart},
	{"Add", lua_GL_add},
	{"Push", lua_GL_push},
	{"Pop", lua_GL_pop},
	{"ClearStack", lua_GL_clearStack},

	{"ResetGlobalScale", lua_GL_resetGlobalScale},
	{"SetGlobalScale", lua_GL_setGlobalScale},
	{"SetCoordInteger", lua_GL_setCoordInteger},

	{"ResetCoord", lua_GL_resetCoord},
	{"SetCoordMode", lua_GL_setCoordMode},
	{"SetCoordXY", lua_GL_setCoordXY},
	{"SetCoordXYZ", lua_GL_setCoordXYZ},
	{"SetCoordXYRelative", lua_GL_setCoordXYRelative},
	{"SetCoordXYZRelative", lua_GL_setCoordXYZRelative},

	{"ResetCrop", lua_GL_resetCrop},
	{"SetCropXY", lua_GL_setCropXY},
	{"SetCropWH", lua_GL_setCropWH},
	{"SetCrop", lua_GL_setCrop},
	{"SetCropXYRealative", lua_GL_setCropXYRelative},
	{"SetCropWHRelative", lua_GL_setCropWHRelative},
	{"SetCropRelative", lua_GL_setCropRelative},

	{"ResetScale", lua_GL_resetScale},
	{"SetScale", lua_GL_setScale},
	{"SetScaleWH", lua_GL_setScaleWH},
	{"SetScaleRelative", lua_GL_setScaleRelative},
	{"SetScaleWHRelative", lua_GL_setScaleWHRelative},

	{"ResetColor", lua_GL_resetColor},
	{"ResetAlpha", lua_GL_resetAlpha},
	{"SetColor", lua_GL_setColor},
	{"SetAlpha", lua_GL_setAlpha},
	{"SetAlphaRelative", lua_GL_setAlphaRelative},

	{"ResetRotation", lua_GL_resetRotation},
	{"SetRotationRad", lua_GL_setRotationRad},
	{"SetRotation", lua_GL_setRotation},
	{"SetRotationRadRelative", lua_GL_setRotationRadRelative},
	{"SetRotationRelative", lua_GL_setRotationRelative},

	{"ResetRotationCenter", lua_GL_resetRotationCenter},
	{"SetRotationCenter", lua_GL_setRotationCenter},
	{"SetRotationCenterRelative", lua_GL_setRotationCenterRelative},

	{"ResetTex", lua_GL_resetTex},
	{"ResetPalette", lua_GL_resetPalette},
	{"SetPalette", lua_GL_setPalette},
	{"SetTexRepeat", lua_GL_setTexRepeat},
	{"SetTexTFX", lua_GL_setTexTFX},
	{"SetTexAlpha", lua_GL_setTexAlpha},
	{"SetTexLinear", lua_GL_setTexLinear},

	{"ResetScissor", lua_GL_resetScissor},
	{"SetScissor", lua_GL_setScissor},

	{"GetCoordXYZ", lua_GL_getCoordXYZ},
	{"GetCropXY", lua_GL_getCropXY},
	{"GetCropWH", lua_GL_getCropWH},
	{"GetScale", lua_GL_getScale},
	{"GetAlpha", lua_GL_getAlpha},
	{"GetRotationRad", lua_GL_getRotationRad},
	{"GetRotation", lua_GL_getRotation},
	{"GetRotationCenter", lua_GL_getRotationCenter},

	{"AddTex", lua_GL_addTex},
	{"AddRect", lua_GL_addRect},
	{"AddLine", lua_GL_addLine},
	{"AddQuad", lua_GL_addQuad},
	{"AddPoint", lua_GL_addPoint},
	{"AddTriangle", lua_GL_addTriangle},

	{NULL,NULL}
};
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Open the PSP lua GL libraray.
LUAMOD_API int luaopen_GL(lua_State * L)
{
	// Initalize the g2d graphics libraray.
	g2dInit();

	// Create the module.
	luaL_newlib(L, GL_lib);

	// Define the Enum Values.
	SetFieldValue("VOID", G2D_VOID)
	//SetFieldValue("SWIZZLE", G2D_SWIZZLE)
	SetFieldValue("VSYNC", G2D_VSYNC)
	SetFieldValue("STRIP", G2D_STRIP)
	SetFieldValue("FAN", G2D_FAN)

	SetFieldValue("UP_LEFT", G2D_UP_LEFT)
	SetFieldValue("UP_RIGHT", G2D_UP_RIGHT)
	SetFieldValue("DOWN_LEFT", G2D_DOWN_LEFT)
	SetFieldValue("DOWN_RIGHT", G2D_DOWN_RIGHT)
	SetFieldValue("CENTER", G2D_CENTER)

	SetFieldValue("PSM_4444", G2D_PSM_4444)
	SetFieldValue("PSM_8888", G2D_PSM_8888)
	SetFieldValue("PSM_T4", G2D_PSM_T4)
	SetFieldValue("PSM_T8", G2D_PSM_T8)

	SetFieldValue("TFX_MODULATE", G2D_TFX_MODULATE)
	SetFieldValue("TFX_DECAL", G2D_TFX_DECAL)
	SetFieldValue("TFX_BLEND", G2D_TFX_BLEND)
	SetFieldValue("TFX_REPLACE", G2D_TFX_REPLACE)
	SetFieldValue("TFX_ADD", G2D_TFX_ADD)

	// Create the handles to the draw, screen and depht buffers.
	(*lua_pushTexture(L)) = &g2d_draw_buffer; lua_setfield(L, -2, "drawBuffer");
	(*lua_pushTexture(L)) = &g2d_disp_buffer; lua_setfield(L, -2, "screenBuffer");
	(*lua_pushTexture(L)) = &g2d_depht_buffer; lua_setfield(L, -2, "dephtBuffer");

	#define SetColorValue(name, r, g, b, a) (*lua_pushColor(L)) = G2D_RGBA(r, g, b, a); lua_setfield(L, -2, (name));

	// Define the default colors.
	lua_newtable(L);
		//(*lua_pushColor(L)) = 0x00000000; lua_pushvalue(L, -1); lua_setfield(L, -3, "Transparent");
		SetColorValue("transparent", 0,   0,   0,   0   ) // 0x00000000
		SetColorValue("black",       0,   0,   0,   255 ) // 0xff000000
		SetColorValue("darkGray",    64,  64,  64,  255 ) // 0xff404040
		SetColorValue("gray",        128, 128, 128, 255 ) // 0xff808080
		SetColorValue("lightGray",   192, 192, 192, 255 ) // 0xffc0c0c0
		SetColorValue("white",       255, 255, 255, 255 ) // 0xffffffff

		//SetColorValue("black",     0,   0,   0,   255) // 0xff000000
		SetColorValue("darkRed",     127, 0,   0,   255) // 0xff00007f
		SetColorValue("red",         255, 0,   0,   255) // 0xff0000ff
		SetColorValue("darkGreen",   0,   127, 0,   255) // 0xff007f00
		SetColorValue("olive",       127, 127, 0,   255) // 0xff007f7f
		SetColorValue("orange",      255, 127, 0,   255) // 0xff007fff
		SetColorValue("green",       0,   255, 0,   255) // 0xff00ff00
		SetColorValue("chartreuse",  127, 255, 0,   255) // 0xff00ff7f
		SetColorValue("yellow",      255, 255, 0,   255) // 0xff00ffff

		SetColorValue("darkBlue",    0,   0,   127, 255) // 0xff7f0000
		SetColorValue("purple",      127, 0,   127, 255) // 0xff7f007f
		SetColorValue("rose",        255, 0,   127, 255) // 0xff7f00ff
		SetColorValue("darkCyan",    0,   127, 127, 255) // 0xff7f7f00
		//SetColorValue("gray",      127, 127, 127, 255) // 0xff7f7f7f
		SetColorValue("lightRed",    255, 127, 127, 255) // 0xff7f7fff
		SetColorValue("springGreen", 0,   255, 127, 255) // 0xff7fff00
		SetColorValue("lightGreen",  127, 255, 127, 255) // 0xff7ffff7
		SetColorValue("ivory",       255, 255, 127, 255) // 0xff7fffff

		SetColorValue("blue",        0,   0,   255, 255) // 0xffff0000
		SetColorValue("violet",      127, 0,   255, 255) // 0xffff00f7
		SetColorValue("magenta",     255, 0,   255, 255) // 0xffff00ff
		SetColorValue("azure",       0,   127, 255, 255) // 0xffff7f00
		SetColorValue("lightBlue",   127, 127, 255, 255) // 0xffff7ff7
		SetColorValue("pink",        255, 127, 255, 255) // 0xffff7fff
		SetColorValue("cyan",        0,   255, 255, 255) // 0xffffff00
		SetColorValue("lightCyan",   127, 255, 255, 255) // 0xfffffff7
		//SetColorValue( 6,  "White",     255, 255, 255, 255) // 0xffffffff
	//SetFieldValue("count", 30)
	lua_setfield(L, -2, "clr");

	// Define the system colors.
	lua_newtable(L);
		SetColorValue("ActiveBorder",        0,   19,  140, 255)
		SetColorValue("ActiveCaption",       0,   72,  241, 255)
		SetColorValue("ActiveCaptionText",   255, 255, 255, 255)
		SetColorValue("AppWorkspace",        128, 128, 128, 255)
		SetColorValue("ButtonFace",          236, 233, 216, 255)
		SetColorValue("ButtonHighlight",     255, 255, 255, 255)
		SetColorValue("ButtonShadow",        172, 168, 153, 255)
		SetColorValue("ButtonBorden",        0,   60,  116, 255)
	    SetColorValue("ButtonSelected",      255, 140, 0,   255)
		SetColorValue("Control",             236, 233, 216, 255)
		SetColorValue("ControlDark",         172, 168, 153, 255)
		SetColorValue("ControlDarkDark",     113, 111, 100, 255)
		SetColorValue("ControlLight",        241, 239, 226, 255)
		SetColorValue("ControlLightLight",   255, 255, 255, 255)
		SetColorValue("ControlText",         0,   0,   0,   255)
		SetColorValue("Desktop",             0,   78,  152, 255)
		SetColorValue("GrayText",            172, 168, 153, 255)
		SetColorValue("Highlight",           49,  106, 197, 255)
		SetColorValue("HighlightText",       255, 255, 255, 255)
		SetColorValue("HotTrack",            0,   0,   128, 255)
		SetColorValue("InactiveBorder",      212, 208, 200, 255)
		SetColorValue("InactiveCaption",     122, 150, 223, 255)
		SetColorValue("InactiveCaptionText", 216, 228, 248, 255)
		SetColorValue("Info",                255, 255, 225, 255)
		SetColorValue("InfoText",            0,   0,   0,   255)
		SetColorValue("Menu",                255, 255, 255, 255)
		SetColorValue("Menuface",            241, 239, 226, 255)
		SetColorValue("MenuBar",             236, 233, 216, 255)
		SetColorValue("MenuDark",            172, 168, 153, 255)
		SetColorValue("MenuText",            0,   0,   0,   255)
		SetColorValue("MenuGrayText",        172, 168, 153, 255)
		SetColorValue("ScrollBar",           212, 208, 200, 255)
		SetColorValue("ScrollBarFace",       195, 213, 253, 255)
		SetColorValue("ScrollBarLight",      255, 255, 255, 255)
		SetColorValue("ScrollBarDark",       77,  97,  133, 255)
		SetColorValue("Window",              255, 255, 255, 255)
		SetColorValue("WindowFrame",         127, 157, 185, 255)
		SetColorValue("WindowText",          0,   0,   0,   255)
	//SetFieldValue("count", 38)
	lua_setfield(L, -2, "clrSystem");

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
