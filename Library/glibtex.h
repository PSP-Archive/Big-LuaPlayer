// glibtex - Texture support library for glib2d. ( FileName: glibtex.h )
// ------------------------------------------------------------------------
// Version: 1.00
// Copyright (c) 2012 M4MSOFT
// ------------------------------------------------------------------------

#ifndef GLIBTEX_H_
#define GLIBTEX_H_

#ifdef __cplusplus
extern "C" {
#endif

// Get or set a pixel.
#define G2D_TARGET_GETPIXEL(target, x, y, w) (*(target + x + y * w))
#define G2D_TARGET_SETPIXEL(target, x, y, w, color) (*(target + x + y * w) = color)
// Create a color.
#define G2D_RGBASQUARE(r, g, b, a) (((r)+(g)+(b)+(a))?(((r)?(r):1)*((g)?(g):1)*((b)?(b):1)*((a)?(a):1)):0)
#define G2D_RGBA16(r, g, b, a) ((r)|((g)<<4)|((b)<<8)|((a)<<12))
#define G2D_RGBA8(r, g, b, a) ((r)|((g)<<2)|((b)<<4)|((a)<<6))
#define G2D_RGBA4(r, g, b, a) ((r)|((g)<<1)|((b)<<2)|((a)<<3))
// Get the delta of to colors.
#define G2D_VALUE_DELTA(a, b) ((a)>(b)?(a)-(b):(b)-(a))
#define G2D_COLOR_DELTA(a, b) \
	    G2D_RGBASQUARE(G2D_VALUE_DELTA(G2D_GET_R(a), G2D_GET_R(b)), \
					   G2D_VALUE_DELTA(G2D_GET_G(a), G2D_GET_G(b)), \
					   G2D_VALUE_DELTA(G2D_GET_B(a), G2D_GET_B(b)), \
					   G2D_VALUE_DELTA(G2D_GET_A(a), G2D_GET_A(b))  )
// Get the color value form a 16 bit color.
#define G2D_GET_R16(color) ((((color)      ) & 0xF) * 0x11)
#define G2D_GET_G16(color) ((((color) >>  4) & 0xF) * 0x11)
#define G2D_GET_B16(color) ((((color) >>  8) & 0xF) * 0x11)
#define G2D_GET_A16(color) ((((color) >> 12) & 0xF) * 0x11)
// Get the color value form a 8 bit color.
#define G2D_GET_R8(color, pltd) ( (pltd) ? G2D_GET_R((pltd)[(color)]) : (((color)     ) & 0x03) * 0x55)
#define G2D_GET_G8(color, pltd) ( (pltd) ? G2D_GET_G((pltd)[(color)]) : (((color) >> 2) & 0x03) * 0x55)
#define G2D_GET_B8(color, pltd) ( (pltd) ? G2D_GET_B((pltd)[(color)]) : (((color) >> 4) & 0x03) * 0x55)
#define G2D_GET_A8(color, pltd) ( (pltd) ? G2D_GET_A((pltd)[(color)]) : (((color) >> 6) & 0x03) * 0x55)
// Get the color value form a 4 bit color.
#define G2D_GET_R4(color, pltd) ( (pltd) ? G2D_GET_R((pltd)[(color)]) : (((color)     ) & 0x1) * 0xFF)
#define G2D_GET_G4(color, pltd) ( (pltd) ? G2D_GET_G((pltd)[(color)]) : (((color) >> 1) & 0x1) * 0xFF)
#define G2D_GET_B4(color, pltd) ( (pltd) ? G2D_GET_B((pltd)[(color)]) : (((color) >> 2) & 0x1) * 0xFF)
#define G2D_GET_A4(color, pltd) ( (pltd) ? G2D_GET_A((pltd)[(color)]) : (((color) >> 3) & 0x1) * 0xFF)
// Convert a 16 bit color to a 32 bit color.
#define G2D_COLOR_FROM16(color) \
	G2D_RGBA((g2dColor32) G2D_GET_R16(color), \
			 (g2dColor32) G2D_GET_G16(color), \
			 (g2dColor32) G2D_GET_B16(color), \
			 (g2dColor32) G2D_GET_A16(color))
// Convert a 8 bit color to a 32 bit color.
#define G2D_COLOR_FROM8(color, pltd) \
	G2D_RGBA((g2dColor32) G2D_GET_R8(color, pltd), \
			 (g2dColor32) G2D_GET_G8(color, pltd), \
			 (g2dColor32) G2D_GET_B8(color, pltd), \
			 (g2dColor32) G2D_GET_A8(color, pltd))
// Convert a 4 bit color to a 32 bit color.
#define G2D_COLOR_FROM4(color, pltd) \
	G2D_RGBA((g2dColor32) G2D_GET_R4(color, pltd), \
			 (g2dColor32) G2D_GET_G4(color, pltd), \
			 (g2dColor32) G2D_GET_B4(color, pltd), \
			 (g2dColor32) G2D_GET_A4(color, pltd))
// Convert the 32 bit color value to a 16 bit color value.
#define G2D_GET_RTO16(color) ((((color) >>  4) & 0xF))
#define G2D_GET_GTO16(color) ((((color) >> 12) & 0xF))
#define G2D_GET_BTO16(color) ((((color) >> 20) & 0xF))
#define G2D_GET_ATO16(color) ((((color) >> 28) & 0xF))
// Convert the 32 bit color value to a 8 bit color value.
#define G2D_GET_RTO8(color) ((((color) >>  6) & 0x3))
#define G2D_GET_GTO8(color) ((((color) >> 14) & 0x3))
#define G2D_GET_BTO8(color) ((((color) >> 22) & 0x3))
#define G2D_GET_ATO8(color) ((((color) >> 30) & 0x3))
// Convert the 32 bit color value to a 4 bit color value.
#define G2D_GET_RTO4(color) ((((color) >>  7) & 0x1))
#define G2D_GET_GTO4(color) ((((color) >> 15) & 0x1))
#define G2D_GET_BTO4(color) ((((color) >> 23) & 0x1))
#define G2D_GET_ATO4(color) ((((color) >> 31) & 0x1))
// Convert the 32 bit color to a 16 bit color.
#define G2D_COLOR_TO16(color) \
	(g2dColor16) G2D_RGBA16(G2D_GET_RTO16(color), \
					        G2D_GET_GTO16(color), \
							G2D_GET_BTO16(color), \
							G2D_GET_ATO16(color))
// Convert the 32 bit color to a 8 bit color.
#define G2D_COLOR_TO8(color) \
	(g2dColor8) G2D_RGBA8(G2D_GET_RTO8(color), \
						  G2D_GET_GTO8(color), \
						  G2D_GET_BTO8(color), \
						  G2D_GET_ATO8(color))
// Convert the 32 bit color to a 4 bit color.
#define G2D_COLOR_TO4(color) \
	(g2dColor8) G2D_RGBA4(G2D_GET_RTO4(color), \
						  G2D_GET_GTO4(color), \
						  G2D_GET_BTO4(color), \
						  G2D_GET_ATO4(color))
// Blend two colors.
// Ac = (At*255 + Af*255 - At*Af);
// Av =  Ac >> 8;
// Cv = Ct - (Ac?Ct*Af*255/Ac:0) + (Ac?Cf*Af*255/Ac:0);
#define G2D_ALPHA_GETAC(At, Af) ((At)*255+(Af)*255-(At)*(Af))
#define G2D_COLOR_BLEND_VALUE(Ct, Cf, Af, Ac) ((Ct)-(Ac?((Ct)*(Af)*255)/Ac:0)+(Ac?((Cf)*(Af)*255)/Ac:0))
#define G2D_COLOR_BLEND(Ct, Cf, Af, Ac) \
	G2D_RGBA(G2D_COLOR_BLEND_VALUE(G2D_GET_R(Ct), G2D_GET_R(Cf), Af, Ac), \
			 G2D_COLOR_BLEND_VALUE(G2D_GET_G(Ct), G2D_GET_G(Cf), Af, Ac), \
			 G2D_COLOR_BLEND_VALUE(G2D_GET_B(Ct), G2D_GET_B(Cf), Af, Ac), \
			 (Ac) / 255 )
#define G2D_COLOR_BLEND_SLOW(Ct, Cf) G2D_COLOR_BLEND_AC(Ct, Cf, G2D_GET_A(Cf), G2D_ALPHA_GETAC(G2D_GET_A(Ct), G2D_GET_A(Cf)))

// Get or set a pixel
#define G2D_TEX_GETPIXEL(tex, x, y, color, pltp) \
{ \
	if ((tex) && (x) >= 0 && (x) < (tex)->tw && (y) >= 0 && (y) < (tex)->sh) { \
		int n; \
		switch ((tex)->psm) { \
			case G2D_PSM_8888: *(color) = G2D_TARGET_GETPIXEL((g2dColor32*)(tex)->data, (x), (y), (tex)->tw); break; \
			case G2D_PSM_4444: *(color) = G2D_COLOR_FROM16(G2D_TARGET_GETPIXEL((g2dColor16*)(tex)->data, (x), (y), (tex)->tw)); break; \
			case G2D_PSM_T8: *(color) = G2D_COLOR_FROM8(G2D_TARGET_GETPIXEL((g2dColor8*)(tex)->data, (x), (y), (tex)->tw), ((pltp) && (pltp)->type == G2D_PALETTE_T8 ? (pltp)->data : G2D_PLT_NULL)); break; \
			case G2D_PSM_T4: n = (x) + (y) * (tex)->tw; *(color) = G2D_COLOR_FROM4((*((g2dColor8*)(tex)->data + (n >> 1)) >> (4*(n&0x1))) & 0xF, ((pltp) && (pltp)->type == G2D_PALETTE_T4 ? (pltp)->data : G2D_PLT_NULL)); break; \
			default: *(color) = 0;} \
	} else *(color) = 0; \
}
#define G2D_TEX_SETPIXEL(tex, x, y, color, blend, pltp) \
{ \
	if ((tex) && (x) >= 0 && (x) < (tex)->tw && (y) >= 0 && (y) < (tex)->sh) { \
		int n; \
		g2dColor Cf = (color); \
		if (blend && (G2D_GET_A(Cf) < 255)){ \
			g2dColor Ct = 0; \
			G2D_TEX_GETPIXEL(tex, x, y, &Ct, pltp) \
			g2dColor Ac = G2D_GET_A(Ct); \
			g2dColor Af = G2D_GET_A(Cf); \
			Ac = G2D_ALPHA_GETAC(Ac, Af); \
			Cf = G2D_COLOR_BLEND(Ct, Cf, Af, Ac);} \
		switch ((tex)->psm) { \
			case G2D_PSM_8888: G2D_TARGET_SETPIXEL((g2dColor32*)(tex)->data, (x), (y), (tex)->tw, Cf); break; \
			case G2D_PSM_4444: G2D_TARGET_SETPIXEL((g2dColor16*)(tex)->data, (x), (y), (tex)->tw, G2D_COLOR_TO16(Cf)); break; \
			case G2D_PSM_T8: \
				if ((pltp) && (pltp)->data && (pltp)->type == G2D_PALETTE_T8){ \
					g2dColor32 i,c,t,b; \
					for (i = 0, c = 0x0, b = 0xffffffff; (i < 256) && b; ++i) \
						{t = G2D_COLOR_DELTA((pltp)->data[i], Cf); if (b >= t) {c = i; b = t;}} \
					Cf = G2D_COLOR_FROM8(c, G2D_PLT_NULL);} \
				G2D_TARGET_SETPIXEL((g2dColor8*)(tex)->data, (x), (y), (tex)->tw, G2D_COLOR_TO8(Cf)); break; \
			case G2D_PSM_T4: \
				if ((pltp) && (pltp)->data && (pltp)->type == G2D_PALETTE_T4){ \
					g2dColor32 i,c,t,b; \
					for (i = 0, c = 0x0, b = 0xffffffff; (i < 16) && b; ++i) \
						{t = G2D_COLOR_DELTA((pltp)->data[i],Cf); if (b >= t) {c = i; b = t;}} \
					Cf = G2D_COLOR_FROM4(c, G2D_PLT_NULL);} \
				n = (x) + (y) * (tex)->tw; \
				if (n & 0x1) *((g2dColor8*)(tex)->data + (n >> 1)) = (G2D_COLOR_TO4(Cf) << 4) | (*((g2dColor8*)(tex)->data + (n >> 1)) & 0x0F); \
				else *((g2dColor8*)(tex)->data + (n >> 1)) = (*((g2dColor8*)(tex)->data + (n >> 1)) & 0xF0) | G2D_COLOR_TO4(Cf); break; \
			default: break;} \
	} \
}

/**
 * \def STRIP_TEXTURE_HEIGHT
 * \brief Choose texture height strip is enabled.
 *
 * This needs to be enable to draw correctly on a texture
 * using the g2dSetDrawTarget function, Otherwise target drawing
 * will not work nicely ???
 */
#define STRIP_TEXTURE_HEIGHT

/**
 * \def USE_PNG
 * \brief Choose if the PNG support is enabled.
 *
 * Otherwise, this part will be not compiled to gain some space.
 * Enable this to get PNG support, disable to avoid compilation errors
 * when libpng is not linked in the Makefile.
 */
/**
 * \def USE_JPEG
 * \brief Choose if the JPEG support is enabled.
 *
 * Otherwise, this part will be not compiled to gain some space.
 * Enable this to get JPEG support, disable to avoid compilation errors
 * when libjpeg is not linked in the Makefile.
 */
#define USE_PNG
#define USE_JPEG

// -----------------------------------------------------------------------------------------------------
// enumerations
typedef enum
{
    G2D_DEFAULT = 0,
	G2D_SWIZZLE = 1 // < Recommended. Use it to speedup rendering.
} g2dTex_Mode;

//   GU_PSM_5650    (0)  - Hicolor, 16-bit - (Not in use.)
//   GU_PSM_5551    (1)  - Hicolor, 16-bit - (Not in use.)
//   --------------------------------------------------------
//   GU_PSM_4444    (2)  - Hicolor, 16-bit
//   GU_PSM_8888    (3)  - Truecolor, 32-bit
//   GU_PSM_T4      (4)  - Indexed, 4-bit (2 pixels per byte)
//   GU_PSM_T8      (5)  - Indexed, 8-bit
//   --------------------------------------------------------
//   GU_PSM_T16		(6)  - Hicolor, 16-bit - (Not in use.)
//   GU_PSM_T32		(7)  - Truecolor, 32-bit - (Not in use.)
//   GU_PSM_DXT1    (8)  - No info - (Not in use.)
//   GU_PSM_DXT3	(9)  - No info - (Not in use.)
//   GU_PSM_DXT5	(10) - No info - (Not in use.)
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Structures
typedef struct
{
    int tw;                /**< Real texture width. A power of two. */
    int th;                /**< Real texture height. A power of two. */
	int sh;                /**< Stripped texture height. A square of eight. */
    int w;                 /**< Texture width, as seen when drawing. */
    int h;                 /**< Texture height, as seen when drawing. */
    float ratio;           /**< Width/height ratio. */
	int psm;			    /**< The pixel format of the texture. */
	bool swizzled;          /**< Is the texture swizzled ? */
	bool vram;              /**< Is the texture Stored in the vram ? */
	g2dPalette *plt;        /**< the pointer to the palette. */
	g2dMemory_Type memType; /**< The memory type of the texture. */
	int memCount;           /**< The total amount of memory links. */
	unsigned int size;     /**< The Size of the data. */
    void *data;             /**< Pointer to raw data. */
} g2dTexture;
// -----------------------------------------------------------------------------------------------------

/**
 * \var g2d_draw_buffer
 * \brief The current draw buffer as a texture.
 */
/**
 * \var g2d_disp_buffer
 * \brief The current display buffer as a texture.
 */
/**
 * \var g2d_disp_buffer
 * \brief The current depht buffer as a texture.
 */
extern g2dTexture g2d_draw_buffer;
extern g2dTexture g2d_disp_buffer;
extern g2dTexture g2d_depht_buffer;

// -----------------------------------------------------------------------------------------------------
// Creates a new blank texture.
//
// @param: w - Width of the texture.
// @param h - Height of the texture.
// @param psm - Pixelformat of the texture.
// @param vram - store the texture in the vram.
//
// @return: This function returns NULL on allocation fail.
//
g2dTexture* g2dTexCreate(int w, int h, int psm, bool vram);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Move the texture data to the ram memory.
//
// @param tex - Pointer to a texture, to move to the ram.
//
// @return: int - 0 on succes.
//
int g2dTexToRam(g2dTexture *tex);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Move the texture data to the Vram memory.
//
// @param tex - Pointer to a texture, to move to the vram.
//
// @return: int - 0 on succes.
//
int g2dTexToVram(g2dTexture *tex);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Swizzle the texture data.
//
// @param tex - Pointer to a texture, to swizzle.
//
// @return: int - 0 on succes.
//
int g2dTexSwizzle(g2dTexture *tex);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Unswizzle the texture data.
//
// @param tex - Pointer to a texture, to Unswizzle.
//
// @return: int - 0 on succes.
//
int g2dTexUnSwizzle(g2dTexture *tex);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Frees a texture & set its pointer to NULL.
//
// @param: tex - Pointer to the variable which contains the texture pointer.
//
// @return: void
//   This function is used to gain memory when a texture is useless.
//   Must pass the pointer to the variable which contains the pointer,
//   to set it to NULL (passing NULL to a g2dBegin* function is safe).
//
void g2dTexFree(g2dTexture **tex);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Add a link to the texture.
//
// @param tex - Pointer to a texture,
//
// @return: int - 0 on succes.
//
int g2dTexAddLink(g2dTexture *tex);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Loads an image.
//
// @param path Path to the file.
// @param psm - Pixelformat of the texture.
// @param vram - store the texture in the vram.
// @param tex_mode A g2dTex_Mode constant.
// @returns Pointer to the generated texture.
//
// @return: g2dTexture
//   This function loads an image file. There is support for PNG & JPEG files
//   (if USE_PNG and USE_JPEG are defined). Swizzling is enabled only for 16*16+
//   textures (useless on small textures), pass G2D_SWIZZLE to enable it.
//   Texture supported up to 512*512 in size only (hardware limitation).
//
g2dTexture* g2dTexLoad(const char *path, int psm, bool vram, g2dTex_Mode mode);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Save an image.
//
// @param path - The name of the file.
// @param tex - Pointer to a texture, to save.
// @param saveAlpha - Save the alpha value.
// @param usePalette - Use the color palette to save the texture.
//
// @return: int - 0 on succes.
//
int g2dTexSave(const char *path, g2dTexture *tex, bool saveAlpha, bool usePalette);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Creates a palette from a texture that can be used to convert the texture.
//
// @param tex - Pointer to a texture.
// @param: type - The type of the palette. to create
//
// @return: This function returns NULL on allocation fail.
//
g2dPalette* g2dTexCreatePalette (g2dTexture *tex, g2dPalette_Type type);
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Get a pixel from the screen buffer.
//
// @param x - The x position.
// @param y - The y position.
//
// @return: g2dColor - The color of the pixel.
//
g2dColor g2dGetPixel(int x, int y);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Get a pixel from the a texture.
//
// @param target - Pointer to a texture.
// @param x - The x position.
// @param y - The y position.
//
// @return: g2dColor - The color of the pixel.
//
g2dColor g2dTexGetPixel(g2dTexture* target, int x, int y);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Set a pixel to the screen.
//
// @param x - The x position.
// @param y - The y position.
// @param color - The new color of the pixel.
// @param blend - Blend of overwrite the color.
//
// @return: void
//
void g2dSetPixel(int x, int y, g2dColor color, bool blend);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Set a pixel to the texture..
//
// @param target - Pointer to a texture.
// @param x - The x position.
// @param y - The y position.
// @param color - The new color of the pixel.
// @param blend - Blend of overwrite the color.
// @param plt - The current color palette.
//
// @return: void
//
void g2dTexSetPixel(g2dTexture* target, int x, int y, g2dColor color, bool blend);
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Blit the texture to a texture.
//
// @param target - Pointer to the destiny texture.
// @param x - The x position.
// @param y - The y position.
// @param tex - Pointer to the source texture.
// @param blend - Blend of overwrite the color.
// @param sx - The source x position.
// @param sy - The source y position.
// @param sw - The source width.
// @param sh - The source height.
// @param blend - Blend of overwrite the color.
// @param plt - The current color palette.
// @param pltScr - The current texture source color palette.
//
// @return: void
//
void g2dTexBlitToTex(g2dTexture* target, int dx, int dy, g2dTexture* tex, int sx, int sy, int sw, int sh, bool blend);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Draw a line.
//
// @param target - Pointer to the destiny texture.
// @param x0 - The x start position.
// @param y0 - The y start position.
// @param x1 - The x stop position.
// @param y1 - The y stop position.
// @param color - The new color of the line.
// @param blend - Blend of overwrite the color.
// @param plt - The current color palette.
//
// @return: void
//
void g2dTexDrawLine(g2dTexture* target, int x0, int y0, int x1, int y1, g2dColor color, bool blend);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Draw a rectangle.
//
// @param target - Pointer to the destiny texture.
// @param x - The x position.
// @param y - The y position.
// @param w - The width of the rectangle.
// @param h - The hieght of the rectangle.
// @param color - The new color of the rectangle.
// @param blend - Blend of overwrite the color.
// @param plt - The current color palette.
//
// @return: void
//
void g2dTexDrawRect(g2dTexture* target, int x, int y, int w, int h, g2dColor color, bool blend);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Fill a rectangle.
//
// @param target - Pointer to the destiny texture.
// @param x - The x position.
// @param y - The y position.
// @param w - The width of the rectangle.
// @param h - The hieght of the rectangle.
// @param color - The new color of the rectangle.
// @param blend - Blend of overwrite the color.
// @param plt - The current color palette.
//
// @return: void
//
void g2dTexFillRect(g2dTexture* target, int x, int y, int w, int h, g2dColor color, bool blend);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Clear the texture.
//
// @param target - Pointer to the destiny texture.
// @param color - The clear color.
// @param blend - Blend of overwrite the color.
// @param plt - The current color palette.
//
// @return: void
//
void g2dTexClear(g2dTexture* target, g2dColor color, bool blend);
// -----------------------------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif  // GLIBTEX_H_
