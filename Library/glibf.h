// glibf - A TrueType font library for the PSP ( FileName: glibf.h )
// ------------------------------------------------------------------------
// Version: 1.00
// Copyright (c) 2012 M4MSOFT
//
//	Requirements:	PSP graphics library by Psilocybeing
//				    libpng for PSP
//				    libfreetype2 for PSP
// ------------------------------------------------------------------------

#ifndef GLIBF_H_
#define GLIBF_H_

#ifdef __cplusplus
extern "C" {
#endif

// ------------------------------------------------------------------------
// Include the freetype.h - header file for FreeType 2 library
//
// FreeType high-level API and common types (specification only).
// Copyright 1996-2001, 2002, 2003, 2004, 2005 by David Turner, Robert Wilhelm, and Werner Lemberg.
#include <ft2build.h>
#include FT_FREETYPE_H
// ------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Defines.
#define GLIBF_SIZE_SCALE       (64)
#define GLIBF_SIZE_SHIFT       (6)
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// error codes
#define GLIBF_ERR_CANTINIT	  (-1)	 // problem initializing freetype 2
#define GLIBF_ERR_CANTOPEN	  (-2)  // problem finding or loading .ttf file
#define GLIBF_ERR_MEMORY	  (-3)	 // ran out of memory
#define GLIBF_ERR_INVALIDFONT (-4)	 // attempt to use bad/empty font
#define GLIBF_ERR_BADSIZE	  (-5)	 // requested size out of range
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Structures
typedef struct {
	int x;
	int y;
	int width;
	int height;
} gfReactangle;
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Structures
typedef struct {
	int x;
	int y;
} gfPoint;
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
typedef struct
{
	gfReactangle size;
	gfPoint offset;
	gfPoint advance;
	int error;
} gfGlyph;
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Structures
typedef struct {
  const char *name;
  FT_Face face;
  int size;
  bool points;
  unsigned char *data;
} gfFont;
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Structures
typedef struct {
  int num_charmaps; // The total number of character maps in the bmp font.
  unsigned char charmaps[256]; // table of pointers to a font Glyph slot. Used to scan the list of available charmaps.
  gfGlyph glyph[256]; // The face's associated glyph slot(s).
  int height;
  gfPoint offset;
  g2dTexture *tex;
} gfBmpFont;
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Initializes the library.
//
// @param: void.
//
// @return: void.
//
void gfInit ();
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Shutdowns the library.
//
// @param: void.
//
// @return: void.
//
void gfTerm ();
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Saves the current font state to stack. (Stack Size is 30.)
//
// @param: void.
//
// @return: void.
//
void gfPush ();
// -----------------------------------------------------------------------------------------------------
// Restore the current font state from stack. (Stack Size is 30.)
//
// @param: void.
//
// @return: void.
//
void gfPop ();
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Clear the font state stack.
//
// @param: void.
//
// @return: void.
//
void gfClearStack ();
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Set the current font.
//
// @param: font - A pointer to the font to activate.
//
// @return: void.
//
void gfSetFont (gfFont *font);
// -----------------------------------------------------------------------------------------------------
// Reset the current font.
//
// @param: void.
//
// @return: void.
//
void gfResetFont ();
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Set the current font color.
//
// @param: color - the new color of the font.
//
// @return: void.
//
void gfSetFontColor (g2dColor color);
// -----------------------------------------------------------------------------------------------------
// Reset the current font color.
//
// @param: void.
//
// @return: void.
//
void gfResetFontColor ();
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Set the current font angle.
//
// @param: angle - The new angle of the font.
//
// @return: void.
//
void gfSetFontAngle (double angle);
// -----------------------------------------------------------------------------------------------------
// Reset the current font angle.
//
// @param: void.
//
// @return: void.
//
void gfResetFontAngle ();
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Set the current font depht.
//
// @param: Z - the z value.
//
// @return: void.
//
void gfSetFontDepht (float z);
// -----------------------------------------------------------------------------------------------------
// Reset the current font depht.
//
// @param: void.
//
// @return: void.
//
void gfResetFontDepht ();
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Set the char size of the font face.
//
// @param: font - The font pointer.
// @param: size - The new size of the font.
// @param: points - Set the points size as boolean.
//
// @return: int - the GLIBF error code.
//
int gfSetCharSize (gfFont *font, int size, bool points);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Set the current font size.
//
// @param: size - The new size of the font.
//
// @return: int - the GLIBF error code.
//
int gfSetFontSize (int size, bool points);
// -----------------------------------------------------------------------------------------------------
// Reset the current font size.
//
// @param: void.
//
// @return: int - the GLIBF error code.
//
int gfResetFontSize ();
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Reset the current font state.
//
// @param: void.
//
// @return: void.
//
void gfReset ();
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Frees a font & set its pointer to NULL.
//
// @param: *font - font Pointer to the variable which contains the font pointer.
//
// @return: void.
//
void gfFontFree (gfFont **font);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Load a font from a file.
//
// @param: filename - The font file to load.
// @param: chache - Cache the font file a boolean.
//
// @return: gfFont - The font pointer.
//
gfFont* gfFontLoad (const char *filename, bool chache);
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Render the text.
//
// @param target - The pointer to the render target.
// @param: text - The text the render.
// @param: x - The x position of the text.
// @param: y - The y position of the text.
//
// @return: void.
//
void gfRenderFont (g2dTexture *target, const char *text, int x, int y);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Render the text to the screen.
//
// @param: text - The text the render.
// @param: x - The x position of the text.
// @param: y - The y position of the text.
//
// @return: void.
//
void gfTextToScreen (const char *text, int x, int y);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Render the text to the image.
//
// @param image - The texture pointer.
// @param: text - The text the render.
// @param: x - The x position of the text.
// @param: y - The y position of the text.
//
// @return: void.
//
void gfTextToImage (g2dTexture *image, const char *text, int x, int y);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Get the x,y ,width and height of the text.
//
// @param: text - The text the measure.
// @param: *Glyph - The Glyph pointer.
//
// @return: void.
//
void gfMeasureFont (const char *text, gfGlyph *Glyph);
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Render the text.
//
// @param target - The pointer to the render target.
// @param: c - The char the render.
// @param: x - The x position of the text.
// @param: y - The y position of the text.
// @param: blend - Blend the font with the target texture.
// @param: greyScale - Don't convert the grayScale.
//
// @return: void.
//
void gfRenderChar (g2dTexture *target, char c, int x, int y, bool blend, bool greyScale);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Render the text to the screen.
//
// @param: c - The char the render.
// @param: x - The x position of the text.
// @param: y - The y position of the text.
//
// @return: void.
//
void gfCharToScreen (char c, int x, int y);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Render the text to the image.
//
// @param image - The texture pointer.
// @param: c - The char the render.
// @param: x - The x position of the text.
// @param: y - The y position of the text.
//
// @return: void.
//
void gfCharToImage (g2dTexture *image, char c, int x, int y);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Get the x,y ,width and height of the text.
//
// @param: c - The char the measure.
// @param: *Glyph - The Glyph pointer.
//
// @return: int - the GLIBF error code.
//
int gfMeasureChar (char c, gfGlyph *Glyph);
// -----------------------------------------------------------------------------------------------------



// -----------------------------------------------------------------------------------------------------
// Bmp font library.
// -----------------------------------------------------------------------------------------------------
// Version: 1.00
// Copyright (c) 2012 M4MSOFT
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Create a new BmpFont object.
//
// @param: void.
//
// @return: This function returns NULL on allocation fail.
//
gfBmpFont* gfBmpFontCreate ();
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Frees a bmpFont & set its pointer to NULL.
//
// @param: bmpFont - Pointer to the variable which contains the bmpfont pointer.
//
// @return: void
//   This function is used to gain memory when a bmpfont is useless.
//   Must pass the pointer to the variable which contains the pointer,
//   to set it to NULL.
//
void gfBmpFontFree (gfBmpFont **bFont);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Create a bmpFont from the current font.
//
// @param: charset - The charset of the font.
// @param psm - Pixelformat of the font texture.
// @param vram - Move the font texture to the vram.
//
// @return: This function returns NULL on allocation fail.
//
gfBmpFont* gfBmpFontFromFont (const char *charset, int psm, bool vram);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Load a bmpFont from a trueTypeFont file.
//
// @param: filename - The font file to load.
// @param: size - The size of the font.
// @param: angle - The angle of the font.
// @param: points - Set the points size as boolean.
// @param: charset - The charset of the font.
// @param psm - Pixelformat of the font texture.
// @param vram - Move the font texture to the vram.
//
// @return: This function returns NULL on allocation fail.
//
gfBmpFont* gfBmpFontLoad (const char *filename, int size, double angle, bool points, const char *charset, int psm, bool vram);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Create a bmpFont from a texture.
//
// @param tex - Pointer to a texture,
// @param: width - the width of a character.
// @param: height -  the height of a character.
// @param: charset - The charset of the font.
// @param: upper - Only use upper case character as boolean. (charset must be given)
// @param: link - Link The texture to the bmpFont as boolean.
//
// @return: This function returns NULL on allocation fail.
//
gfBmpFont* gfBmpFontFromTex (g2dTexture *tex, int width, int height, const char *charset, bool upper, bool link);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Render the text to the screen.
//
// @param bFont  - Pointer to the variable which contains the bmpfont pointer.
// @param: text - The text the render.
// @param: x - The x position of the text.
// @param: y - The y position of the text.
// @param: z - The z position of the text.
// @param: color - The color of the text.
// @param: monoSpace - The monoSpace width of the font.
//
// @return: void.
//
void gfRenderBmpFont (gfBmpFont *bFont, int x, int y, int z, const char *text, g2dColor color, int monoSpace);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Get the x,y ,width and height of the text.
//
// @param bFont  - Pointer to the variable which contains the bmpfont pointer.
// @param: text - The text the measure.
// @param: *Glyph - The Glyph pointer.
// @param: monoSpace - The monoSpace width of the font.
//
// @return: void.
//
void gfMeasureBmpFont (gfBmpFont *bFont, const char *text, gfGlyph *Glyph, int monoSpace);
// -----------------------------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif  // GLIBF_H_
