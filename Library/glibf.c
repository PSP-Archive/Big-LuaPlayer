// glibf - A TrueType font library for the PSP ( FileName: glibf.h )
// ------------------------------------------------------------------------
// Version: 1.00
// Copyright (c) 2012 M4MSOFT
//
//	Requirements:	PSP graphics library by Psilocybeing
//				    libpng for PSP
//				    libfreetype2 for PSP
// ------------------------------------------------------------------------

// Include C header files.
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Include psp Header files.
#include <pspkernel.h>
#include <pspctrl.h>
#include <pspdisplay.h>

// ------------------------------------------------------------------------
// Include the freetype.h - header file for FreeType 2 library
//
// FreeType high-level API and common types (specification only).
// Copyright 1996-2001, 2002, 2003, 2004, 2005 by David Turner, Robert Wilhelm, and Werner Lemberg.
#include <ft2build.h>
#include FT_FREETYPE_H
// ------------------------------------------------------------------------

// Include the PSP glib2D and glibf Header files.
#include "glib2d.h"
#include "glibf.h"

// -----------------------------------------------------------------------------------------------------
// Defines.
#define GLIBF_M_PI_Q2       (6.283185307f)
#define GLIBF_HORZ_DPI      (100)

#define DEFAULT_SIZE        (14)
#define DEFAULT_COLOR       (0xffffffff)
#define DEFAULT_ANGLE       (0.0f)
#define DEFAULT_Z           (0.0f)

#define FSTACK_MAX          (32)
#define FSTACK_OFFSET       (2)
#define FRONTSTATE          fstack[fstack_size-1]

#define GLIBF_CHARSET_SIZE   (97)
#define GLIBF_MAX_TEX_WIDTH  (512)
#define GLIBF_MAX_TEX_HEIGHT (512)
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Structures
typedef struct
{
    g2dColor color;
    double angle;
	float z;
	int size;
	bool points;
	gfFont *font;
} FontState;
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Private variables.
static bool init = false;

static FT_Library library = NULL;

static FontState curFntState = {DEFAULT_COLOR, DEFAULT_ANGLE, DEFAULT_Z, DEFAULT_SIZE, true, NULL};
static FontState fstack[FSTACK_MAX];
static unsigned int fstack_size = 0;

static const char GLIBF_CHARSET[GLIBF_CHARSET_SIZE] = {
	' ', '!', '\"','#', '$', '%', '&','\'', '(', ')', '*', '+', ',', '-', '.', '/',
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ':', ';', '<', '=', '>', '?',
	'@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
	'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '[','\\', ']', '^', '_',
	'`', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
	'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '{', '|', '}', '~', '£',
	'\0'
};

static unsigned int GLIBF_DATA_T4[16] =
{
	0x00000000, 0x11ffffff, 0x22ffffff, 0x33ffffff,
	0x44ffffff, 0x55ffffff, 0x66ffffff, 0x77ffffff,
	0x88ffffff, 0x99ffffff, 0xaaffffff, 0xbbffffff,
	0xccffffff, 0xddffffff, 0xeeffffff, 0xffffffff
};

