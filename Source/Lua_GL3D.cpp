// PSP Lua GL3D ( FileName: Lua_GL3D.cpp )
// ------------------------------------------------------------------------
// Version: 1.01
// Copyright (c) 2014 M4MSOFT
// ------------------------------------------------------------------------

// Include the Lua header files for C++.
#include "luaplayer.hpp"
#include "Lua_GL3D.hpp"
#include "Lua_Color.hpp"
#include "Lua_Palette.hpp"
#include "Lua_Texture.hpp"

// Select the luaplayer objects to use in this file.
using luaplayer::luaL_newusermatatable;
using luaplayer::clamp;

#include <pspkernel.h>
#include "glib2d.h"
#include <pspdisplay.h>
#include <pspgu.h>
#include <pspgum.h>

#include <malloc.h>

// -----------------------------------------------------------------------------------------------------
// Define the function prototypes
static int lua_sceGuClearColor(lua_State *L);
static int lua_sceGuClearDepth(lua_State *L);
static int lua_sceGuClearStencil(lua_State *L);
static int lua_sceGuClear(lua_State *L);

static int lua_sceGumMatrixMode(lua_State *L);
static int lua_sceGumLoadIdentity(lua_State *L);
static int lua_sceGumFastInverse(lua_State *L);
static int lua_sceGumFullInverse(lua_State *L);
static int lua_sceGumOrtho(lua_State *L);
static int lua_sceGumPerspective(lua_State *L);
static int lua_sceGumLookAt(lua_State *L);
static int lua_sceGumTranslate(lua_State *L);
static int lua_sceGumScale(lua_State *L);
static int lua_sceGumRotateXYZ(lua_State *L);
static int lua_sceGumPopMatrix(lua_State *L);
static int lua_sceGumPushMatrix(lua_State *L);

static int lua_sceGetStatus(lua_State *L);
static int lua_sceGuEnable(lua_State *L);
static int lua_sceGuDisable(lua_State *L);

static int lua_sceGuDepthFunc(lua_State *L);
static int lua_sceGuDepthMask(lua_State *L);
static int lua_sceGuDepthOffset(lua_State *L);
static int lua_sceGuDepthRange(lua_State *L);
static int lua_sceGuBlendFunc(lua_State *L);
static int lua_sceGuFrontFace(lua_State *L);
static int lua_sceGuLogicalOp(lua_State *L);
static int lua_sceGuPixelMask(lua_State *L);
static int lua_sceGuShadeModel(lua_State *L);
static int lua_sceGuColor(lua_State *L);
static int lua_sceGuColorFunc(lua_State *L);

static int lua_sceGuTexImage(lua_State *L);
static int lua_sceGuPalette(lua_State *L);
static int lua_sceGuTexLevelMode(lua_State *L);
static int lua_sceGuTexMapMode(lua_State *L);
static int lua_sceGuTexProjMapMode(lua_State *L);
static int lua_sceGuTexSlope(lua_State *L);
static int lua_sceGuTexFunc(lua_State *L);
static int lua_sceGuTexEnvColor(lua_State *L);
static int lua_sceGuTexFilter(lua_State *L);
static int lua_sceGuTexWrap(lua_State *L);
static int lua_sceGuTexScale(lua_State *L);
static int lua_sceGuTexOffset(lua_State *L);
static int lua_sceGuStencilFunc(lua_State *L);
static int lua_sceGuStencilOp(lua_State *L);

static int lua_sceGuAmbientColor(lua_State *L);
static int lua_sceGuAmbient(lua_State *L);
static int lua_sceGuLight(lua_State *L);
static int lua_sceGuLightAtt(lua_State *L);
static int lua_sceGuLightColor(lua_State *L);
static int lua_sceGuLightMode(lua_State *L);
static int lua_sceGuLightSpot(lua_State *L);
static int lua_sceGuColorMaterial(lua_State *L);
static int lua_sceGuMaterial(lua_State *L);
static int lua_sceGuModelColor(lua_State *L);
static int lua_sceGuSpecular(lua_State *L);

static int lua_sceGuFog(lua_State *L);

static int lua_sceGumDrawArray(lua_State *L);

static int lua_start3d(lua_State *L);
static int lua_finish3d(lua_State *L);

