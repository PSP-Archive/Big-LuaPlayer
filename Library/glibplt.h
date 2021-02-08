// glibplt - Palette support library for glib2d. ( FileName: glibplt.h )
// ------------------------------------------------------------------------
// Version: 1.00
// Copyright (c) 2012 M4MSOFT
// ------------------------------------------------------------------------

#ifndef GLIBPLT_H_
#define GLIBPLT_H_

#ifdef __cplusplus
extern "C" {
#endif

// -----------------------------------------------------------------------------------------------------
// Defines.
#define G2D_BLOKS_PSM_T4 (2)
#define G2D_BLOKS_PSM_T8 (32)

#define G2D_PSM_4444    (2)
#define G2D_PSM_8888    (3)
#define G2D_PSM_T4      (4)
#define	G2D_PSM_T8      (5)

#define G2D_PLT_NULL    ((unsigned int *)0)
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// enumuration
typedef enum
{
    G2D_PALETTE_NONE = 0,
	G2D_PALETTE_T4 = 1,
	G2D_PALETTE_T8 = 2
} g2dPalette_Type;
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Structures
typedef struct
{
	g2dPalette_Type type; /**< the palette type ? */
	unsigned int count; /**< the number of color in the palette ? */
	g2dMemory_Type memType; /**< The memory type of the texture. */
	int memCount;   /**< The total amount of memory links. */
	g2dColor *data; /**< the color palette data? */
} g2dPalette;
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Global variables
extern unsigned int g2d_palette_T4[G2D_BLOKS_PSM_T4*8];
extern unsigned int g2d_palette_T8[G2D_BLOKS_PSM_T8*8];

extern g2dPalette g2d_plt_T8;
extern g2dPalette g2d_plt_T4;
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Creates a new blank palette.
//
// @param: type - The type of the palette.
// @param: count - The Size of the palette.
//
// @return: This function returns NULL on allocation fail.
//
g2dPalette* g2dPaletteCreate (g2dPalette_Type type, unsigned int count);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Frees a palette & set its pointer to NULL.
//
// @param: plt - Pointer to the variable which contains the palette pointer.
//
// @return: void
//   This function is used to gain memory when a palette is useless.
//   Must pass the pointer to the variable which contains the pointer,
//
void g2dPaletteFree(g2dPalette **plt);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Add a link to the palette.
//
// @param plt - Pointer to a palette,
//
// @return: int - 0 on succes.
//
int g2dpaletteAddLink(g2dPalette *plt);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// sort the color in the palette.
//
// @param plt - Pointer to a palette, to sort.
//
// @return: void
//
void g2dPaletteSort (g2dPalette *plt);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Load a palette from a file.
//
// @param path - The name of the file.
//
// @return: This function returns NULL on allocation fail.
//
g2dPalette* g2dPaletteLoad (const char *path);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// save a palette to a file.
//
// @param path - The name of the file.
// @param plt - Pointer to a palette. to save
//
// @return: int - 0 on succes.
//
int g2dPaletteSave (const char *path, g2dPalette* plt);
// -----------------------------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif  // GLIBPLT_H_