static unsigned int GLIBF_DATA_T8[256] =
{
	0x00000000, 0x01ffffff, 0x02ffffff, 0x03ffffff,  0x04ffffff, 0x05ffffff, 0x06ffffff, 0x07ffffff,  0x08ffffff, 0x09ffffff, 0x0affffff, 0x0bffffff,  0x0cffffff, 0x0dffffff, 0x0effffff, 0x0fffffff,
	0x10ffffff, 0x11ffffff, 0x12ffffff, 0x13ffffff,  0x14ffffff, 0x15ffffff, 0x16ffffff, 0x17ffffff,  0x18ffffff, 0x19ffffff, 0x1affffff, 0x1bffffff,  0x1cffffff, 0x1dffffff, 0x1effffff, 0x1fffffff,
	0x20ffffff, 0x21ffffff, 0x22ffffff, 0x23ffffff,  0x24ffffff, 0x25ffffff, 0x26ffffff, 0x27ffffff,  0x28ffffff, 0x29ffffff, 0x2affffff, 0x0bffffff,  0x2cffffff, 0x2dffffff, 0x2effffff, 0x2fffffff,
	0x30ffffff, 0x31ffffff, 0x32ffffff, 0x33ffffff,  0x34ffffff, 0x35ffffff, 0x36ffffff, 0x37ffffff,  0x38ffffff, 0x39ffffff, 0x3affffff, 0x0bffffff,  0x3cffffff, 0x3dffffff, 0x3effffff, 0x3fffffff,

	0x40ffffff, 0x41ffffff, 0x42ffffff, 0x43ffffff,  0x44ffffff, 0x45ffffff, 0x46ffffff, 0x47ffffff,  0x48ffffff, 0x49ffffff, 0x4affffff, 0x4bffffff,  0x4cffffff, 0x4dffffff, 0x4effffff, 0x4fffffff,
	0x50ffffff, 0x51ffffff, 0x52ffffff, 0x53ffffff,  0x54ffffff, 0x55ffffff, 0x56ffffff, 0x57ffffff,  0x58ffffff, 0x59ffffff, 0x5affffff, 0x5bffffff,  0x5cffffff, 0x5dffffff, 0x5effffff, 0x5fffffff,
	0x60ffffff, 0x61ffffff, 0x62ffffff, 0x63ffffff,  0x64ffffff, 0x65ffffff, 0x66ffffff, 0x67ffffff,  0x68ffffff, 0x69ffffff, 0x6affffff, 0x6bffffff,  0x6cffffff, 0x6dffffff, 0x6effffff, 0x6fffffff,
	0x70ffffff, 0x71ffffff, 0x72ffffff, 0x73ffffff,  0x74ffffff, 0x75ffffff, 0x76ffffff, 0x77ffffff,  0x78ffffff, 0x79ffffff, 0x7affffff, 0x7bffffff,  0x7cffffff, 0x7dffffff, 0x7effffff, 0x7fffffff,

	0x80ffffff, 0x81ffffff, 0x82ffffff, 0x83ffffff,  0x84ffffff, 0x85ffffff, 0x86ffffff, 0x87ffffff,  0x88ffffff, 0x89ffffff, 0x8affffff, 0x8bffffff,  0x8cffffff, 0x8dffffff, 0x8effffff, 0x8fffffff,
	0x90ffffff, 0x91ffffff, 0x92ffffff, 0x93ffffff,  0x94ffffff, 0x95ffffff, 0x96ffffff, 0x97ffffff,  0x98ffffff, 0x99ffffff, 0x9affffff, 0x9bffffff,  0x9cffffff, 0x9dffffff, 0x9effffff, 0x9fffffff,
	0xa0ffffff, 0xa1ffffff, 0xa2ffffff, 0xa3ffffff,  0xa4ffffff, 0xa5ffffff, 0xa6ffffff, 0xa7ffffff,  0xa8ffffff, 0xa9ffffff, 0xaaffffff, 0xabffffff,  0xacffffff, 0xadffffff, 0xaeffffff, 0xafffffff,
	0xb0ffffff, 0xb1ffffff, 0xb2ffffff, 0xb3ffffff,  0xb4ffffff, 0xb5ffffff, 0xb6ffffff, 0xb7ffffff,  0xb8ffffff, 0xb9ffffff, 0xbaffffff, 0xbbffffff,  0xbcffffff, 0xbdffffff, 0xbeffffff, 0xbfffffff,

	0xc0ffffff, 0xc1ffffff, 0xc2ffffff, 0xc3ffffff,  0xc4ffffff, 0xc5ffffff, 0xc6ffffff, 0xc7ffffff,  0xc8ffffff, 0xc9ffffff, 0xcaffffff, 0xcbffffff,  0xccffffff, 0xcdffffff, 0xceffffff, 0xcfffffff,
	0xd0ffffff, 0xd1ffffff, 0xd2ffffff, 0xd3ffffff,  0xd4ffffff, 0xd5ffffff, 0xd6ffffff, 0xd7ffffff,  0xd8ffffff, 0xd9ffffff, 0xdaffffff, 0xdbffffff,  0xdcffffff, 0xddffffff, 0xdeffffff, 0xdfffffff,
	0xe0ffffff, 0xe1ffffff, 0xe2ffffff, 0xe3ffffff,  0xe4ffffff, 0xe5ffffff, 0xe6ffffff, 0xe7ffffff,  0xe8ffffff, 0xe9ffffff, 0xeaffffff, 0xebffffff,  0xecffffff, 0xedffffff, 0xeeffffff, 0xefffffff,
	0xf0ffffff, 0xf1ffffff, 0xf2ffffff, 0xf3ffffff,  0xf4ffffff, 0xf5ffffff, 0xf6ffffff, 0xf7ffffff,  0xf8ffffff, 0xf9ffffff, 0xfaffffff, 0xfbffffff,  0xfcffffff, 0xfdffffff, 0xfeffffff, 0xffffffff
};

static g2dPalette GLIBF_PLT_T4 = {G2D_PALETTE_T4,  16, G2D_MEMORY_STACK, 0, GLIBF_DATA_T4};
static g2dPalette GLIBF_PLT_T8 = {G2D_PALETTE_T8, 256, G2D_MEMORY_STACK, 0, GLIBF_DATA_T8};

// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Define the function prototypes.
static void _gfPush();
gfFont* _gfCreateFont ();
static void _gfDrawBitmap (g2dTexture *target, FT_Bitmap *bitmap, FT_Int x, FT_Int y, int width, int height);
static void _gfDrawTargetBitmap (g2dTexture *target, FT_Bitmap *bitmap, FT_Int x, FT_Int y, int width, int height, bool blend, bool greyScale);
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Initializes the library.
void gfInit ()
{
	if (init)
        return;

	if (FT_Init_FreeType(&library))
		return;

    gfReset();

	init = true;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Shutdowns the library.
void gfTerm ()
{
    if (!init)
        return;

	gfReset();
    FT_Done_FreeType(library);

    init = false;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Saves the current font state to stack.
static void _gfPush()
{
	if (fstack_size >= FSTACK_MAX) return;
    fstack_size++;
    FRONTSTATE.color = curFntState.color;
    FRONTSTATE.angle = curFntState.angle;
    FRONTSTATE.z = curFntState.z;
	FRONTSTATE.size = curFntState.size;
	FRONTSTATE.points = curFntState.points;
	FRONTSTATE.font = curFntState.font;
}
void gfPush ()
{
	if (fstack_size >= FSTACK_MAX - FSTACK_OFFSET) return;
    fstack_size++;
    FRONTSTATE.color = curFntState.color;
    FRONTSTATE.angle = curFntState.angle;
    FRONTSTATE.z = curFntState.z;
	FRONTSTATE.size = curFntState.size;
	FRONTSTATE.points = curFntState.points;
	FRONTSTATE.font = curFntState.font;
}
// -----------------------------------------------------------------------------------------------------
// Restore the current font state from stack.
void gfPop ()
{
    if (fstack_size <= 0) return;
    curFntState.color = FRONTSTATE.color;
    curFntState.angle = FRONTSTATE.angle;
    curFntState.z = FRONTSTATE.z;
	curFntState.font = FRONTSTATE.font;
	gfSetFont(curFntState.font);
	curFntState.size = FRONTSTATE.size;
	curFntState.points = FRONTSTATE.points;
	gfSetFontSize(curFntState.size, curFntState.points);
	fstack_size--;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Clear the font state stack.
void gfClearStack ()
{
	fstack_size = 0;
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Set the current font.
void gfSetFont (gfFont *font)
{
	curFntState.font = font;
	if (font)
	{
		curFntState.size = font->size;
		curFntState.points = font->points;
	}
}
// -----------------------------------------------------------------------------------------------------
// Reset the current font.
void gfResetFont ()
{
	curFntState.font = NULL;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Set the current font color.
void gfSetFontColor (g2dColor color)
{
	curFntState.color = color;
}
// -----------------------------------------------------------------------------------------------------
// Reset the current font color.
void gfResetFontColor ()
{
	curFntState.color = DEFAULT_COLOR;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Set the current font Angle.
void gfSetFontAngle (double angle)
{
	if(angle >= 360.0 || angle <= -360.0)
		angle = fmod(angle, 360.0);
	if(angle < 0.0)
		curFntState.angle = 360.0 + angle;
	else
		curFntState.angle = angle;
}
// -----------------------------------------------------------------------------------------------------
// Reset the current font Angle.
void gfResetFontAngle ()
{
	curFntState.angle = DEFAULT_ANGLE;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Set the current font depht.
void gfSetFontDepht (float z)
{
	curFntState.z = z;
}
// -----------------------------------------------------------------------------------------------------
// Reset the current font depht.
void gfResetFontDepht ()
{
	curFntState.z = DEFAULT_Z;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Set the char size of the font face.
int gfSetCharSize (gfFont *font, int size, bool points)
{
	if (font == NULL)
		return GLIBF_ERR_INVALIDFONT;
	if (font->face == NULL)
		return GLIBF_ERR_INVALIDFONT;
	if (font->size == size && font->points == points)
		return 0;
	if (points)
	{
		if (FT_Set_Char_Size(font->face, size * GLIBF_SIZE_SCALE, 0, GLIBF_HORZ_DPI, 0))
			return GLIBF_ERR_BADSIZE;
	}
	else
	{
		if (FT_Set_Pixel_Sizes(font->face, size, 0))
			return GLIBF_ERR_BADSIZE;
	}
	if (curFntState.font == font)
	{
		curFntState.size = size;
		curFntState.points = points;
	}
	return 0;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Set the current font Size.
int gfSetFontSize (int size, bool points)
{
	return gfSetCharSize(curFntState.font, size, points);
}
// -----------------------------------------------------------------------------------------------------
// Reset the current font Size.
int gfResetFontSize ()
{
	return gfSetCharSize(curFntState.font, DEFAULT_SIZE, true);
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Reset the current font state.
void gfReset ()
{
	gfResetFontSize();
	gfResetFontDepht();
	gfResetFontAngle();
	gfResetFontColor();
    gfResetFont();
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Create a new font object.
static gfFont* _gfFontCreate ()
{
	gfFont *font = (gfFont*) malloc(sizeof(gfFont));
	if (font == NULL) return NULL;

	font->name = NULL;
	font->face = NULL;
	font->data = NULL;

	return font;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Frees a font & set its pointer to NULL.
void gfFontFree (gfFont **font)
{
	// Check the pointer.
	if (font == NULL)
        return;
    if ((*font) == NULL)
        return;

	// Free the face.
	if ((*font)->face)
	{
		FT_Done_Face((*font)->face);
		(*font)->face = NULL;
	}

	// Free the cache data.
	if ((*font)->data)
	{
		free((*font)->data);
		(*font)->data = NULL;
	}

	// Reset the stack pointer.
	if (curFntState.font == *font)
		curFntState.font = NULL;
	int i = 0;
	for (i = 0; i < fstack_size; ++i)
		if (fstack[i].font == *font)
			fstack[i].font = NULL;

	// Free the font structure,
	free((*font));
	(*font) = NULL;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Load a font from a file.
gfFont* gfFontLoad (const char *filename, bool chache)
{
	// Initalize the Free type library.
	if (!init)
        gfInit();

	gfFont *font = NULL;
	FILE *fp = NULL;
	FT_Error error = 0;

	// Create the font object.
	font = _gfFontCreate();
	if (font == NULL) goto label_error;

	if (chache)
	{
		// cache font for faster access. This might be a bad idea for big fonts.
		fp = fopen(filename, "rb");
		if (fp == NULL) goto label_error;
		fseek(fp, 0, SEEK_END);
		int filesize = ftell(fp);
		font->data = (unsigned char *) malloc(filesize);
		if (font->data == NULL) goto label_error;
		rewind(fp);
		fread(font->data, filesize, 1, fp);
		fclose(fp);

		// Create the memory face.
		error = FT_New_Memory_Face(library, font->data, filesize, 0, &font->face);
		if (error) goto label_error;
	}
	else
	{
		// Create the new file face.
		error = FT_New_Face(library, filename, 0, &font->face );
		if (error) goto label_error;
	}

	// set default font size.
	error = gfSetCharSize(font, DEFAULT_SIZE, true);
	if (error) goto label_error;

	// Get the ASCII Postscript name of the font,
	font->name = FT_Get_Postscript_Name(font->face);

	// Return the font object.
	return font;

	// Load failure... abort
label_error:
	// Close the File.
	if (fp != NULL)
        fclose(fp);

	// Free the Font.
	gfFontFree(&font);

	return NULL;
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Render the text to the screen.
void gfTextToScreen (const char *text, int x, int y)
{
	g2dBeginPoints();
	gfRenderFont(G2D_VOID, text, x, y);
	g2dEnd();
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Render the text to the image.
void gfTextToImage (g2dTexture *image, const char *text, int x, int y)
{
	gfRenderFont(image, text, x, y);
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Render the text.
void gfRenderFont (g2dTexture *target, const char *text, int x, int y)
{
	if (curFntState.font == NULL)
		return;
	FT_Face face = curFntState.font->face;
	if (face == NULL)
		return;

	FT_GlyphSlot slot;
	FT_Matrix matrix;
	FT_Vector pen;
	FT_Error error;
	double angle;
	int target_height = G2D_SCR_H;
	int n, num_chars;

	if(!text || !*text)
		return;
	if(target)
		target_height = target->sh;

	slot = face->glyph;
	num_chars = strlen(text);
	angle = (curFntState.angle / 360.0) * GLIBF_M_PI_Q2;
	matrix.xx = (FT_Fixed)(cos(angle) * 0x10000L);
	matrix.xy = (FT_Fixed)(-sin(angle) * 0x10000L);
	matrix.yx = (FT_Fixed)(sin(angle) * 0x10000L);
	matrix.yy = (FT_Fixed)(cos(angle) * 0x10000L);

	pen.x = x * GLIBF_SIZE_SCALE;
	pen.y = (target_height - y) * GLIBF_SIZE_SCALE;
	for(n = 0; n < num_chars; n++) {
		FT_Set_Transform(face, &matrix, &pen);
		error = FT_Load_Char(face, *(text + n), FT_LOAD_RENDER);
		if(error)
			continue;                 /* ignore errors */
		if(target)
			_gfDrawBitmap(target, &slot->bitmap, slot->bitmap_left,
				target_height - slot->bitmap_top,
				target->w, target->h);
		else // to screen
			_gfDrawBitmap(G2D_VOID, &slot->bitmap, slot->bitmap_left,
				target_height - slot->bitmap_top,
		        G2D_SCR_W, G2D_SCR_H);
		pen.x += slot->advance.x;
		pen.y += slot->advance.y;
	}
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static void _gfDrawBitmap (g2dTexture *target, FT_Bitmap *bitmap, FT_Int x, FT_Int y, int width, int height)
{
	FT_Int i, j, p, q;
	FT_Int x_max = x + bitmap->width;
	FT_Int y_max = y + bitmap->rows;
	g2dColor pixel, grey;
	int z = curFntState.z;
	g2dColor color = curFntState.color;
	g2dColor alpha = G2D_GET_A(color)+1;
	if (alpha == 0) return;
	color = color & 0x00ffffff;

	for(i = x, p = 0; i < x_max; i++, p++) {
		for(j = y, q = 0; j < y_max; j++, q++) {
			if(i >= width || i < 0 || j >= height || j < 0)
				continue;
			grey = (bitmap->buffer[q * bitmap->width + p] * alpha) >> 8;
			if (grey) {
				pixel = (grey << 24) | color;
				if (target)
					G2D_TEX_SETPIXEL(target, i, j, pixel, true, target->plt)
				else
					{g2dSetCoordXYZ(i, j, z); g2dSetColor(pixel); g2dAdd();}
			}
		}
	}
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Get the x,y ,width and height of the text.
void gfMeasureFont (const char *text, gfGlyph *Glyph)
{
	Glyph->size.x = 0; Glyph->size.y = 0;
	Glyph->size.width = 0; Glyph->size.height = 0;
	Glyph->offset.x = 0; Glyph->offset.y = 0;
	Glyph->advance.x = 0; Glyph->advance.y = 0;
	Glyph->error = 0;

	if (curFntState.font == NULL)
		return;
	FT_Face face = curFntState.font->face;
	if (face == NULL)
		return;

	FT_GlyphSlot slot;
	FT_Matrix matrix;
	FT_Vector pen;
	FT_Error error;
	double angle;
	int n, num_chars;

	if(!text || !*text)
		return;

	slot = face->glyph;
	num_chars = strlen(text);
	angle = (curFntState.angle / 360.0) * GLIBF_M_PI_Q2;
	matrix.xx = (FT_Fixed)(cos(angle) * 0x10000L);
	matrix.xy = (FT_Fixed)(-sin(angle) * 0x10000L);
	matrix.yx = (FT_Fixed)(sin(angle) * 0x10000L);
	matrix.yy = (FT_Fixed)(cos(angle) * 0x10000L);

	pen.x = 0 * GLIBF_SIZE_SCALE;
	pen.y = 0 * GLIBF_SIZE_SCALE;
	for(n = 0; n < num_chars; n++) {
		FT_Set_Transform(face, &matrix, &pen);
		error = FT_Load_Char(face, *(text + n), FT_LOAD_RENDER);
		// ignore errors
		if(error) {Glyph->error = error; continue;}
		// Update values.
		if (slot->bitmap.width && slot->bitmap.rows)
		{
			// Get the start x and y offset.
			if (n == 0)
			{
				// Get the x0, y0, x1 and y1 positions.
				Glyph->offset.x = slot->bitmap_left;
				Glyph->offset.y = slot->bitmap_top;
				Glyph->size.width = slot->bitmap_left + slot->bitmap.width;
				Glyph->size.height = slot->bitmap.rows - slot->bitmap_top;
			}
			else
			{
				// Update the x0, y0 positions.
				if (Glyph->offset.x > slot->bitmap_left)
					Glyph->offset.x = slot->bitmap_left;
				if (Glyph->offset.y < slot->bitmap_top)
					Glyph->offset.y = slot->bitmap_top;
				// Update the x1, y1 positions.
				if (Glyph->size.width < slot->bitmap_left + slot->bitmap.width)
					Glyph->size.width = slot->bitmap_left + slot->bitmap.width;
				if (Glyph->size.height < slot->bitmap.rows - slot->bitmap_top)
					Glyph->size.height = slot->bitmap.rows - slot->bitmap_top;
			}
		}
		pen.x += slot->advance.x;
		pen.y += slot->advance.y;
	}
	// Update values.
	Glyph->offset.y = -Glyph->offset.y;
	Glyph->size.width = G2D_VALUE_DELTA(Glyph->size.width, Glyph->offset.x);
	Glyph->size.height = G2D_VALUE_DELTA(Glyph->size.height, Glyph->offset.y);
	Glyph->advance.x = pen.x >> GLIBF_SIZE_SHIFT;
	Glyph->advance.y = -(pen.y >> GLIBF_SIZE_SHIFT);
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Render the char to the screen.
void gfCharToScreen (char c, int x, int y)
{
	g2dBeginPoints();
	gfRenderChar(G2D_VOID, c, x, y, false, false);
	g2dEnd();
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Render the char to the image.
void gfCharToImage (g2dTexture *image, char c, int x, int y)
{
	gfRenderChar(image, c, x, y, true, false);
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Render the char.
void gfRenderChar (g2dTexture *target, char c, int x, int y, bool blend, bool greyScale)
{
	if (curFntState.font == NULL) return;
	FT_Face face = curFntState.font->face;
	if (face == NULL) return;

	FT_GlyphSlot slot;
	FT_Matrix matrix;
	FT_Vector pen;
	FT_Error error;
	double angle;
	int target_height = G2D_SCR_H;
	if(target) target_height = target->sh;

	slot = face->glyph;
	angle = (curFntState.angle / 360.0) * GLIBF_M_PI_Q2;
	matrix.xx = (FT_Fixed)(cos(angle) * 0x10000L);
	matrix.xy = (FT_Fixed)(-sin(angle) * 0x10000L);
	matrix.yx = (FT_Fixed)(sin(angle) * 0x10000L);
	matrix.yy = (FT_Fixed)(cos(angle) * 0x10000L);

	pen.x = x * GLIBF_SIZE_SCALE;
	pen.y = (target_height - y) * GLIBF_SIZE_SCALE;
	FT_Set_Transform(face, &matrix, &pen);
	error = FT_Load_Char(face, c, FT_LOAD_RENDER);
	if(error)
		return;                 /* ignore errors */
	if(target)
		_gfDrawTargetBitmap (target, &slot->bitmap, slot->bitmap_left,
			target_height - slot->bitmap_top,
			target->w, target->h, blend, greyScale);
	else // to screen
		_gfDrawBitmap(G2D_VOID, &slot->bitmap, slot->bitmap_left,
			target_height - slot->bitmap_top,
			G2D_SCR_W, G2D_SCR_H);
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static void _gfDrawTargetBitmap (g2dTexture *target, FT_Bitmap *bitmap, FT_Int x, FT_Int y, int width, int height, bool blend, bool greyScale)
{
	FT_Int i, j, p, q;
	FT_Int x_max = x + bitmap->width;
	FT_Int y_max = y + bitmap->rows;
	g2dPalette *plt = NULL;
	g2dColor pixel, grey;
	g2dColor color = curFntState.color;
	g2dColor alpha = G2D_GET_A(color)+1;
	if (alpha == 0) return;
	color = color & 0x00ffffff;

	if (target->plt && !greyScale) plt = target->plt;
	for(i = x, p = 0; i < x_max; i++, p++) {
		for(j = y, q = 0; j < y_max; j++, q++) {
			if(i >= width || i < 0 || j >= height || j < 0)
				continue;
			grey = (bitmap->buffer[q * bitmap->width + p] * alpha) >> 8;
			if (grey) {
				if (!greyScale)
					pixel = ((grey) << 24) | color;
				else switch (target->psm)
				{
					case G2D_PSM_T8:
						pixel = ((grey & 0x03) << 6) | ((grey & 0x0c) << 12) | ((grey & 0x30) << 18) | ((grey & 0xc0) << 24); break;
					case G2D_PSM_T4:
						grey >>= 4; pixel = ((grey & 0x1) << 7) | ((grey & 0x2) << 14) | ((grey & 0x4) << 21) | ((grey & 0x8) << 28); break;
					default: pixel = ((grey) << 24) | color; break;
				}
				G2D_TEX_SETPIXEL(target, i, j, pixel, blend, plt)
			}
		}
	}
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Get the x,y ,width and height of the the char.
int gfMeasureChar (char c, gfGlyph *Glyph)
{
	Glyph->size.x = 0; Glyph->size.y = 0;
	Glyph->size.width = 0; Glyph->size.height = 0;
	Glyph->offset.x = 0; Glyph->offset.y = 0;
	Glyph->advance.x = 0; Glyph->advance.y = 0;
	Glyph->error = 0;

	if (curFntState.font == NULL)
		return GLIBF_ERR_INVALIDFONT;
	FT_Face face = curFntState.font->face;
	if (face == NULL)
		return GLIBF_ERR_INVALIDFONT;

	FT_GlyphSlot slot;
	FT_Matrix matrix;
	FT_Vector pen;
	FT_Error error;
	double angle;

	slot = face->glyph;
	angle = (curFntState.angle / 360.0) * GLIBF_M_PI_Q2;
	matrix.xx = (FT_Fixed)(cos(angle) * 0x10000L);
	matrix.xy = (FT_Fixed)(-sin(angle) * 0x10000L);
	matrix.yx = (FT_Fixed)(sin(angle) * 0x10000L);
	matrix.yy = (FT_Fixed)(cos(angle) * 0x10000L);

	pen.x = 0 * GLIBF_SIZE_SCALE;
	pen.y = 0 * GLIBF_SIZE_SCALE;
	FT_Set_Transform(face, &matrix, &pen);
	error = FT_Load_Char(face, c, FT_LOAD_RENDER);
	if(!error)
	{
		// Update values.
		if (slot->bitmap.width && slot->bitmap.rows)
		{
			// Get the x0, y0, x1 and y1 positions.
			Glyph->offset.x = slot->bitmap_left;
			Glyph->offset.y = slot->bitmap_top;
			Glyph->size.width = slot->bitmap_left + slot->bitmap.width;
			Glyph->size.height = slot->bitmap.rows - slot->bitmap_top;
		}
		pen.x += slot->advance.x;
		pen.y += slot->advance.y;
	}
	else {Glyph->error = error; return GLIBF_ERR_INVALIDFONT;};
	// Update values.
	Glyph->offset.y = -Glyph->offset.y;
	Glyph->size.width = G2D_VALUE_DELTA(Glyph->size.width, Glyph->offset.x);
	Glyph->size.height = G2D_VALUE_DELTA(Glyph->size.height, Glyph->offset.y);
	Glyph->advance.x = pen.x >> GLIBF_SIZE_SHIFT;
	Glyph->advance.y = -(pen.y >> GLIBF_SIZE_SHIFT);
	return 0;
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Bmp font library.
// -----------------------------------------------------------------------------------------------------
// Version: 1.00
// Copyright (c) 2012 M4MSOFT
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Create a new BmpFont object.
gfBmpFont* gfBmpFontCreate ()
{
	gfBmpFont *bFont = (gfBmpFont*) malloc(sizeof(gfBmpFont));
	if (bFont == NULL) return NULL;

	bFont->num_charmaps = 1;
	bFont->charmaps[0] = 0;
	bFont->glyph[0].size.x = 0;
	bFont->glyph[0].size.y = 0;
	bFont->glyph[0].size.width = 0;
	bFont->glyph[0].size.height = 0;
	bFont->glyph[0].offset.x = 0;
	bFont->glyph[0].offset.y = 0;
	bFont->glyph[0].advance.x = 0;
	bFont->glyph[0].advance.y = 0;
	bFont->glyph[0].error = 0;
	bFont->height = 0;
	bFont->offset.x = 0;
	bFont->offset.y = 0;
	bFont->tex = NULL;

	int n;
	for (n = 0; n < 256 ; n++)
		bFont->charmaps[n] = 0;

	return bFont;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Frees a bmpFont & set its pointer to NULL.
void gfBmpFontFree (gfBmpFont **bFont)
{
	// Check the pointer.
	if (bFont == NULL)
        return;
    if ((*bFont) == NULL)
        return;

	// Free the bmp font texture.
	g2dTexFree(&((*bFont)->tex));

	// Free the font structure,
	free((*bFont));
	(*bFont) = NULL;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Create a bmpFont from the current font.
gfBmpFont* gfBmpFontFromFont (const char *charset, int psm, bool vram)
{
	if (curFntState.font == NULL) return NULL;
	if (curFntState.font->face == NULL) return NULL;

	// Select the charmap.
	if (!charset || !*charset)
		charset = GLIBF_CHARSET;

	// Create the font structure.
	gfBmpFont * bFont = gfBmpFontCreate();
	if (bFont == NULL)
		return NULL;

	int n, num_chars;
	int char_count = bFont->num_charmaps;
	int Tex_width = 0;
	int Tex_height = 0;
	int Tex_LineHeight = 0;
	unsigned char uChar = 0;
	int x = 0; int y = 0;
	int w = 0; int h = 0;
	bool greyScale = false;

	// Collect the character in the charset.
	num_chars = strlen(charset);
	for (n = 0; n < num_chars ; n++)
	{
		uChar = (unsigned char) *(charset + n);
		// Check if the character exist.
		if (uChar && bFont->charmaps[uChar] == 0)
		{
			// Add the char to the character list.
			if (!gfMeasureChar((unsigned char) *(charset + n), &(bFont->glyph[char_count])))
			{
				w = bFont->glyph[char_count].size.width;
				h = bFont->glyph[char_count].size.height;
				if ((x + w) > GLIBF_MAX_TEX_WIDTH)
				{
					if ((Tex_height + h) > GLIBF_MAX_TEX_HEIGHT)
						goto label_error;
					Tex_height += Tex_LineHeight;
					x = 0;
					y = Tex_height;
					Tex_LineHeight = 0;
				}
				bFont->glyph[char_count].size.x = x;
				bFont->glyph[char_count].size.y = y;
				x += w;
				if (Tex_width < x) Tex_width = x;
				if (Tex_LineHeight < h)
				{
					Tex_LineHeight = h;
					if ((Tex_height + Tex_LineHeight) > GLIBF_MAX_TEX_HEIGHT)
						goto label_error;
					if (bFont->height < Tex_LineHeight) bFont->height = Tex_LineHeight;
				}
				if (n == 0 || ((bFont->offset.y > bFont->glyph[char_count].offset.y) && w && h))
					bFont->offset.y = bFont->glyph[char_count].offset.y;
				bFont->charmaps[uChar] = char_count++;
			}
		}
	}
	bFont->num_charmaps = char_count;
	Tex_height += Tex_LineHeight;
	bFont->offset.y = -bFont->offset.y;

	// Set the bmpFont info.
	bFont->height += 1;
	//bFont->offset.y = 0;

	// Create the texture.
	bFont->tex = g2dTexCreate(Tex_width, Tex_height, psm, false);
	if (bFont->tex == NULL)
		goto label_error;

	// Set the palette for the font texture.
	if (psm == G2D_PSM_T8)
		{(bFont->tex)->plt = &GLIBF_PLT_T8; greyScale = true;}
	else if (psm == G2D_PSM_T4)
		{(bFont->tex)->plt = &GLIBF_PLT_T4; greyScale = true;}

	// Render the font.
	_gfPush();
	gfSetFontColor(0xffffffff);
	for (n = 0; n < num_chars ; n++)
	{
		uChar = (unsigned char) *(charset + n);
		char_count = bFont->charmaps[uChar];
		// Check if the character exist.
		if (uChar && char_count)
			gfRenderChar(bFont->tex,
						 *(charset + n),
			             bFont->glyph[char_count].size.x - bFont->glyph[char_count].offset.x,
						 bFont->glyph[char_count].size.y - bFont->glyph[char_count].offset.y,
		                 false, greyScale);
	}
	gfPop();

	// Swizzle the Font.
	g2dTexSwizzle(bFont->tex);

	// Move the Texture to the vram.
	if (vram) g2dTexToVram(bFont->tex);

	return bFont;

	// Build failure... abort
label_error:

	// Free the bmpFont.
	gfBmpFontFree(&bFont);

	return NULL;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Load a bmpFont from a trueTypeFont file.
gfBmpFont* gfBmpFontLoad (const char *filename, int size, double angle, bool points, const char *charset, int psm, bool vram)
{
	// Store the current font state.
	_gfPush();

	gfFont* font = NULL;
	gfBmpFont* bFont = NULL;

	 // Load the font file.
	font = gfFontLoad (filename, false);
	if (font == NULL)
		goto label_error;

	// Set the current font state.
	gfReset();
	gfSetFont(font);
	gfSetFontAngle(angle);
	if (gfSetFontSize(size, points))
		goto label_error;

	// Create the bmpFont from the current font.
	bFont = gfBmpFontFromFont (charset, psm, vram);
	if (bFont == NULL)
		goto label_error;

	// Restore the font state.
	gfPop();
	gfFontFree(&font);

	return bFont;

// Load failure... abort
label_error:

	gfBmpFontFree(&bFont);
	gfFontFree(&font);

	gfPop();
	return NULL;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Create a bmpFont from a texture.
gfBmpFont* gfBmpFontFromTex (g2dTexture *tex, int width, int height, const char *charset, bool upper, bool link)
{
	if (tex == NULL)
		return NULL;

	// Create the bmpFont from the current font.
	gfBmpFont *bFont = gfBmpFontCreate();
	if (bFont == NULL)
		return NULL;

	int x = 0; int y = 0;
	int char_count = bFont->num_charmaps;
	unsigned char uChar = 0;

	// Link the texture to the bmpFont structure.
	if (link)
		if (g2dTexAddLink(tex))
			{gfBmpFontFree(&bFont); return NULL;}
	bFont->tex = tex;

	// Build the glyph array.
	if (!charset || !*charset)
	{
		int n;
		for (n = 1, x = width; n < 256; n++)
		{
			uChar = n;
			bFont->glyph[char_count].size.x = x;
			bFont->glyph[char_count].size.y = y;
			bFont->glyph[char_count].size.width = width;
			bFont->glyph[char_count].size.height = height;
			bFont->glyph[char_count].offset.x = 0;
			bFont->glyph[char_count].offset.y = 0;
			bFont->glyph[char_count].advance.x = width;
			bFont->glyph[char_count].advance.y = 0;
			bFont->glyph[char_count].error = 0;
			x += width;
			bFont->charmaps[uChar] = char_count++;
			if ((n & 0xf) == 0xf)
				{x = 0; y += height;}
		}
		bFont->num_charmaps = char_count;
		bFont->height = height;
	}
	else
	{
		// Collect the character in the charset.
		int n, num_chars;
		num_chars = strlen(charset);
		for (n = 0; n < num_chars ; n++)
		{
			uChar = (unsigned char) *(charset + n);
			// Add the char to the character list if the character dos't exist.
			if (uChar && bFont->charmaps[uChar] == 0)
			{
				bFont->glyph[char_count].size.x = x;
				bFont->glyph[char_count].size.y = 0;
				bFont->glyph[char_count].size.width = width;
				bFont->glyph[char_count].size.height = height;
				bFont->glyph[char_count].offset.x = 0;
				bFont->glyph[char_count].offset.y = 0;
				bFont->glyph[char_count].advance.x = width;
				bFont->glyph[char_count].advance.y = 0;
				bFont->glyph[char_count].error = 0;
				x += width;
				if (upper){
					bFont->charmaps[tolower(uChar)] = char_count;
					bFont->charmaps[toupper(uChar)] = char_count++;
				}else
					bFont->charmaps[uChar] = char_count++;
			}
		}
		bFont->num_charmaps = char_count;
		bFont->height = height;
	}

	return bFont;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
void gfRenderBmpFont (gfBmpFont *bFont, int x, int y, int z, const char *text, g2dColor color, int monoSpace)
{
	if (bFont == NULL || !text || !*text)
		return;

	int n, num_chars;
	unsigned char *Utext = (unsigned char*) text;
	unsigned char *charmaps = bFont->charmaps;
	gfGlyph *glyph = 0;
	unsigned char glyph_n = 0;
	int x0 = x + bFont->offset.x;
	int y0 = y + bFont->offset.y;

	num_chars = strlen(text);
	g2dBeginRects(bFont->tex);
	g2dSetColor(color);
	for (n = 0; n < num_chars; n++)
	{
		glyph_n = charmaps[Utext[n]];
		if (!glyph_n) continue;
		glyph = bFont->glyph + glyph_n;
		if (glyph->size.width && glyph->size.height){
			g2dSetCoordXYZ(x0 + glyph->offset.x, y0 + glyph->offset.y, z);
			g2dSetScaleWH(glyph->size.width, glyph->size.height);
			g2dSetCropXY(glyph->size.x, glyph->size.y);
			g2dSetCropWH(glyph->size.width, glyph->size.height);
			g2dAdd();
		}
		x0 += (monoSpace ? monoSpace : glyph->advance.x);
		y0 += glyph->advance.y;
	}
	g2dEnd();
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Get the x,y ,width and height of the text.
void gfMeasureBmpFont (gfBmpFont *bFont, const char *text, gfGlyph *Glyph, int monoSpace)
{
	Glyph->size.x = 0; Glyph->size.y = 0;
	Glyph->size.width = 0; Glyph->size.height = 0;
	Glyph->offset.x = 0; Glyph->offset.y = 0;
	Glyph->advance.x = 0; Glyph->advance.y = 0;
	Glyph->error = 0;

	if (bFont == NULL || !text || !*text)
		return;

	int n, num_chars;
	unsigned char *Utext = (unsigned char*) text;
	unsigned char *charmaps = bFont->charmaps;
	gfGlyph *slot = 0;
	unsigned char glyph_n = 0;
	int x0 = bFont->offset.x;
	int y0 = bFont->offset.y;

	num_chars = strlen(text);
	for (n = 0; n < num_chars; n++)
	{
		glyph_n = charmaps[Utext[n]];
		// ignore errors
		if (!glyph_n) continue;
		slot = bFont->glyph + glyph_n;
		// Update values.
		if (slot->size.width && slot->size.height)
		{
			// Get the start x and y offset.
			if (n == 0)
			{
				// Get the x0, y0, x1 and y1 positions.
				Glyph->size.x = slot->size.x;
				Glyph->size.y = slot->size.y;
				Glyph->offset.x = x0 + slot->offset.x;
				Glyph->offset.y = y0 + slot->offset.y;
				Glyph->size.width = x0 + slot->offset.x + slot->size.width;
				Glyph->size.height = y0 + slot->offset.y + slot->size.height;
			}
			else
			{
				// Update the x0, y0 positions.
				if (Glyph->offset.x > x0 + slot->offset.x)
					Glyph->offset.x = x0 + slot->offset.x;
				if (Glyph->offset.y > y0 + slot->offset.y)
					Glyph->offset.y = y0 + slot->offset.y;
				// Update the x1, y1 positions.
				if (Glyph->size.width < x0 + slot->offset.x + slot->size.width)
					Glyph->size.width = x0 + slot->offset.x + slot->size.width;
				if (Glyph->size.height < y0 + slot->offset.y + slot->size.height)
					Glyph->size.height = y0 + slot->offset.y + slot->size.height;
			}
		}
		x0 += (monoSpace ? monoSpace : slot->advance.x);
		y0 += slot->advance.y;
	}
	// Update values.
	Glyph->size.width = G2D_VALUE_DELTA(Glyph->size.width, Glyph->offset.x);
	Glyph->size.height = G2D_VALUE_DELTA(Glyph->size.height, Glyph->offset.y);
	Glyph->advance.x = x0 - bFont->offset.x;
	Glyph->advance.y = y0 - bFont->offset.y;
}
// -----------------------------------------------------------------------------------------------------