static int lua_VerticesNew(lua_State *L);
static int lua_VerticesLoad(lua_State * L);
static int lua_Vertices3D(lua_State * L);
static int lua_VerticesIndex(lua_State * L);
static int lua_VerticesDraw(lua_State * L);
static int lua_VerticesPush(lua_State * L);
static int lua_VerticesGet(lua_State * L);
static int lua_VerticesSet(lua_State * L);
static int lua_VerticesSave(lua_State * L);
static int lua_VerticesFree(lua_State * L);
static int lua_VerticesToString(lua_State * L);
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Set the current clear-color.
static int lua_sceGuClearColor(lua_State *L)
{
	// Read the argument list.
	int argc = lua_gettop(L);
	if (argc > 1) return luaL_error(L, "Argument error: Gu.clearColor([color]) takes zero or one argument.");
	sceGuClearColor(argc == 0 ? 0 : *luaL_checkColor(L, 1));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
// Set the current clear-depth.
static int lua_sceGuClearDepth(lua_State *L)
{
	// Read the argument list.
	int argc = lua_gettop(L);
	if (argc > 1) return luaL_error(L, "Argument error: Gu.clearDepth([number]) takes zero or one argument.");
	sceGuClearDepth(argc == 0 ? 0 : clamp(luaL_checkint(L, 1), 0x0000, 0xffff));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
// Set the current stencil clear value.
static int lua_sceGuClearStencil(lua_State *L)
{
	// Read the argument list.
	int argc = lua_gettop(L);
	if (argc > 1) return luaL_error(L, "Argument error: Gu.clearStencil([number]) takes zero or one argument.");
	sceGuClearStencil(argc == 0 ? 0 : clamp(luaL_checkint(L, 1), 0x00, 0xff));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
// Clear current drawbuffer.
static int lua_sceGuClear(lua_State *L)
{
	// Read the argument list.
	int argc = lua_gettop(L);
	if (argc > 1) return luaL_error(L, "Argument error: Gu.clear([CLBF_ENUM]) takes zero or one argument.");
	sceGuClear(argc == 0 ? (GU_COLOR_BUFFER_BIT | GU_FAST_CLEAR_BIT | GU_DEPTH_BUFFER_BIT) : luaL_checkint(L, 1));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Select which matrix stack to operate on.
static int lua_sceGumMatrixMode(lua_State *L)
{
	// Read the argument list.
	int argc = lua_gettop(L);
	if (argc != 1) return luaL_error(L, "Argument error: Gum.matrixMode(MTRX_ENUM) takes one argument.");
	sceGumMatrixMode(luaL_checkint(L, 1));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
// Load identity matrix.
static int lua_sceGumLoadIdentity(lua_State *L)
{
	// Read the argument list.
	int argc = lua_gettop(L);
	if (argc) return luaL_error(L, "Argument error: Gum.loadIdentity() takes zero arguments.");
	sceGumLoadIdentity();

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
// Invert orthonogal 4x4 matrix.
static int lua_sceGumFastInverse(lua_State *L)
{
	// Read the argument list.
	int argc = lua_gettop(L);
	if (argc) return luaL_error(L, "Argument error: Gum.fastInverse() takes zero arguments.");
	sceGumFastInverse();

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
// Invert 4x4 matrix.
static int lua_sceGumFullInverse(lua_State *L)
{
	// Read the argument list.
	int argc = lua_gettop(L);
	if (argc) return luaL_error(L, "Argument error: Gum.fullInverse() takes zero arguments.");
	sceGumFullInverse();

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
// Set ortho projection matrix
static int lua_sceGumOrtho(lua_State *L)
{
	// Read the argument list.
	int argc = lua_gettop(L);
	if (argc != 6) return luaL_error(L, "Argument error: Gum.ortho(l, r, b, t, n, f) takes six arguments.");
	sceGumOrtho(luaL_checknumber(L, 1), luaL_checknumber(L, 2),
			    luaL_checknumber(L, 3), luaL_checknumber(L, 4),
	            luaL_checknumber(L, 5), luaL_checknumber(L, 6));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
// Set perspective projection matrix.
static int lua_sceGumPerspective(lua_State *L)
{
	// Read the argument list.
	int argc = lua_gettop(L);
	if (argc != 4) return luaL_error(L, "Argument error: Gum.perspective(y, a, n, f) takes four arguments.");
	sceGumPerspective(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
static int lua_sceGumLookAt(lua_State *L)
{
	// Read the argument list.
	int argc = lua_gettop(L);
	if (argc != 9) return luaL_error(L, "Argument error: Gum.lookAt(eX, eY, eZ, cX, cY, cZ, uX, uY, uZ) takes nine arguments.");
	ScePspFVector3 cameraPos;
	ScePspFVector3 lookAtPos;
	ScePspFVector3 upVec;
	cameraPos.x = luaL_checknumber(L, 1); cameraPos.y = luaL_checknumber(L, 2); cameraPos.z = luaL_checknumber(L, 3);
	lookAtPos.x = luaL_checknumber(L, 4); lookAtPos.y = luaL_checknumber(L, 5); lookAtPos.z = luaL_checknumber(L, 6);
	upVec.x     = luaL_checknumber(L, 7); upVec.y     = luaL_checknumber(L, 8); upVec.z     = luaL_checknumber(L, 9);
	sceGumLookAt(&cameraPos, &lookAtPos, &upVec);

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
// Translate coordinate system.
static int lua_sceGumTranslate(lua_State *L)
{
	// Read the argument list.
	int argc = lua_gettop(L);
	if (argc != 3) return luaL_error(L, "Argument error: Gum.translate(x, y, z) takes three arguments.");
	ScePspFVector3 v;
	v.x = luaL_checknumber(L, 1);
	v.y = luaL_checknumber(L, 2);
	v.z = luaL_checknumber(L, 3);
	sceGumTranslate(&v);

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
static int lua_sceGumScale(lua_State *L)
{
	// Read the argument list.
	int argc = lua_gettop(L);
	if (argc != 3) return luaL_error(L, "Argument error: Gum.Scale(x, y, z) takes three arguments.");
	ScePspFVector3 v;
	v.x = luaL_checknumber(L, 1);
	v.y = luaL_checknumber(L, 2);
	v.z = luaL_checknumber(L, 3);
	sceGumScale(&v);

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
// Rotate around all 3 axis in order Z, Y, X.
static int lua_sceGumRotateXYZ(lua_State *L)
{
	// Read the argument list.
	int argc = lua_gettop(L);
	if (argc != 3) return luaL_error(L, "Argument error: Gum.rotateXYZ(Xr, Yr, Zr) takes three arguments.");
	ScePspFVector3 v;
	v.x = luaL_checknumber(L, 1);
	v.y = luaL_checknumber(L, 2);
	v.z = luaL_checknumber(L, 3);
	sceGumRotateXYZ(&v);

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
static int lua_sceGumPopMatrix(lua_State *L)
{
	// Read the argument list.
	int argc = lua_gettop(L);
	if (argc) return luaL_error(L, "Argument error: Gum.popMatrix() takes zero arguments.");
	sceGumPopMatrix();

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
// Load identity matrix.
static int lua_sceGumPushMatrix(lua_State *L)
{
	// Read the argument list.
	int argc = lua_gettop(L);
	if (argc) return luaL_error(L, "Argument error: Gum.pushMatrix() takes zero arguments.");
	sceGumPushMatrix();

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Check GU state.
static int lua_sceGetStatus(lua_State *L)
{
	// Read the argument list.
	int argc = lua_gettop(L);
	if (argc != 1) return luaL_error(L, "Argument error: Gu.state(STATE_ENUM) takes one argument.");
	if (sceGuGetStatus(luaL_checkint(L, 1)) == GU_TRUE)
			lua_pushboolean(L, true);
		else
			lua_pushboolean(L, false);

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
// Enable GU state.
static int lua_sceGuEnable(lua_State *L)
{
	// Read the argument list.
	int argc = lua_gettop(L);
	if (argc != 1) return luaL_error(L, "Argument error: Gu.enable(STATE_ENUM) takes one argument.");
	sceGuEnable(luaL_checkint(L, 1));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
// Disable GU state.
static int lua_sceGuDisable(lua_State *L)
{
	// Read the argument list.
	int argc = lua_gettop(L);
	if (argc != 1) return luaL_error(L, "Argument error: Gu.disable(STATE_ENUM) takes one argument.");
	sceGuDisable(luaL_checkint(L, 1));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Select which depth-test function to use.
static int lua_sceGuDepthFunc(lua_State *L)
{
	// Read the argument list.
	int argc = lua_gettop(L);
	if (argc != 1) return luaL_error(L,"Argument error: Gu.depthFunc(FNC_ENUM) takes one argument.");
	sceGuDepthFunc(luaL_checkint(L, 1));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
// Mask depth buffer writes.
static int lua_sceGuDepthMask(lua_State *L)
{
	// Read the argument list.
	int argc = lua_gettop(L);
	if (argc != 1) return luaL_error(L,"Argument error: Gu.depthMask(mask) takes one argument.");
	sceGuDepthMask(lua_toboolean(L, 1));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
static int lua_sceGuDepthOffset(lua_State *L)
{
	// Read the argument list.
	int argc = lua_gettop(L);
	if (argc != 1) return luaL_error(L,"Argument error: Gu.depthOffset(offset) takes one argument.");
	sceGuDepthOffset(luaL_checkint(L, 1));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
// Set which range to use for depth calculations.
static int lua_sceGuDepthRange(lua_State *L)
{
	// Read the argument list.
	int argc = lua_gettop(L);
	if (argc != 2) return luaL_error(L,"Argument error: Gu.depthRange(near, far) takes two arguments.");
	sceGuDepthRange(luaL_checkint(L, 1), luaL_checkint(L, 2));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
// Set the blending-mode.
static int lua_sceGuBlendFunc(lua_State *L)
{
	// Read the argument list.
	int argc = lua_gettop(L);
	if (argc != 5) return luaL_error(L,"Argument error: Gu.blendFunc(OP_ENUM, sBLD_ENUM, dBLD_ENUM, sfix, dfix) takes five arguments.");
	sceGuBlendFunc(luaL_checkint(L, 1), luaL_checkint(L, 2), luaL_checkint(L, 3), luaL_checkint(L, 4), luaL_checkint(L, 5));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
// Set the current face-order (for culling).
static int lua_sceGuFrontFace(lua_State *L)
{
	// Read the argument list.
	int argc = lua_gettop(L);
	if (argc != 1) return luaL_error(L, "Argument error: Gu.frontFace(FF_ENUM) takes one argument.");
	sceGuFrontFace(luaL_checkint(L, 1));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
// Set color logical operation.
static int lua_sceGuLogicalOp(lua_State *L)
{
	// Read the argument list.
	int argc = lua_gettop(L);
	if (argc != 1) return luaL_error(L, "Argument error: Gu.logicalOp(LOG_ENUM) takes one argument.");
	sceGuLogicalOp(luaL_checkint(L, 1));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
// Set mask for which bits of the pixels to write.
static int lua_sceGuPixelMask(lua_State *L)
{
	// Read the argument list.
	int argc = lua_gettop(L);
	if (argc != 1) return luaL_error(L, "Argument error: Gu.pixelMask(color) takes one argument.");
	sceGuPixelMask(*luaL_checkColor(L, 1));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
// Set how primitives are shaded.
static int lua_sceGuShadeModel(lua_State *L)
{
	// Read the argument list.
	int argc = lua_gettop(L);
	if (argc != 1) return luaL_error(L, "Argument error: Gu.shadeModel(SHADE_ENUM) takes one argument.");
	sceGuShadeModel(luaL_checkint(L, 1));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
// Set current primitive color.
static int lua_sceGuColor(lua_State *L)
{
	// Read the argument list.
	int argc = lua_gettop(L);
	if (argc != 1) return luaL_error(L, "Argument error: Gu.color(color) takes one argument.");
	sceGuColor(*luaL_checkColor(L, 1));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
// Set the color test function.
static int lua_sceGuColorFunc(lua_State *L)
{
	// Read the argument list.
	int argc = lua_gettop(L);
	if (argc != 3) return luaL_error(L, "Argument error: Gu.colorFunc(FNC_ENUM, color, mask) takes three arguments.");
	sceGuColorFunc(luaL_checkint(L, 1), *luaL_checkColor(L, 2), *luaL_checkColor(L, 3));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
// Set stencil function and reference value for stencil testing.
static int lua_sceGuStencilFunc(lua_State *L)
{
	// Read the argument list.
	int argc = lua_gettop(L);
	if (argc != 3) return luaL_error(L, "Argument error: Gu.stencilFunc(FUNC_ENUM, ref, mask) takes three arguments.");
	sceGuStencilFunc(luaL_checkint(L, 1), luaL_checkint(L, 2), luaL_checkint(L, 3));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
// Set the stencil test actions.
static int lua_sceGuStencilOp(lua_State *L)
{
	// Read the argument list.
	int argc = lua_gettop(L);
	if (argc != 3) return luaL_error(L, "Argument error: Gu.stencilOp(fail, zfail, zpass) takes three arguments.");
	sceGuStencilOp(luaL_checkint(L, 1), luaL_checkint(L, 2), luaL_checkint(L, 3));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Set current texturemap.
static int lua_sceGuTexImage(lua_State *L)
{
	// Read the argument list.
	int argc = lua_gettop(L);
	if (argc < 1 || argc > 3) return luaL_error(L, "Argument error: Gu.texImage(texture [,mips [,maxMips]]) takes one two or three arguments.");
	lua_Texture *texPtr = lua_toTexture(L, 1);
	if (texPtr && *texPtr)
	{
		// Load the texture image.
		sceGuTexMode((*texPtr)->psm, (argc == 3 ? clamp(luaL_checkint(L, 3), 0, 8) : 0), 0, (*texPtr)->swizzled);
		sceGuTexImage((argc == 2 ? luaL_checkint(L, 2) : 0), (*texPtr)->tw, (*texPtr)->th, (*texPtr)->tw, (*texPtr)->data);
		// Load the palette.
		switch ((*texPtr)->psm)
		{
			case GU_PSM_T4: // Indexed, 4-bit (2 pixels per byte)
				sceGuClutMode(GU_PSM_8888,0,0xff,0);
				if ((*texPtr)->plt && (*texPtr)->plt->type == G2D_PALETTE_T4)
					sceGuClutLoad(G2D_BLOKS_PSM_T4, (*texPtr)->plt->data);
				else
					sceGuClutLoad(G2D_BLOKS_PSM_T4, g2d_palette_T4);
				break;
			case GU_PSM_T8: // Indexed, 8-bit
				sceGuClutMode(GU_PSM_8888,0,0xff,0);
				if ((*texPtr)->plt && (*texPtr)->plt->type == G2D_PALETTE_T8)
					sceGuClutLoad(G2D_BLOKS_PSM_T8, (*texPtr)->plt->data);
				else
					sceGuClutLoad(G2D_BLOKS_PSM_T8, g2d_palette_T8);
				break;
		}
	}

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
// Set current palette.
static int lua_sceGuPalette(lua_State *L)
{
	// Read the argument list.
	int argc = lua_gettop(L);
	if (argc != 1 ) return luaL_error(L, "Argument error: Gu.palette(plt) takes one argument.");
	lua_Palette *pltPtr = lua_toPalette(L, 1);
	if (pltPtr && *pltPtr && (*pltPtr)->type != G2D_PALETTE_NONE) {
		sceGuClutMode(GU_PSM_8888,0,0xff,0);
		sceGuClutLoad((*pltPtr)->type == G2D_PALETTE_T4 ? G2D_BLOKS_PSM_T4 : G2D_BLOKS_PSM_T8, (*pltPtr)->data);
	}

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// Set texture-level mode (mipmapping).
static int lua_sceGuTexLevelMode(lua_State *L)
{
	// Read the argument list.
	int argc = lua_gettop(L);
	if (argc != 2) return luaL_error(L, "Argument error: Gu.texLevelMode(TLM, bias) takes two arguments.");
	sceGuTexLevelMode(luaL_checkint(L, 1), luaL_checknumber(L, 2));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
// Set the texture-mapping mode..
static int lua_sceGuTexMapMode(lua_State *L)
{
	// Read the argument list.
	int argc = lua_gettop(L);
	if (argc != 3) return luaL_error(L, "Argument error: Gu.texMapMode(TMM, a1, a2) takes three arguments.");
	sceGuTexMapMode(luaL_checkint(L, 1), luaL_checkint(L, 2), luaL_checkint(L, 3));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
// Set texture projection-map mode.
static int lua_sceGuTexProjMapMode(lua_State *L)
{
	// Read the argument list.
	int argc = lua_gettop(L);
	if (argc != 1) return luaL_error(L, "Argument error: Gu.texProjMapMode(TPM) takes one argument.");
	sceGuTexProjMapMode(luaL_checkint(L, 1));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
static int lua_sceGuTexSlope(lua_State *L)
{
	// Read the argument list.
	int argc = lua_gettop(L);
	if (argc != 1) return luaL_error(L, "Argument error: Gu.texSlope(slope) takes one argument.");
	sceGuTexSlope(luaL_checknumber(L, 1));

	return 0; // Number of results.
}

// -----------------------------------------------------------------------------------------------------
// Set how textures are applied.
static int lua_sceGuTexFunc(lua_State *L)
{
	// Read the argument list.
	int argc = lua_gettop(L);
	if (argc != 2) return luaL_error(L, "Argument error: Gu.texFunc(TFX, TCC) takes two arguments.");
	sceGuTexFunc(luaL_checkint(L, 1), luaL_checkint(L, 2));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
// Specify the texture environment color.
static int lua_sceGuTexEnvColor(lua_State *L)
{
	// Read the argument list.
	int argc = lua_gettop(L);
	if (argc != 1) return luaL_error(L, "Argument error: Gu.texEnvColor(color) takes one argument.");
	sceGuTexEnvColor(*luaL_checkColor(L, 1));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
// Set how the texture is filtered.
static int lua_sceGuTexFilter(lua_State *L)
{
	// Read the argument list.
	int argc = lua_gettop(L);
	if (argc != 2) return luaL_error(L, "Argument error: Gu.texFilter(minTFL_ENUM, magTFL_ENUM) takes two arguments.");
	sceGuTexFilter(luaL_checkint(L, 1), luaL_checkint(L, 2));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
// Set if the texture should repeat or clamp.
static int lua_sceGuTexWrap(lua_State *L)
{
	// Read the argument list.
	int argc = lua_gettop(L);
	if (argc != 2) return luaL_error(L, "Argument error: Gu.texWrap(uWRAP_ENUM, vWRAP_ENUM) takes two arguments.");
	sceGuTexWrap(luaL_checkint(L, 1), luaL_checkint(L, 2));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
// Set texture scale
static int lua_sceGuTexScale(lua_State *L)
{
	// Read the argument list.
	int argc = lua_gettop(L);
	if (argc != 2) return luaL_error(L, "Argument error: Gu.texScale(u, v) takes two arguments.");
	sceGuTexScale(luaL_checknumber(L, 1), luaL_checknumber(L, 2));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
// Set texture offset.
static int lua_sceGuTexOffset(lua_State *L)
{
	// Read the argument list.
	int argc = lua_gettop(L);
	if (argc != 2) return luaL_error(L, "Argument error: Gu.texOffset(u, v) takes two arguments.");
	sceGuTexOffset(luaL_checknumber(L, 1), luaL_checknumber(L, 2));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
static int lua_sceGuAmbientColor(lua_State *L)
{
	// Read the argument list.
	int argc = lua_gettop(L);
	if (argc != 1) return luaL_error(L, "Argument error: Gu.ambientColor(color) takes one argument.");
	sceGuAmbientColor(*luaL_checkColor(L, 1));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
static int lua_sceGuAmbient(lua_State *L)
{
	// Read the argument list.
	int argc = lua_gettop(L);
	if (argc != 1) return luaL_error(L, "Argument error: Gu.ambient(color) takes one argument.");
	sceGuAmbient(*luaL_checkColor(L, 1));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
// Set light parameters.
static int lua_sceGuLight(lua_State *L)
{
	// Read the argument list.
	int argc = lua_gettop(L);
	if (argc != 6) return luaL_error(L, "Argument error: Gu.light(idx, LTYPE_ENUM, LCOM_ENUM, x, y, z) takes six arguments.");
	ScePspFVector3 v;
	v.x = luaL_checknumber(L, 4);
	v.y = luaL_checknumber(L, 5);
	v.z = luaL_checknumber(L, 6);
	sceGuLight(luaL_checkint(L, 1), luaL_checkint(L, 2), luaL_checkint(L, 3), &v);

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
// Set light attenuation.
static int lua_sceGuLightAtt(lua_State *L)
{
	// Read the argument list.
	int argc = lua_gettop(L);
	if (argc != 4) return luaL_error(L, "Argument error: Gu.lightAtt(idx, att1, att2, att3) takes four arguments.");
	sceGuLightAtt(luaL_checkint(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
// Set light color.
static int lua_sceGuLightColor(lua_State *L)
{
	// Read the argument list.
	int argc = lua_gettop(L);
	if (argc != 3) return luaL_error(L, "Argument error: Gu.lightColor(idx, LCOM_ENUM, color) takes three arguments.");
	sceGuLightColor(luaL_checkint(L, 1), luaL_checkint(L, 2), *luaL_checkColor(L, 3));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
// Set light mode.
static int lua_sceGuLightMode(lua_State *L)
{
	// Read the argument list.
	int argc = lua_gettop(L);
	if (argc != 1) return luaL_error(L, "Argument error: Gu.lightMode(LMODE_ENUM) takes one argument.");
	sceGuLightMode(luaL_checkint(L, 1));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
// Set spotlight parameters.
static int lua_sceGuLightSpot(lua_State *L)
{
	// Read the argument list.
	int argc = lua_gettop(L);
	if (argc != 6) return luaL_error(L, "Argument error: Gu.lightSpot(idx, x, y, z, exp, cut) takes six arguments.");
	ScePspFVector3 v;
	v.x = luaL_checknumber(L, 2);
	v.y = luaL_checknumber(L, 3);
	v.z = luaL_checknumber(L, 4);
	sceGuLightSpot(luaL_checkint(L, 1), &v, luaL_checknumber(L, 5), luaL_checknumber(L, 6));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
// Set which color components that the material will receive.
static int lua_sceGuColorMaterial(lua_State *L)
{
	// Read the argument list.
	int argc = lua_gettop(L);
	if (argc != 1) return luaL_error(L, "Argument error: Gu.colorMaterial(LCOM_ENUM) takes one argument.");
	sceGuColorMaterial(luaL_checkint(L, 1));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
static int lua_sceGuMaterial(lua_State *L)
{
	// Read the argument list.
	int argc = lua_gettop(L);
	if (argc != 2) return luaL_error(L, "Argument error: Gu.material(LMODE_ENUM, color) takes two arguments.");
	sceGuMaterial(luaL_checkint(L, 1), *luaL_checkColor(L, 2));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
static int lua_sceGuModelColor(lua_State *L)
{
	// Read the argument list.
	int argc = lua_gettop(L);
	if (argc != 4) return luaL_error(L, "Argument error: Gu.modelColor(emissive, ambient, diffuse, specular) takes four arguments.");
	sceGuModelColor(*luaL_checkColor(L, 1), *luaL_checkColor(L, 2), *luaL_checkColor(L, 3), *luaL_checkColor(L, 4));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
// Set the specular power for the material.
static int lua_sceGuSpecular(lua_State *L)
{
	// Read the argument list.
	int argc = lua_gettop(L);
	if (argc != 1) return luaL_error(L, "Argument error: Gu.Specular(power) takes one argument.");
	sceGuSpecular(luaL_checknumber(L, 1));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Set Fog.
static int lua_sceGuFog(lua_State *L)
{
	// Read the argument list.
	int argc = lua_gettop(L);
	if (argc != 3) return luaL_error(L, "Argument error: Gu.fog(near, far, color) takes three argument.");
	sceGuFog(luaL_checknumber(L, 1), luaL_checknumber(L, 2), *luaL_checkColor(L, 3));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
static int lua_sceGumDrawArray(lua_State *L) {
	// Read the argument list.
	int argc = lua_gettop(L);
	if (argc != 3 && argc != 4) return luaL_error(L, "Argument error: Gu.drawArray(prim, vtype, table, indices) takes three or four arguments.");

	int prim = luaL_checkint(L, 1);
	int vtype = luaL_checkint(L, 2);
	if (lua_type(L, 3) != LUA_TTABLE) return luaL_error(L, "vertices table missing");
	int n = luaL_len(L, 3);
	int n_indices = 0;

	if (argc == 4 || (vtype & GU_INDEX_16BIT)) {
		if (lua_type(L, 4) != LUA_TTABLE) return luaL_error(L, "indices table missing");
		n_indices = luaL_len(L, 4);
		if (n_indices == 0) return luaL_error(L, "wrong number of index components");
	}

	int quads = 0;
	int colorLuaIndex = -1;
	if (vtype & GU_TEXTURE_32BITF) quads += 2;
	if (vtype & GU_COLOR_8888) {
		quads++;
		colorLuaIndex = quads;
	}
	if (vtype & GU_NORMAL_32BITF) quads += 3;
	if (vtype & GU_VERTEX_32BITF) quads += 3;

	void* vertices = memalign(16, n * quads*4);
	float* vertex = (float*) vertices;
	int i;
	for (i = 1; i <= n; ++i)
	{
		// get vertice table
		lua_rawgeti(L, 3, i);
		int n2 = luaL_len(L, -1);
		if (n2 != quads)
		{
			free(vertices);
			return luaL_error(L, "wrong number of vertex components");
		}
		int j;
		for (j = 1; j <= n2; ++j)
		{
			lua_rawgeti(L, -1, j);
			if (j != colorLuaIndex)
				*vertex = lua_tonumber(L, -1);
			else
				*((lua_Color*) vertex) = (lua_isColor(L, -1) ? *luaL_checkColor(L, -1) : 0x00000000);
			lua_pop(L, 1);  // removes 'value'
			vertex++;
		}

		// remove vertice table
		lua_pop(L, 1);
	}

	// build indices array.
	if (n_indices)
	{
		void* indices = memalign(16, n_indices * sizeof(unsigned short));
		unsigned short* index = (unsigned short*) indices;
		for (i = 1; i <= n_indices; ++i)
		{
			lua_rawgeti(L, 4, i); // get index 'value'
			*index = lua_tounsigned(L, -1);
			lua_pop(L, 1); // removes 'value'
			index++;
		}
		sceKernelDcacheWritebackInvalidateAll();
		sceGumDrawArray(prim, vtype | GU_INDEX_16BIT, n_indices, indices, vertices);
		free(indices);
		free(vertices);
		return 0; // Number of results.
	}

	sceKernelDcacheWritebackInvalidateAll();
	sceGumDrawArray(prim, vtype, n, NULL, vertices);
	free(vertices);

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Start 3D rendering
static int lua_start3d(lua_State *L)
{
	// Read the argument list.
	int argc = lua_gettop(L);
	if (argc != 0) return luaL_error(L, "Argument error: Gu.start3d() takes no arguments.");
	g2dStartContext();
	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
// Finish 3D rendering
static int lua_finish3d(lua_State *L)
{
	// Read the argument list.
	int argc = lua_gettop(L);
	if (argc != 0) return luaL_error(L, "Argument error: Gu.finish3d() takes no arguments.");
	g2dFinish();
	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Lua_Vertex userdata functions start.
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Create a new vertices list.
static int lua_VerticesNew(lua_State *L)
{
	// Read the argument list.
	int argc = lua_gettop(L);
	if (argc != 3 && argc != 4) return luaL_error(L, "Argument error: Gum.verticesNew(prim, vtype, table, indices) takes three or four arguments.");
	lua_gc(L, LUA_GCCOLLECT, 0);

	int prim = luaL_checkint(L, 1);
	int vtype = luaL_checkint(L, 2);
	if (lua_type(L, 3) != LUA_TTABLE) return luaL_error(L, "vertices table missing");
	int n = luaL_len(L, 3);
	int n_indices = 0;

	if (argc == 4) {
		if (lua_type(L, 4) != LUA_TTABLE) return luaL_error(L, "indices table missing");
		n_indices = luaL_len(L, 4);
		if (n_indices == 0) return luaL_error(L, "wrong number of index components");
	}

	int quads = 0;
	int colorLuaIndex = -1;
	if (vtype & GU_TEXTURE_32BITF) quads += 2;
	if (vtype & GU_COLOR_8888) {quads++; colorLuaIndex = quads;}
	if (vtype & GU_NORMAL_32BITF) quads += 3;
	if (vtype & GU_VERTEX_32BITF) quads += 3;

	gumVertices *Vertices = gl3dVerticesCreate(prim, vtype, quads, n, n_indices);
	if (Vertices == NULL) return luaL_error(L, "Can't create the Vertices list");
	float* vertex = (float*) (Vertices->data);

	int i;
	for (i = 1; i <= n; ++i)
	{
		// get vertice table
		lua_rawgeti(L, 3, i);
		int n2 = luaL_len(L, -1);
		if (n2 != quads)
		{
			gl3dVerticesFree(&Vertices);
			return luaL_error(L, "wrong number of vertex components");
		}
		int j;
		for (j = 1; j <= n2; ++j)
		{
			lua_rawgeti(L, -1, j);
			if (j != colorLuaIndex)
				*vertex = lua_tonumber(L, -1);
			else
				*((lua_Color*) vertex) = (lua_isColor(L, -1) ? *luaL_checkColor(L, -1) : 0x00000000);
			lua_pop(L, 1);  // removes 'value'
			vertex++;
		}

		// remove vertices table
		lua_pop(L, 1);
	}

	// build indices array.
	if (n_indices)
	{
		Vertices->vtype |= GU_INDEX_16BIT;
		unsigned short* index = (unsigned short*) (Vertices->data_index);
		for (i = 1; i <= n_indices; ++i)
		{
			lua_rawgeti(L, 4, i); // get index 'value'
			*index = lua_tounsigned(L, -1);
			lua_pop(L, 1); // removes 'value'
			index++;
		}
	}

	// Return the Vertices list.
	(*lua_pushVertices(L)) = Vertices;

	return 1; // Number of results.
}
// ----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// load a vertices list from a file.
static int lua_VerticesLoad(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "Gum.VerticesLoad(path): takes one argument.");
	lua_gc(L, LUA_GCCOLLECT, 0);

	// Load the vertices list.
	gumVertices* Vertices = gl3dVerticesLoad(luaL_checkstring(L, 1));

	// Return the Texture.
	if (Vertices)
		(*lua_pushVertices(L)) = Vertices;
	else
		lua_pushnil(L);

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Set Vertex type of the Vertices list to 3D.
static int lua_Vertices3D(lua_State * L)
{
	// Read the argument list.
	int argc = lua_gettop(L);
	if (argc != 2) return luaL_error(L, "Argument error: Gum.VerticesType3D([vertices], enable) takes two argument.");
	lua_Vertices Vertices = *luaL_checkVertex(L, 1);
	int enable = lua_toboolean(L, 2);
	gl3dVerticesDisable(Vertices, (enable ? GU_TRANSFORM_2D : GU_TRANSFORM_3D), 0);
	gl3dVerticesEnable(Vertices, (enable ? GU_TRANSFORM_3D : GU_TRANSFORM_2D), 0);

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Enable or disable the indices list.
static int lua_VerticesIndex(lua_State * L)
{
	// Read the argument list.
	int argc = lua_gettop(L);
	if (argc < 1 || argc > 4) return luaL_error(L, "Argument error: Gum.VerticesIndex([vertices] [,n ,len] [,enable]) takes 1,2,3 or 4 arguments.");
	lua_Vertices Vertices = *luaL_checkVertex(L, 1);
	if (Vertices == NULL || Vertices->count_index == 0) return luaL_error(L, "Error: the index table is missing");
	switch (argc) {
		case 4: // Enable or disable the VERT_INDEX_16BIT.
			gl3dVerticesDisable(Vertices, 0, (lua_toboolean(L, 4) ? 0 : GU_INDEX_16BIT));
			gl3dVerticesEnable(Vertices, 0, (lua_toboolean(L, 4) ? GU_INDEX_16BIT : 0));
		case 3: // Set the n and len values.
			Vertices->n = clamp(luaL_checkunsigned(L, 2), 0, Vertices->count_index);
			Vertices->len = clamp(luaL_checkunsigned(L, 3) + Vertices->n, Vertices->n, Vertices->count_index) - Vertices->n;
			break;
		case 2: // Enable or disable the VERT_INDEX_16BIT.
			gl3dVerticesDisable(Vertices, 0, (lua_toboolean(L, 2) ? 0 : GU_INDEX_16BIT));
			gl3dVerticesEnable(Vertices, 0, (lua_toboolean(L, 2) ? GU_INDEX_16BIT : 0));
			break;
		case 1: // Return the n and len values.
			lua_pushnumber(L, Vertices->n);
			lua_pushnumber(L, Vertices->len);
			return 2; // Number of results.
		default: return luaL_error(L, "Argument error: Gum.VerticesSetIndex([vertices] [,n ,len] [,enable]) takes 1,2,3 or 4 arguments.");
	}

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Draw the Vertices list.
static int lua_VerticesDraw(lua_State * L)
{
	// Read the argument list.
	int argc = lua_gettop(L);
	if (argc != 1) return luaL_error(L, "Argument error: Gum.verticesDraw([vertices]) takes one argument.");
	lua_Vertices Vertices = *luaL_checkVertex(L, 1);
	sceKernelDcacheWritebackInvalidateAll();
	if (Vertices->vtype & GU_INDEX_16BIT && Vertices->count_index)
		sceGumDrawArray(Vertices->prim, Vertices->vtype, Vertices->len, ((unsigned short*)(Vertices->data_index)) + Vertices->n, Vertices->data);
	else
		sceGumDrawArray(Vertices->prim, Vertices->vtype, Vertices->count, NULL, Vertices->data);

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Save the Vertices list.
static int lua_VerticesPush(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 1) return luaL_error(L, "Gum.VerticesPush([vertices]): takes one arguments.");
	lua_Vertices Vertices = *luaL_checkVertex(L, 1);
	if (Vertices == NULL) return luaL_error(L, "Can't read the vertices list");
	float* vertex = (float*) (Vertices->data);

	int prim = Vertices->prim;
	int vtype = Vertices->vtype;
	int n = Vertices->count;
	int n_indices = Vertices->count_index;

	int quads = 0;
	int colorLuaIndex = -1;
	if (vtype & GU_TEXTURE_32BITF) quads += 2;
	if (vtype & GU_COLOR_8888) {quads++; colorLuaIndex = quads;}
	if (vtype & GU_NORMAL_32BITF) quads += 3;
	if (vtype & GU_VERTEX_32BITF) quads += 3;

	lua_pushinteger(L, prim); // First result.
	lua_pushinteger(L, vtype); // Second result.

	// Creates a new empty vertices table and pushes it onto the stack.
	lua_newtable(L);

	int i; // Fill the vertices table
	for (i = 1; i <= n; ++i)
	{
		// push key for vertex entry.
		lua_pushnumber(L, i);
		// Creates a new empty vertex table and pushes it onto the stack.
		lua_newtable(L);
		int j; // Fill the vertex table.
		for (j = 1; j <= quads; ++j)
		{
			lua_pushnumber(L, j);
			if (j != colorLuaIndex)
				lua_pushnumber(L, *vertex);
			else
				(*lua_pushColor(L)) = *((lua_Color*) vertex);
			lua_settable(L, -3);
			vertex++;
		}
		lua_settable(L, -3);
	}

	if (n_indices)
	{
		unsigned short* indices = (unsigned short*) (Vertices->data_index);
		// Creates a new empty indices table and pushes it onto the stack.
		lua_newtable(L);
		for (i = 1; i <= n_indices; ++i)
		{
			// push key for vertex entry.
			lua_pushnumber(L, i);
			lua_pushnumber(L, *indices);
			lua_settable(L, -3);
			indices++;
		}
	}

	return n_indices ? 4 : 3; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Get the vertex components.
static int lua_VerticesGet(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 2) return luaL_error(L, "Gum.VerticesGet([vertices], n): takes two arguments.");
	lua_Vertices Vertices = *luaL_checkVertex(L, 1);
	if (Vertices == NULL) return luaL_error(L, "Can't read the vertices list");
	float* vertex = (float*) (Vertices->data);
	int vtype = Vertices->vtype;
	int n = clamp(luaL_checkint(L, 2), 0, Vertices->count-1);

	int quads = 0;
	int colorLuaIndex = -1;
	if (vtype & GU_TEXTURE_32BITF) quads += 2;
	if (vtype & GU_COLOR_8888) {quads++; colorLuaIndex = quads;}
	if (vtype & GU_NORMAL_32BITF) quads += 3;
	if (vtype & GU_VERTEX_32BITF) quads += 3;

	int j; // Push the vertex components.
	vertex += n*quads;
	for (j = 1; j <= quads; ++j)
	{
		if (j != colorLuaIndex)
			lua_pushnumber(L, *vertex);
		else
			(*lua_pushColor(L)) = *((lua_Color*) vertex);
		vertex++;
	}

	return quads; // (...) Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Set the vertex components.
static int lua_VerticesSet(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg < 2) return luaL_error(L, "Gum.VerticesSet([vertices], n, ...): takes two or more arguments.");
	lua_Vertices Vertices = *luaL_checkVertex(L, 1);
	if (Vertices == NULL) return luaL_error(L, "Can't read the vertices list");
	float* vertex = (float*) (Vertices->data);
	int vtype = Vertices->vtype;
	int n = clamp(luaL_checkint(L, 2), 0, Vertices->count-1);

	int quads = 0;
	int colorLuaIndex = -1;
	if (vtype & GU_TEXTURE_32BITF) quads += 2;
	if (vtype & GU_COLOR_8888) {quads++; colorLuaIndex = quads;}
	if (vtype & GU_NORMAL_32BITF) quads += 3;
	if (vtype & GU_VERTEX_32BITF) quads += 3;

	// Check vertex components.
	if (narg-2 != quads) return luaL_error(L, "wrong number of vertex components");

	int j; // Set vertex components.
	vertex += n*quads;
	for (j = 1; j <= quads; ++j)
	{
		if (j != colorLuaIndex)
			*vertex = lua_tonumber(L, j+2);
		else
			*((lua_Color*) vertex) = (lua_isColor(L, j+2) ? *luaL_checkColor(L, j+2) : 0x00000000);
		vertex++;
	}

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Save the Vertices list.
static int lua_VerticesSave(lua_State * L)
{
	// Read the argument list.
	int narg = lua_gettop(L);
	if (narg != 2) return luaL_error(L, "Gum.VerticesSave([vertices], filename): takes two arguments.");

    // Save the Vertices list.
	gl3dVerticesSave(luaL_checkstring(L, 2), *luaL_checkVertex(L, 1));

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Free a Vertices list.
static int lua_VerticesFree(lua_State * L)
{
	// Get the Vertices Pointer.
	lua_Vertices *vertPtr = lua_toVertices(L, 1);

	// Free the Vertices Pointer and set the Pointer to NULL.
	if (vertPtr && *vertPtr)
		gl3dVerticesFree(vertPtr);

	return 0; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static int lua_VerticesToString(lua_State * L)
{
	// Get the texture Pointer.
	lua_Vertices *vertPtr = lua_toVertices(L, 1);

	// Return the texture info string.
	if (vertPtr && *vertPtr)
		lua_pushfstring(L, "Vertices: (%p) [%d, %d, %d]", (*vertPtr)->data, (*vertPtr)->prim, (*vertPtr)->vtype, (*vertPtr)->count);
	else
		lua_pushstring(L, "nil");

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Push the Vertices userdata.
lua_Vertices* lua_pushVertices(lua_State * L)
{
	lua_Vertices* newvalue = (lua_Vertices*) lua_newuserdata(L, sizeof(lua_Vertices));
	luaL_setmetatable(L, LUA_VERTICESHANDLE);
	return newvalue;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Define the Vertices userdata metatable.
static const luaL_Reg Vertices_lib[] =
{
	{"type3D",lua_Vertices3D},
	{"index", lua_VerticesIndex},
	{"draw", lua_VerticesDraw},
	{"push", lua_VerticesPush},
	{"get", lua_VerticesGet},
	{"set", lua_VerticesSet},
	{"save",lua_VerticesSave},
	//{"free", lua_TextureFree},

	{"__gc", lua_VerticesFree},
	{"__tostring", lua_VerticesToString},
	{NULL, NULL}
};
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Lua_Vertex userdata functions end.
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Define the module table.
static const luaL_Reg Gu_functions[] =
{
	{"clearColor", lua_sceGuClearColor},
	{"clearDepth", lua_sceGuClearDepth},
	{"clearStencil", lua_sceGuClearStencil},
	{"clear", lua_sceGuClear},

	{"state", lua_sceGetStatus},
	{"enable", lua_sceGuEnable},
	{"disable", lua_sceGuDisable},

	{"depthFunc", lua_sceGuDepthFunc},
	{"depthMask", lua_sceGuDepthMask},
	{"depthOffset", lua_sceGuDepthOffset},
	{"depthRange", lua_sceGuDepthRange},
	{"blendFunc", lua_sceGuBlendFunc},
	{"frontFace", lua_sceGuFrontFace},
	{"logicalOp", lua_sceGuLogicalOp},
	{"pixelMask", lua_sceGuPixelMask},
	{"shadeModel", lua_sceGuShadeModel},
	{"color", lua_sceGuColor},
	{"colorFunc", lua_sceGuColorFunc},
	{"stencilFunc", lua_sceGuStencilFunc},
	{"stencilOp", lua_sceGuStencilOp},

	{"texImage", lua_sceGuTexImage},
	{"palette", lua_sceGuPalette},
	{"texLevelMode", lua_sceGuTexLevelMode},
	{"texMapMode", lua_sceGuTexMapMode},
	{"texProjMapMode", lua_sceGuTexProjMapMode},
	{"texSlope", lua_sceGuTexSlope},
	{"texFunc", lua_sceGuTexFunc},
	{"texEnvColor", lua_sceGuTexEnvColor},
	{"texFilter", lua_sceGuTexFilter},
	{"texWrap", lua_sceGuTexWrap},
	{"texScale", lua_sceGuTexScale},
	{"texOffset", lua_sceGuTexOffset},

	{"ambientColor", lua_sceGuAmbientColor},
	{"ambient", lua_sceGuAmbient},
	{"light", lua_sceGuLight},
	{"lightAtt", lua_sceGuLightAtt},
	{"lightColor", lua_sceGuLightColor},
	{"lightMode", lua_sceGuLightMode},
	{"lightSpot", lua_sceGuLightSpot},
	{"colorMaterial", lua_sceGuColorMaterial},
	{"material", lua_sceGuMaterial},
	{"modelColor", lua_sceGuModelColor},
	{"specular", lua_sceGuSpecular},

	{"fog", lua_sceGuFog},

	{"start3d", lua_start3d},
	{"finish3d", lua_finish3d},

	{NULL, NULL}
};
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Define the module table.
static const luaL_Reg Gum_functions[] =
{
	{"matrixMode", lua_sceGumMatrixMode},
	{"loadIdentity", lua_sceGumLoadIdentity},
	{"fastInverse", lua_sceGumFastInverse},
	{"fullInverse", lua_sceGumFullInverse},
	{"ortho", lua_sceGumOrtho},
	{"perspective", lua_sceGumPerspective},
	{"lookAt", lua_sceGumLookAt},
	{"translate", lua_sceGumTranslate},
	{"scale", lua_sceGumScale},
	{"rotateXYZ", lua_sceGumRotateXYZ},
	{"popMatrix", lua_sceGumPopMatrix},
	{"pushMatrix", lua_sceGumPushMatrix},

	{"drawArray", lua_sceGumDrawArray},

	{"verticesNew", lua_VerticesNew},
	{"verticesLoad", lua_VerticesLoad},
	{"verticesType3D", lua_Vertices3D},
	{"VerticesIndex", lua_VerticesIndex},
	{"verticesDraw", lua_VerticesDraw},
	{"verticesPush", lua_VerticesPush},
	{"verticesGet", lua_VerticesGet},
	{"verticesSet", lua_VerticesSet},
	{"verticesSave", lua_VerticesSave},

	{NULL, NULL}
};
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Open the PSP lua Gu libraray.
LUAMOD_API int luaopen_GU(lua_State * L)
{
	// Create the module.
	luaL_newlib(L, Gu_functions);

	// PI, float-sized
	SetFieldValue("PI", GU_PI)

	// Gu Clear Buffer Mask enums
	SetFieldValue("CLBF_COLOR_BUFFER_BIT", GU_COLOR_BUFFER_BIT)
	SetFieldValue("CLBF_STENCIL_BUFFER_BIT", GU_STENCIL_BUFFER_BIT)
	SetFieldValue("CLBF_DEPTH_BUFFER_BIT", GU_DEPTH_BUFFER_BIT)
	SetFieldValue("CLBF_FAST_CLEAR_BIT", GU_FAST_CLEAR_BIT)

	// GU Depth Test Function
	SetFieldValue("FNC_NEVER", GU_NEVER)
	SetFieldValue("FNC_ALWAYS", GU_ALWAYS)
	SetFieldValue("FNC_EQUAL", GU_EQUAL)
	SetFieldValue("FNC_NOTEQUAL", GU_NOTEQUAL)
	SetFieldValue("FNC_LESS", GU_LESS)
	SetFieldValue("FNC_LEQUAL", GU_LEQUAL)
	SetFieldValue("FNC_GREATER", GU_GREATER)
	SetFieldValue("FNC_GEQUAL", GU_GEQUAL)

	// Gu Blending Op enums
	SetFieldValue("OP_ADD", GU_ADD)
	SetFieldValue("OP_SUBTRACT", GU_SUBTRACT)
	SetFieldValue("OP_REVERSE_SUBTRACT", GU_REVERSE_SUBTRACT)
	SetFieldValue("OP_MIN", GU_MIN)
	SetFieldValue("OP_MAX", GU_MAX)
	SetFieldValue("OP_ABS", GU_ABS)

	// Gu Blending Factor
	SetFieldValue("BLD_SRC_COLOR", GU_SRC_COLOR)
	SetFieldValue("BLD_DST_COLOR", GU_DST_COLOR)
	SetFieldValue("BLD_ONE_MINUS_SRC_COLOR", GU_ONE_MINUS_SRC_COLOR)
	SetFieldValue("BLD_ONE_MINUS_DST_COLOR", GU_ONE_MINUS_DST_COLOR)
	SetFieldValue("BLD_SRC_ALPHA", GU_SRC_ALPHA)
	SetFieldValue("BLD_DST_ALPHA", GU_DST_ALPHA)
	SetFieldValue("BLD_ONE_MINUS_SRC_ALPHA", GU_ONE_MINUS_SRC_ALPHA)
	SetFieldValue("BLD_ONE_MINUS_DST_ALPHA", GU_ONE_MINUS_DST_ALPHA)
	SetFieldValue("BLD_FIX", GU_FIX)

	// Front Face Direction enums
	SetFieldValue("FF_CW", GU_CW)
	SetFieldValue("FF_CCW", GU_CCW)

	// Logical operation enums
	SetFieldValue("LOG_CLEAR", GU_CLEAR)
	SetFieldValue("LOG_AND", GU_AND)
	SetFieldValue("LOG_AND_REVERSE", GU_AND_REVERSE)
	SetFieldValue("LOG_COPY", GU_COPY)
	SetFieldValue("LOG_AND_INVERTED", GU_AND_INVERTED)
	SetFieldValue("LOG_NOOP", GU_NOOP)
	SetFieldValue("LOG_XOR", GU_XOR)
	SetFieldValue("LOG_OR", GU_OR)
	SetFieldValue("LOG_NOR", GU_NOR)
	SetFieldValue("LOG_EQUIV", GU_EQUIV)
	SetFieldValue("LOG_INVERTED", GU_INVERTED)
	SetFieldValue("LOG_OR_REVERSE", GU_OR_REVERSE)
	SetFieldValue("LOG_COPY_INVERTED", GU_COPY_INVERTED)
	SetFieldValue("LOG_OR_INVERTED", GU_OR_INVERTED)
	SetFieldValue("LOG_NAND", GU_NAND)
	SetFieldValue("LOG_SET", GU_SET)

	// Shading Model enums
	SetFieldValue("SHADE_FLAT", GU_FLAT)
	SetFieldValue("SHADE_SMOOTH", GU_SMOOTH)

	// Gu Texture Level Mode
	SetFieldValue("TLM_TEXTURE_AUTO", GU_TEXTURE_AUTO)
	SetFieldValue("TLM_TEXTURE_CONST", GU_TEXTURE_CONST)
	SetFieldValue("TLM_TEXTURE_SLOPE", GU_TEXTURE_SLOPE)

	// Gu Texture-mapping mode.
	SetFieldValue("TMM_TEXTURE_COORDS", GU_TEXTURE_COORDS)
	SetFieldValue("TMM_TEXTURE_MATRIX", GU_TEXTURE_MATRIX)
	SetFieldValue("TMM_ENVIRONMENT_MAP", GU_ENVIRONMENT_MAP)

	// Texture Projection Map Mode
	SetFieldValue("TPM_POSITION", GU_POSITION)
	SetFieldValue("TPM_UV", GU_UV)
	SetFieldValue("TPM_NORMALIZED_NORMAL", GU_NORMALIZED_NORMAL)
	SetFieldValue("TPM_NORMAL", GU_NORMAL)

	// Gu Texture Effect enum
	SetFieldValue("TFX_MODULATE", GU_TFX_MODULATE)
	SetFieldValue("TFX_DECAL", GU_TFX_DECAL)
	SetFieldValue("TFX_BLEND", GU_TFX_BLEND)
	SetFieldValue("TFX_REPLACE", GU_TFX_REPLACE)
	SetFieldValue("TFX_ADD", GU_TFX_ADD)

	// Gu Texture Color Component enum
	SetFieldValue("TCC_RGB", GU_TCC_RGB)
	SetFieldValue("TCC_RGBA", GU_TCC_RGBA)

	// Gu Texture Filter enum
	SetFieldValue("TFL_NEAREST", GU_NEAREST)
	SetFieldValue("TFL_LINEAR", GU_LINEAR)
	SetFieldValue("TFL_NEAREST_MIPMAP_NEAREST", GU_NEAREST_MIPMAP_NEAREST)
	SetFieldValue("TFL_LINEAR_MIPMAP_NEAREST", GU_LINEAR_MIPMAP_NEAREST)
	SetFieldValue("TFL_NEAREST_MIPMAP_LINEAR", GU_NEAREST_MIPMAP_LINEAR)
	SetFieldValue("TFL_LINEAR_MIPMAP_LINEAR", GU_LINEAR_MIPMAP_LINEAR)

	// GU Wrap Mode enums
	SetFieldValue("WRAP_REPEAT", GU_REPEAT)
	SetFieldValue("WRAP_CLAMP", GU_CLAMP)

	// GU Stencil Operations.
	SetFieldValue("STENCIL_KEEP", GU_KEEP)
	SetFieldValue("STENCIL_ZERO", GU_ZERO)
	SetFieldValue("STENCIL_REPLACE", GU_REPLACE)
	SetFieldValue("STENCIL_INVERT", GU_INVERT)
	SetFieldValue("STENCIL_INCR", GU_INCR)
	SetFieldValue("STENCIL_DECR", GU_DECR)

	// GU States enums
	SetFieldValue("STATE_ALPHA_TEST", GU_ALPHA_TEST)
	SetFieldValue("STATE_DEPTH_TEST", GU_DEPTH_TEST)
	SetFieldValue("STATE_SCISSOR_TEST", GU_SCISSOR_TEST)
	SetFieldValue("STATE_STENCIL_TEST", GU_STENCIL_TEST)
	SetFieldValue("STATE_BLEND", GU_BLEND)
	SetFieldValue("STATE_CULL_FACE", GU_CULL_FACE)
	SetFieldValue("STATE_DITHER", GU_DITHER)
	SetFieldValue("STATE_FOG", GU_FOG)
	SetFieldValue("STATE_CLIP_PLANES", GU_CLIP_PLANES)
	SetFieldValue("STATE_TEXTURE_2D", GU_TEXTURE_2D)
	SetFieldValue("STATE_LIGHTING", GU_LIGHTING)
	SetFieldValue("STATE_LIGHT0", GU_LIGHT0)
	SetFieldValue("STATE_LIGHT1", GU_LIGHT1)
	SetFieldValue("STATE_LIGHT2", GU_LIGHT2)
	SetFieldValue("STATE_LIGHT3", GU_LIGHT3)
	SetFieldValue("STATE_LINE_SMOOTH", GU_LINE_SMOOTH)
	SetFieldValue("STATE_PATCH_CULL_FACE", GU_PATCH_CULL_FACE)
	SetFieldValue("STATE_COLOR_TEST", GU_COLOR_TEST)
	SetFieldValue("STATE_COLOR_LOGIC_OP", GU_COLOR_LOGIC_OP)
	SetFieldValue("STATE_FACE_NORMAL_REVERSE", GU_FACE_NORMAL_REVERSE)
	SetFieldValue("STATE_PATCH_FACE", GU_PATCH_FACE)
	SetFieldValue("STATE_FRAGMENT_2X", GU_FRAGMENT_2X)

	// Gu Light Type enums
	SetFieldValue("LTYPE_DIRECTIONAL", GU_DIRECTIONAL)
	SetFieldValue("LTYPE_POINTLIGHT", GU_POINTLIGHT)
	SetFieldValue("LTYPE_SPOTLIGHT", GU_SPOTLIGHT)

	// Light Components enums
	SetFieldValue("LCOM_AMBIENT", GU_AMBIENT)
	SetFieldValue("LCOM_DIFFUSE", GU_DIFFUSE)
	SetFieldValue("LCOM_SPECULAR", GU_SPECULAR)
	SetFieldValue("LCOM_AMBIENT_AND_DIFFUSE", GU_AMBIENT_AND_DIFFUSE)
	SetFieldValue("LCOM_DIFFUSE_AND_SPECULAR", GU_DIFFUSE_AND_SPECULAR)
	SetFieldValue("LCOM_UNKNOWN_LIGHT_COMPONENT", GU_UNKNOWN_LIGHT_COMPONENT)

	// Light modes enums
	SetFieldValue("LMODE_SINGLE_COLOR", GU_SINGLE_COLOR)
	SetFieldValue("LMODE_SEPARATE_SPECULAR_COLOR", GU_SEPARATE_SPECULAR_COLOR)

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Open the PSP lua Gu libraray.
LUAMOD_API int luaopen_GUM(lua_State * L)
{
	// Create the module.
	luaL_newlib(L, Gum_functions);

	// Create the Vertices user metatable.
	luaL_newusermatatable(L, LUA_VERTICESHANDLE, Vertices_lib);

	// Create a dummy object.
	(*lua_pushVertices(L)) = NULL;
	lua_setfield(L, -2, "object");

	// Gu Matrix modes enums
	SetFieldValue("MTRX_PROJECTION", GU_PROJECTION)
	SetFieldValue("MTRX_VIEW", GU_VIEW)
	SetFieldValue("MTRX_MODEL", GU_MODEL)
	SetFieldValue("MTRX_TEXTURE", GU_TEXTURE)

	// GU Primitive types enums
	SetFieldValue("PRIM_POINTS", GU_POINTS)
	SetFieldValue("PRIM_LINES", GU_LINES)
	SetFieldValue("PRIM_LINE_STRIP", GU_LINE_STRIP)
	SetFieldValue("PRIM_TRIANGLES", GU_TRIANGLES)
	SetFieldValue("PRIM_TRIANGLE_STRIP", GU_TRIANGLE_STRIP)
	SetFieldValue("PRIM_TRIANGLE_FAN", GU_TRIANGLE_FAN)
	SetFieldValue("PRIM_SPRITES", GU_SPRITES)

	// Gu Vertex Declarations enums
	//SetFieldValue("VERT_TEXTURE_8BIT", GU_TEXTURE_8BIT)
	//SetFieldValue("VERT_TEXTURE_16BIT", GU_TEXTURE_16BIT)
	//SetFieldValue("VERT_TEXTURE_32BITF", GU_TEXTURE_32BITF)
	SetFieldValue("VERT_TEXTURE_BITS", GU_TEXTURE_BITS)

	//SetFieldValue("VERT_COLOR_5650", GU_COLOR_5650)
	//SetFieldValue("VERT_COLOR_5551", GU_COLOR_5551)
	//SetFieldValue("VERT_COLOR_4444", GU_COLOR_4444)
	//SetFieldValue("VERT_COLOR_8888", GU_COLOR_8888)
	SetFieldValue("VERT_COLOR_BITS", GU_COLOR_BITS)

	//SetFieldValue("VERT_NORMAL_8BIT", GU_NORMAL_8BIT)
	//SetFieldValue("VERT_NORMAL_16BIT", GU_NORMAL_16BIT)
	//SetFieldValue("VERT_NORMAL_32BITF", GU_NORMAL_32BITF)
	SetFieldValue("VERT_NORMAL_BITS", GU_NORMAL_BITS)

	//SetFieldValue("VERT_VERTEX_8BIT", GU_VERTEX_8BIT)
	//SetFieldValue("VERT_VERTEX_16BIT", GU_VERTEX_16BIT)
	//SetFieldValue("VERT_VERTEX_32BITF", GU_VERTEX_32BITF)
	SetFieldValue("VERT_VERTEX_BITS", GU_VERTEX_BITS)

	///SetFieldValue("VERT_WEIGHT_8BIT", GU_WEIGHT_8BIT)
	//SetFieldValue("VERT_WEIGHT_16BIT", GU_WEIGHT_16BIT)
	//SetFieldValue("VERT_WEIGHT_32BITF", GU_WEIGHT_32BITF)
	//SetFieldValue("VERT_WEIGHT_BITS", GU_WEIGHT_BITS)

	//SetFieldValue("VERT_INDEX_8BIT", GU_INDEX_8BIT)
	SetFieldValue("VERT_INDEX_16BIT", GU_INDEX_16BIT)
	//SetFieldValue("VERT_INDEX_BITS", GU_INDEX_BITS)

	//SetFieldValue("VERT_WEIGHTS_BITS", GU_WEIGHTS_BITS)
	//SetFieldValue("VERT_VERTICES_BITS", GU_VERTICES_BITS)

	SetFieldValue("VERT_TRANSFORM_3D", GU_TRANSFORM_3D)
	SetFieldValue("VERT_TRANSFORM_2D", GU_TRANSFORM_2D)
	//SetFieldValue("VERT_TRANSFORM_BITS", GU_TRANSFORM_BITS)

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
