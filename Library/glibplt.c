// glibplt - Palette support library for glib2d. ( FileName: glibplt.c )
// ------------------------------------------------------------------------
// Version: 1.00
// Copyright (c) 2012 M4MSOFT
// ------------------------------------------------------------------------

#include "glib2d.h"

// Include C header files.
//#include <ctype.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


// Include psp Header files.
//#include <pspkernel.h>

// -----------------------------------------------------------------------------------------------------
// Global variables
unsigned int g2d_palette_T4[G2D_BLOKS_PSM_T4*8] =
{
	0x80000000, 0x800000ff, 0x8000ff00, 0x8000ffff,   // black2 red2 green2 Yellow2
	0x80ff0000, 0x80ff00ff, 0x80ffff00, 0x80ffffff,   // blue2 magenta2 cycan2 white2
	0xff000000, 0xff0000ff, 0xff00ff00, 0xff00ffff,   // black red green Yellow
	0xffff0000, 0xffff00ff, 0xffffff00, 0xffffffff    // blue magenta cycan white
};

unsigned int g2d_palette_T8[G2D_BLOKS_PSM_T8*8] =
{
	0x00000000, 0x0000000f, 0x000000f0, 0x000000ff,  0x00000f00, 0x00000f0f, 0x00000ff0, 0x00000fff,  0x0000f000, 0x0000f00f, 0x0000f0f0, 0x0000f0ff,  0x0000ff00, 0x0000ff0f, 0x0000fff0, 0x0000ffff,
	0x000f0000, 0x000f000f, 0x000f00f0, 0x000f00ff,  0x000f0f00, 0x000f0f0f, 0x000f0ff0, 0x000f0fff,  0x000ff000, 0x000ff00f, 0x000ff0f0, 0x000ff0ff,  0x000fff00, 0x000fff0f, 0x000ffff0, 0x000fffff,
	0x00f00000, 0x00f0000f, 0x00f000f0, 0x00f000ff,  0x00f00f00, 0x00f00f0f, 0x00f00ff0, 0x00f00fff,  0x00f0f000, 0x00f0f00f, 0x00f0f0f0, 0x00f0f0ff,  0x00f0ff00, 0x00f0ff0f, 0x00f0fff0, 0x00f0ffff,
	0x00ff0000, 0x00ff000f, 0x00ff00f0, 0x00ff00ff,  0x00ff0f00, 0x00ff0f0f, 0x00ff0ff0, 0x00ff0fff,  0x00fff000, 0x00fff00f, 0x00fff0f0, 0x00fff0ff,  0x00ffff00, 0x00ffff0f, 0x00fffff0, 0x00ffffff,

	0x0f000000, 0x0f00000f, 0x0f0000f0, 0x0f0000ff,  0x0f000f00, 0x0f000f0f, 0x0f000ff0, 0x0f000fff,  0x0f00f000, 0x0f00f00f, 0x0f00f0f0, 0x0f00f0ff,  0x0f00ff00, 0x0f00ff0f, 0x0f00fff0, 0x0f00ffff,
	0x0f0f0000, 0x0f0f000f, 0x0f0f00f0, 0x0f0f00ff,  0x0f0f0f00, 0x0f0f0f0f, 0x0f0f0ff0, 0x0f0f0fff,  0x0f0ff000, 0x0f0ff00f, 0x0f0ff0f0, 0x0f0ff0ff,  0x0f0fff00, 0x0f0fff0f, 0x0f0ffff0, 0x0f0fffff,
	0x0ff00000, 0x0ff0000f, 0x0ff000f0, 0x0ff000ff,  0x0ff00f00, 0x0ff00f0f, 0x0ff00ff0, 0x0ff00fff,  0x0ff0f000, 0x0ff0f00f, 0x0ff0f0f0, 0x0ff0f0ff,  0x0ff0ff00, 0x0ff0ff0f, 0x0ff0fff0, 0x0ff0ffff,
	0x0fff0000, 0x0fff000f, 0x0fff00f0, 0x0fff00ff,  0x0fff0f00, 0x0fff0f0f, 0x0fff0ff0, 0x0fff0fff,  0x0ffff000, 0x0ffff00f, 0x0ffff0f0, 0x0ffff0ff,  0x0fffff00, 0x0fffff0f, 0x0ffffff0, 0x0fffffff,

	0xf0000000, 0xf000000f, 0xf00000f0, 0xf00000ff,  0xf0000f00, 0xf0000f0f, 0xf0000ff0, 0xf0000fff,  0xf000f000, 0xf000f00f, 0xf000f0f0, 0xf000f0ff,  0xf000ff00, 0xf000ff0f, 0xf000fff0, 0xf000ffff,
	0xf00f0000, 0xf00f000f, 0xf00f00f0, 0xf00f00ff,  0xf00f0f00, 0xf00f0f0f, 0xf00f0ff0, 0xf00f0fff,  0xf00ff000, 0xf00ff00f, 0xf00ff0f0, 0xf00ff0ff,  0xf00fff00, 0xf00fff0f, 0xf00ffff0, 0xf00fffff,
	0xf0f00000, 0xf0f0000f, 0xf0f000f0, 0xf0f000ff,  0xf0f00f00, 0xf0f00f0f, 0xf0f00ff0, 0xf0f00fff,  0xf0f0f000, 0xf0f0f00f, 0xf0f0f0f0, 0xf0f0f0ff,  0xf0f0ff00, 0xf0f0ff0f, 0xf0f0fff0, 0xf0f0ffff,
	0xf0ff0000, 0xf0ff000f, 0xf0ff00f0, 0xf0ff00ff,  0xf0ff0f00, 0xf0ff0f0f, 0xf0ff0ff0, 0xf0ff0fff,  0xf0fff000, 0xf0fff00f, 0xf0fff0f0, 0xf0fff0ff,  0xf0ffff00, 0xf0ffff0f, 0xf0fffff0, 0xf0ffffff,

	0xff000000, 0xff00000f, 0xff0000f0, 0xff0000ff,  0xff000f00, 0xff000f0f, 0xff000ff0, 0xff000fff,  0xff00f000, 0xff00f00f, 0xff00f0f0, 0xff00f0ff,  0xff00ff00, 0xff00ff0f, 0xff00fff0, 0xff00ffff,
	0xff0f0000, 0xff0f000f, 0xff0f00f0, 0xff0f00ff,  0xff0f0f00, 0xff0f0f0f, 0xff0f0ff0, 0xff0f0fff,  0xff0ff000, 0xff0ff00f, 0xff0ff0f0, 0xff0ff0ff,  0xff0fff00, 0xff0fff0f, 0xff0ffff0, 0xff0fffff,
	0xfff00000, 0xfff0000f, 0xfff000f0, 0xfff000ff,  0xfff00f00, 0xfff00f0f, 0xfff00ff0, 0xfff00fff,  0xfff0f000, 0xfff0f00f, 0xfff0f0f0, 0xfff0f0ff,  0xfff0ff00, 0xfff0ff0f, 0xfff0fff0, 0xfff0ffff,
	0xffff0000, 0xffff000f, 0xffff00f0, 0xffff00ff,  0xffff0f00, 0xffff0f0f, 0xffff0ff0, 0xffff0fff,  0xfffff000, 0xfffff00f, 0xfffff0f0, 0xfffff0ff,  0xffffff00, 0xffffff0f, 0xfffffff0, 0xffffffff
};

g2dPalette g2d_plt_T8 = {G2D_PALETTE_T8, G2D_BLOKS_PSM_T8*8, G2D_MEMORY_STACK, 0, g2d_palette_T8};
g2dPalette g2d_plt_T4 = {G2D_PALETTE_T4, G2D_BLOKS_PSM_T4*8, G2D_MEMORY_STACK, 0, g2d_palette_T4};

// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Define the function prototypes.
static g2dPalette* _g2dPaletteLoad (FILE *fp);
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Creates a new blank palette.
g2dPalette* g2dPaletteCreate (g2dPalette_Type type, unsigned int count)
{
	// Set the palette size.
	switch (type){
		case G2D_PALETTE_T8: count = 256; break;
		case G2D_PALETTE_T4: count = 16; break;
		default: type = G2D_PALETTE_NONE; break;}

	g2dPalette *plt = malloc(sizeof(g2dPalette));
	if (plt == NULL)
        return NULL;

	plt->type = type;
	plt->count = count;
	plt->memType = G2D_MEMORY_DEFAULT;
	plt->memCount = 0;
	plt->data = malloc(count * sizeof(g2dColor));
	memset(plt->data, 0, count * sizeof(g2dColor));

    return plt;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Frees a palette & set its pointer to NULL.
void g2dPaletteFree (g2dPalette **plt)
{
	if (plt == NULL)
        return;
    if (*plt == NULL)
        return;

	// Break the link.
	if ((*plt)->memType == G2D_MEMORY_LINK)
	{
		if (((*plt)->memCount)-- <= 0)
			(*plt)->memType = G2D_MEMORY_DEFAULT;
	}

	switch ((*plt)->memType)
	{
		case G2D_MEMORY_DEFAULT: // Free the palette data.
			free((*plt)->data);
		case G2D_MEMORY_STRUCT: // Free the palette structure.
			free((*plt));
		case G2D_MEMORY_POINTER: // Set the palette pointer to NULL.
			*plt = NULL;
		case G2D_MEMORY_STACK: // Don't free the palette.
			break;
		case G2D_MEMORY_LINK: // Set the palette pointer to NULL.
		default:
			*plt = NULL;
	}
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Add a link to the palette.
int g2dpaletteAddLink(g2dPalette *plt)
{
    if (plt == NULL)
        return -1;

	if (plt->memType != G2D_MEMORY_LINK && plt->memType != G2D_MEMORY_DEFAULT)
		return -2;

	plt->memType = G2D_MEMORY_LINK;
	plt->memCount++;

	return 0;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// sort the color in the palette.
void g2dPaletteSort (g2dPalette *plt)
{
	if (plt == NULL)
        return;

	int i, iLow;
	int iStart = 0;
	int count = plt->count;
	int iMax = plt->count;
	g2dColor32 Clr;

	// Sort the Color palette.
	while (count)
	{
		// Get the lowest color.
		for (i = iStart, iLow = iStart, Clr = 0xffffffff; i < iMax; ++i)
			if (plt->data[i] < Clr)
				{iLow = i; Clr = plt->data[i];}
		// Move the palette colors.
		for (i = iLow; i > iStart; --i )
			plt->data[i] = plt->data[i - 1];
		plt->data[iStart] = Clr;
		++iStart; --count;
	}
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
static g2dPalette* _g2dPaletteLoad (FILE *fp)
{
	long int size = 0;
	char temp[5] = {'\0','\0','\0','\0','\0'};

	// Get the file size.
	fseek(fp, 0, SEEK_END);
	fgetpos(fp, &size);
	rewind(fp);

	// Check the file header.
	fread(temp, sizeof(char), 4, fp);
	if (strcmp(temp,"PLT%"))
		return NULL;

	// Read the data lengtht.
	int count = 0;
	fread(&count, sizeof(g2dColor32), 1, fp);

	// Check the file size.
	if (count*sizeof(g2dColor32)+8 != size)
		return NULL;

	// Read the file data.
	g2dPalette_Type type = (count == 16 ? G2D_PALETTE_T4 : (count == 256 ? G2D_PALETTE_T8 : G2D_PALETTE_NONE));
	g2dPalette *plt = g2dPaletteCreate(type, count);
	if (plt == NULL)
		return NULL;
	fread(plt->data, sizeof(g2dColor32), plt->count, fp);

	return plt;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
g2dPalette* g2dPaletteLoad (const char *path)
{
	g2dPalette *plt = NULL;
	FILE *fp = NULL;

	if (path == NULL)
        return NULL;
	if ((fp = fopen(path, "rb")) == NULL)
        return NULL;

	char* suffix = strrchr(path, '.');

	if (stricmp(suffix, ".plt") == 0)
        plt = _g2dPaletteLoad(fp);

	if (plt == NULL)
        goto label_error;

	fclose(fp);
    fp = NULL;

	return plt;

	// Load failure... abort
label_error:
    if (fp != NULL)
        fclose(fp);

    g2dPaletteFree(&plt);

    return NULL;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
int g2dPaletteSave (const char *path, g2dPalette* plt)
{
	if (path == NULL)
        return -1;

	if (plt == NULL)
        return -2;

	FILE * fp = fopen(path, "wb");
	if (fp == NULL)
		return -3;

	char temp[5] = {'P','L','T','%','\0'};

	fwrite(temp, sizeof(char), 4, fp);
	fwrite(&(plt->count), sizeof(int), 1, fp);
	fwrite(plt->data, sizeof(g2dColor32), plt->count, fp);
	fclose(fp);

	return 0;
}
// -----------------------------------------------------------------------------------------------------
