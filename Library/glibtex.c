// glibtex - Texture support library for glib2d. ( FileName: glibtex.h )
// ------------------------------------------------------------------------
// Version: 1.00
// Copyright (c) 2012 M4MSOFT
// ------------------------------------------------------------------------

#include "glib2d.h"

#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspgu.h>
#include <valloc.h>
#include <malloc.h>

#ifdef USE_PNG
#include <png.h>
#endif

#ifdef USE_JPEG
#include <jpeglib.h>
#endif

#include <malloc.h>

#define G2D_CLAMP(val, min, max) ((val)>(max)?(max):((val)<(min)?(min):(val)))

// -----------------------------------------------------------------------------------------------------
// Get the next power of two.
static unsigned int _getNextPowerOf2(unsigned int value)
{
	unsigned int result = value;
	unsigned int n = 0;
	for ( n = 0; value; ++n)
		value >>= 1;
	value = result;
	result = 1 << n;
	if (result == 2 * value) result >>= 1;
	return result;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Return the next square of eight.
#ifdef STRIP_TEXTURE_HEIGHT
static unsigned int _getNextSquareOf8(unsigned int value)
{
	unsigned int result = value;
	unsigned int n = 0;
	for ( n = 1; value > 8; ++n)
		value -= 8;
	value = result;
	result = n * 8;
	if (result == value + 8) result -= 8;
	return result;
}
static unsigned int _vgetMemorySize(int width, int height, int psm)
{
	switch (psm) {
		case G2D_PSM_T4: return (width * height) >> 1;
		case G2D_PSM_T8: return width * height;
		case G2D_PSM_4444: return 2 * width * height;
		case G2D_PSM_8888: return 4 * width * height;
		default: return 0;
	}
}
#endif
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Swizzle the texture.
static void _swizzle(unsigned char *dest, unsigned char *source, int width, int height)
{
    int i, j;
    int rowblocks = (width / 16);
    int rowblocks_add = (rowblocks-1) * 128;
    unsigned int block_address = 0;
	unsigned int *block = (unsigned int*)(0);
    unsigned int *img = (unsigned int*)source;
    for (j=0; j<height; j++)
    {
        block = (unsigned int*)(dest + block_address);
        for (i = 0; i < rowblocks; i++)
        {
            *block++ = *img++;
            *block++ = *img++;
            *block++ = *img++;
            *block++ = *img++;
            block += 28;
        }
        if ((j & 0x7) == 0x7)
            block_address += rowblocks_add;
        block_address += 16;
    }
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Unswizzle the texture.
static void _unswizzle(unsigned char *dest, unsigned char *source, int width, int height)
{
    int i, j;
    int rowblocks = (width / 16);
    int rowblocks_add = (rowblocks-1) * 128;
    unsigned int block_address = 0;
	unsigned int *block = (unsigned int*)dest;
    unsigned int *img = (unsigned int*)(0);
    for (j=0; j<height; j++)
    {
        img = (unsigned int*)(source + block_address);
        for (i = 0; i < rowblocks; i++)
        {
            *block++ = *img++;
            *block++ = *img++;
            *block++ = *img++;
            *block++ = *img++;
            img += 28;
        }
        if ((j & 0x7) == 0x7)
            block_address += rowblocks_add;
        block_address += 16;
    }
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Creates a new blank texture.
g2dTexture* g2dTexCreate(int w, int h, int psm, bool vram)
{
	// The PSP can't draw 512*512+ textures.
    if (w > 512 || h > 512)
        return NULL;

	// Check the pixel format.
	if (psm != G2D_PSM_4444 && psm != G2D_PSM_8888 && psm != G2D_PSM_T4 && psm != G2D_PSM_T8)
		return NULL;

	g2dTexture *tex = malloc(sizeof(g2dTexture));
    if (tex == NULL)
        return NULL;

	// The width of the texture needs to be aleast 4.
    tex->tw = w > 2 ? _getNextPowerOf2(w) : 4;
    tex->th = _getNextPowerOf2(h);
#ifdef STRIP_TEXTURE_HEIGHT
	// The Height Offset needs to be equal to 2^(11 - current width Power of 2),
	// Apparently Needed for targetDrawing ???
	tex->sh = _getNextSquareOf8(h + 8192/_vgetMemorySize(tex->tw, 1, psm));
#else
	tex->sh = tex->th;
#endif
    tex->w = w;
    tex->h = h;
    tex->ratio = (float)w / h;
	tex->psm = psm;
    tex->swizzled = false;
	tex->vram = vram;
	tex->plt = NULL;
	tex->memType = G2D_MEMORY_DEFAULT;
	tex->memCount = 0;
#ifdef STRIP_TEXTURE_HEIGHT
	tex->size = _vgetMemorySize(tex->tw, tex->sh, tex->psm);
#else
	tex->size = vgetMemorySize(tex->tw, tex->sh, tex->psm);
#endif

	if (tex->vram)
	{
		tex->data = valloc(tex->size);
		if (tex->data == NULL)
		{
			free(tex);
			return NULL;
		}
	}
	else
	{
		tex->data = malloc(tex->size);
		if (tex->data == NULL)
		{
			free(tex);
			return NULL;
		}
	}
	memset(tex->data, 0, tex->size);

    return tex;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Move the texture data to the ram memory.
int g2dTexToRam(g2dTexture *tex)
{
    if (tex == NULL)
        return -1;

	if (!tex->vram)
		return 0;

	// Get the new data pointer.
	u8 *dst = malloc(tex->size);
	if (dst == NULL)
   		return -2;
	memset((void*)dst, 0, tex->size);

	// Move the data.
	u8 *src = (u8*)tex->data;
	int i;
	for (i = 0; i < tex->size; ++i)
		*(dst + i) = *(src + i);

	// free the vram memory.
	vfree(tex->data);

	// Set the new data pointer.
	tex->data =(void*)dst;
	tex->vram = false;

	sceKernelDcacheWritebackRange(tex->data, tex->size);

    return 0;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Move the texture data to the Vram memory.
int g2dTexToVram(g2dTexture *tex)
{
    if (tex == NULL)
        return -1;

	if (tex->vram)
		return 0;

	// Get the new data pointer.
	u8 *dst = valloc(tex->size);
	if (dst == NULL)
   		return -2;
	memset((void*)dst, 0, tex->size);

	// Move the data.
	u8 *src = (u8*)tex->data;
	int i;
	for (i = 0; i < tex->size; ++i)
		*(dst + i) = *(src + i);

	// free the ram memory.
	free(tex->data);

	// Set the new data pointer.
	tex->data =(void*)dst;
	tex->vram = true;

    return 0;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Swizzle the texture data.
int g2dTexSwizzle(g2dTexture *tex)
{
    if (tex == NULL)
        return -1;

    if (tex->swizzled)
        return 0;

	// Swizzling is useless with small textures.
    if (tex->size/tex->sh >= 16 && tex->sh >= 8)
    {
        u8 *tmp = malloc(tex->size);
		if (tmp != NULL)
		{
			_swizzle(tmp, (u8*)tex->data, tex->size/tex->sh, tex->sh);
			if (tex->vram)
			{
				vfree(tex->data);
				tex->data = (void*)tmp;
				tex->vram = false;
				g2dTexToVram(tex);
			}
			else
			{
				free(tex->data);
				tex->data = (void*)tmp;
			}
			tex->swizzled = true;
		}
		else
			return -2;
    }
    else
       return -3;

	return 0;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Unswizzle the texture data.
int g2dTexUnSwizzle(g2dTexture *tex)
{
    if (tex == NULL)
        return -1;

    if (!tex->swizzled)
        return 0;

	// Swizzling is useless with small textures.
    if (tex->size/tex->sh >= 16 && tex->sh >= 8)
    {
        u8 *tmp = malloc(tex->size);
		if (tmp != NULL)
		{
			_unswizzle(tmp, (u8*)tex->data, tex->size/tex->sh, tex->sh);
			if (tex->vram)
			{
				vfree(tex->data);
				tex->data = (void*)tmp;
				tex->vram = false;
				g2dTexToVram(tex);
			}
			else
			{
				free(tex->data);
				tex->data = (void*)tmp;
			}
			tex->swizzled = false;
		}
		else
			return -2;
    }
    else
       return -3;

	return 0;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Frees a texture & set its pointer to NULL.
void g2dTexFree(g2dTexture **tex)
{
    if (tex == NULL)
        return;
    if (*tex == NULL)
        return;

	// Break the link.
	if ((*tex)->memType == G2D_MEMORY_LINK)
	{
		if (((*tex)->memCount)-- <= 0)
			(*tex)->memType = G2D_MEMORY_DEFAULT;
	}

	switch ((*tex)->memType)
	{
		case G2D_MEMORY_DEFAULT: // Free the texture data.
			if ((*tex)->vram)
				vfree((*tex)->data);
			else
				free((*tex)->data);
		case G2D_MEMORY_STRUCT: // Free the texture structure.
			g2dPaletteFree(&((*tex)->plt));
			free((*tex));
		case G2D_MEMORY_POINTER: // Set the texture pointer to NULL.
			*tex = NULL;
		case G2D_MEMORY_STACK: // Don't free the texture.
			break;
		case G2D_MEMORY_LINK: // Set the palette pointer to NULL.
		default:
			*tex = NULL;
	}
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Add a link to the texture.
int g2dTexAddLink(g2dTexture *tex)
{
    if (tex == NULL)
        return -1;

	if (tex->memType != G2D_MEMORY_LINK && tex->memType != G2D_MEMORY_DEFAULT)
		return -2;

	tex->memType = G2D_MEMORY_LINK;
	tex->memCount++;

	return 0;
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
#ifdef USE_PNG
g2dTexture* _g2dTexLoadPNG(FILE *fp, int psm)
{
    png_structp png_ptr;
    png_infop info_ptr;
    unsigned int sig_read = 0;
    png_uint_32 width, height;
    int bit_depth, color_type, interlace_type;
    u32 x, y;
	g2dColor C;
    g2dColor *line;
    g2dTexture *tex;

    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL)
		return NULL;

    png_set_error_fn(png_ptr, NULL, NULL, NULL);
    info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL)
	{
		png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
		return NULL;
	}

    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, sig_read);
    png_read_info(png_ptr, info_ptr);
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
                 &interlace_type, NULL, NULL);
    png_set_strip_16(png_ptr);
    png_set_packing(png_ptr);

    if (color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png_ptr);
    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png_ptr);

    png_set_filler(png_ptr, 0xff, PNG_FILLER_AFTER);

    tex = g2dTexCreate(width, height, psm, false);
	if (tex == NULL)
	{
		png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
		return NULL;
	}

    line = malloc(width * 4);
	if (line == NULL)
	{
		g2dTexFree(&tex);
		png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
		return NULL;
	}

	// Load and convert the PNG data.
	switch (tex->psm)
	{
		case G2D_PSM_8888: // Truecolor, 32-bit
			for (y = 0; y < height; y++){
				png_read_row(png_ptr, (u8*) line, NULL);
				for (x = 0; x < width; x++)
					G2D_TARGET_SETPIXEL((g2dColor*)tex->data, x, y, tex->tw, line[x]);}
			break;
		case G2D_PSM_4444: // Hicolor, 16-bit
			for (y = 0; y < height; y++){
				png_read_row(png_ptr, (u8*) line, NULL);
				for (x = 0; x < width; x++){
					C = line[x]; G2D_TARGET_SETPIXEL((g2dColor*)tex->data, x, y, tex->tw, G2D_COLOR_TO16(C));}}
			break;
		case G2D_PSM_T8: // Indexed, 8-bit
			for (y = 0; y < height; y++){
				png_read_row(png_ptr, (u8*) line, NULL);
				for (x = 0; x < width; x++){
					C = line[x]; G2D_TARGET_SETPIXEL((g2dColor*)tex->data, x, y, tex->tw, G2D_COLOR_TO8(C));}}
			break;
		case G2D_PSM_T4: // Indexed, 4-bit (2 pixels per byte)
			for (y = 0; y < height; y++){
				int n;
				png_read_row(png_ptr, (u8*) line, NULL);
				for (x = 0; x < width; x++){
					C = line[x]; n = x + y * tex->tw;
					if (n & 0x1)
						*((g2dColor8*)tex->data + (n>>1)) = (G2D_COLOR_TO4(C)<<4) | (*((g2dColor8*)tex->data+(n>>1))&0x0F);
					else
						*((g2dColor8*)tex->data + (n>>1)) = (*((g2dColor8*)tex->data+(n>>1))&0xF0) | G2D_COLOR_TO4(C);}}
			break;
		default:
			g2dTexFree(&tex);
			free(line);
			png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
			return NULL;
	}

    free(line);
    png_read_end(png_ptr, info_ptr);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    return tex;
}

int _g2dTexSavePNG(FILE *fp, g2dTexture *tex, bool saveAlpha, bool usePalette)
{
	if (tex == NULL)
		return -1;

	png_structp png_ptr;
	png_infop info_ptr;
	png_uint_32 width, height;
	u32 i, x, y;
	u8 *line;

	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL)
		return -2; // Can't create the write structure.

	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL)
		return -3; // Can't create the info structure.

	// Set the info structure.
	width = tex->w;
	height = tex->h;
	png_init_io(png_ptr, fp);
	png_set_IHDR(png_ptr, info_ptr, width, height, 8,
		         saveAlpha ? PNG_COLOR_TYPE_RGBA : PNG_COLOR_TYPE_RGB,
	             PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
	png_write_info(png_ptr, info_ptr);

	// Set the alpha value.
	//if (saveAlpha && png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(png_ptr);
	//	png_set_filler(png_ptr, 0xff, PNG_FILLER_AFTER);

	line = malloc(width * (saveAlpha ? 4 : 3));
	if (line == NULL)
	{
		png_write_end(png_ptr, info_ptr);
		png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
		return -4;
	}

	unsigned int lineSize = tex->tw;
	g2dColor u32color = 0xffffffff;
	unsigned int* plt = NULL;
	unsigned int n;

	// Convert and save the texture.
	switch (tex->psm)
	{
		case G2D_PSM_8888: // Truecolor, 32-bit
			for (y = 0; y < height; y++){
				for (i = 0, x = 0; x < width; x++){
					u32color = G2D_TARGET_GETPIXEL((g2dColor32*)tex->data, x, y, lineSize);
					line[i++] = G2D_GET_R(u32color); line[i++] = G2D_GET_G(u32color); line[i++] = G2D_GET_B(u32color);
					if (saveAlpha) line[i++] = G2D_GET_A(u32color);}
				png_write_row(png_ptr, line);}
			break;
		case G2D_PSM_4444: // Hicolor, 16-bit
			for (y = 0; y < height; y++){
				for (i = 0, x = 0; x < width; x++){
					u32color = G2D_COLOR_FROM16(G2D_TARGET_GETPIXEL((g2dColor16*)tex->data, x, y, lineSize));
					line[i++] = G2D_GET_R(u32color); line[i++] = G2D_GET_G(u32color); line[i++] = G2D_GET_B(u32color);
					if (saveAlpha) line[i++] = G2D_GET_A(u32color);}
				png_write_row(png_ptr, line);}
			break;
		case G2D_PSM_T8: // Indexed, 8-bit
			if (usePalette) plt = (tex->plt && tex->plt->type == G2D_PALETTE_T8 ? tex->plt->data : G2D_PLT_NULL);
			for (y = 0; y < height; y++){
				for (i = 0, x = 0; x < width; x++){
					u32color = G2D_TARGET_GETPIXEL((g2dColor8*)tex->data, x, y, lineSize); u32color = G2D_COLOR_FROM8(u32color, plt);
					line[i++] = G2D_GET_R(u32color); line[i++] = G2D_GET_G(u32color); line[i++] = G2D_GET_B(u32color);
					if (saveAlpha) line[i++] = G2D_GET_A(u32color);}
				png_write_row(png_ptr, line);}
			break;
		case G2D_PSM_T4: // Indexed, 4-bit (2 pixels per byte)
			if (usePalette) plt = (tex->plt && tex->plt->type == G2D_PALETTE_T4 ? tex->plt->data : G2D_PLT_NULL);
			for (y = 0; y < height; y++){
				for (i = 0, x = 0; x < width; x++){
					n = x + y * lineSize;
					u32color = (*((g2dColor8*)tex->data + (n >> 1)) >> (4*(n&0x1))) & 0xF; u32color = G2D_COLOR_FROM4(u32color, plt);
					line[i++] = G2D_GET_R(u32color); line[i++] = G2D_GET_G(u32color); line[i++] = G2D_GET_B(u32color);
					if (saveAlpha) line[i++] = G2D_GET_A(u32color);}
				png_write_row(png_ptr, line);}
			break;
		default:
			free(line);
			png_write_end(png_ptr, info_ptr);
			png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
			return -5;
	}
	free(line);
	png_write_end(png_ptr, info_ptr);
	png_destroy_write_struct(&png_ptr, (png_infopp)NULL);

    return 0;
}
#endif
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
#ifdef USE_JPEG
g2dTexture* _g2dTexLoadJPEG(FILE *fp, int psm)
{
    struct jpeg_decompress_struct dinfo;
    struct jpeg_error_mgr jerr;
    int width, height;
    g2dTexture *tex;
    u8 *line;

    dinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&dinfo);
    jpeg_stdio_src(&dinfo, fp);
    jpeg_read_header(&dinfo, TRUE);

    width = dinfo.image_width;
    height = dinfo.image_height;

    tex = g2dTexCreate(width, height, psm, false);
	if (tex == NULL)
	{
		jpeg_destroy_decompress(&dinfo);
		return tex;
	}
    line = malloc(width * 3);
	if (line == NULL)
	{
		g2dTexFree(&tex);
		jpeg_destroy_decompress(&dinfo);
		return NULL;
	}

	// Load and convert the jpg data.
	jpeg_start_decompress(&dinfo);
	switch (tex->psm)
	{
		case G2D_PSM_8888: // Truecolor, 32-bit
			if (dinfo.jpeg_color_space == JCS_GRAYSCALE){
				while (dinfo.output_scanline < dinfo.output_height){
					int y = dinfo.output_scanline; int x;
					jpeg_read_scanlines(&dinfo, &line, 1);
					for (x=0; x<width; x++){
						g2dColor gray = line[x]; g2dColor c = gray | (gray << 8) | (gray << 16) | 0xff000000;
						G2D_TARGET_SETPIXEL((g2dColor*)tex->data, x, y, tex->tw, c);}}
			}else{
				while (dinfo.output_scanline < dinfo.output_height){
					int y = dinfo.output_scanline; int x; u8 *pline;
					jpeg_read_scanlines(&dinfo, &line, 1); pline = line;
					for (x=0; x<width; x++){
						g2dColor c; c = (*(pline++)); c |= (*(pline++)) << 8; c |= (*(pline++)) << 16; c |= 0xff000000;
						G2D_TARGET_SETPIXEL((g2dColor*)tex->data, x, y, tex->tw, c);}}
			}
			break;
		case G2D_PSM_4444: // Hicolor, 16-bit
			if (dinfo.jpeg_color_space == JCS_GRAYSCALE){
				while (dinfo.output_scanline < dinfo.output_height){
					int y = dinfo.output_scanline; int x;
					jpeg_read_scanlines(&dinfo, &line, 1);
					for (x=0; x<width; x++){
						g2dColor gray = line[x]; g2dColor c = gray | (gray << 8) | (gray << 16) | 0xff000000;
						G2D_TARGET_SETPIXEL((g2dColor*)tex->data, x, y, tex->tw, G2D_COLOR_TO16(c));}}
			}else{
				while (dinfo.output_scanline < dinfo.output_height){
					int y = dinfo.output_scanline; int x; u8 *pline;
					jpeg_read_scanlines(&dinfo, &line, 1); pline = line;
					for (x=0; x<width; x++){
						g2dColor c; c = (*(pline++)); c |= (*(pline++)) << 8; c |= (*(pline++)) << 16; c |= 0xff000000;
						G2D_TARGET_SETPIXEL((g2dColor*)tex->data, x, y, tex->tw, G2D_COLOR_TO16(c));}}
			}
			break;
		case G2D_PSM_T8: // Indexed, 8-bit
			if (dinfo.jpeg_color_space == JCS_GRAYSCALE){
				while (dinfo.output_scanline < dinfo.output_height){
					int y = dinfo.output_scanline; int x;
					jpeg_read_scanlines(&dinfo, &line, 1);
					for (x=0; x<width; x++){
						G2D_TARGET_SETPIXEL((g2dColor*)tex->data, x, y, tex->tw, line[x]);}}
			}else{
				while (dinfo.output_scanline < dinfo.output_height){
					int y = dinfo.output_scanline; int x; u8 *pline;
					jpeg_read_scanlines(&dinfo, &line, 1); pline = line;
					for (x=0; x<width; x++){
						g2dColor c; c = (*(pline++)); c |= (*(pline++)) << 8; c |= (*(pline++)) << 16; c |= 0xff000000;
						G2D_TARGET_SETPIXEL((g2dColor*)tex->data, x, y, tex->tw, G2D_COLOR_TO8(c));}}
			}
			break;
		case G2D_PSM_T4: // Indexed, 4-bit (2 pixels per byte)
			if (dinfo.jpeg_color_space == JCS_GRAYSCALE){
				while (dinfo.output_scanline < dinfo.output_height){
					int y = dinfo.output_scanline; int x; int n;
					jpeg_read_scanlines(&dinfo, &line, 1);
					for (x=0; x<width; x++){
						n = x + y * tex->tw;
						if (n & 0x1) *((g2dColor8*)tex->data + (n>>1)) = ((line[x]>>4)<<4) | (*((g2dColor8*)tex->data+(n>>1))&0x0F);
						else *((g2dColor8*)tex->data + (n>>1)) = (*((g2dColor8*)tex->data+(n>>1))&0xF0) | (line[x]>>4);}}
			}else{
				while (dinfo.output_scanline < dinfo.output_height){
					int y = dinfo.output_scanline; int x; int n; u8 *pline;
					jpeg_read_scanlines(&dinfo, &line, 1); pline = line;
					for (x=0; x<width; x++){
						n = x + y * tex->tw;
						g2dColor c; c = (*(pline++)); c |= (*(pline++)) << 8; c |= (*(pline++)) << 16; c |= 0xff000000;
						if (n & 0x1) *((g2dColor8*)tex->data + (n>>1)) = (G2D_COLOR_TO4(c)<<4) | (*((g2dColor8*)tex->data+(n>>1))&0x0F);
						else *((g2dColor8*)tex->data + (n>>1)) = (*((g2dColor8*)tex->data+(n>>1))&0xF0) | G2D_COLOR_TO4(c);}}
			}
			break;
		default:
			g2dTexFree(&tex);
			jpeg_finish_decompress(&dinfo);
			jpeg_destroy_decompress(&dinfo);
			free(line);
			return NULL;
	}

    jpeg_finish_decompress(&dinfo);
    jpeg_destroy_decompress(&dinfo);
    free(line);

    return tex;
}
#endif
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Loads an image.
g2dTexture* g2dTexLoad(const char *path, int psm, bool vram, g2dTex_Mode mode)
{
    g2dTexture *tex = NULL;
    FILE *fp = NULL;

    if (path == NULL)
        return NULL;
    if ((fp = fopen(path, "rb")) == NULL)
        return NULL;

	char* suffix = strrchr(path, '.');

#ifdef USE_PNG
    if (stricmp(suffix, ".png") == 0)
        tex = _g2dTexLoadPNG(fp, psm);
#endif

#ifdef USE_JPEG
    if (stricmp(suffix, ".jpg") == 0 || stricmp(suffix, ".jpeg") == 0)
        tex = _g2dTexLoadJPEG(fp, psm);
#endif

    if (tex == NULL)
        goto error;

    fclose(fp);
    fp = NULL;

    // Swizzling is useless with small textures.
    if (mode & G2D_SWIZZLE)
		g2dTexSwizzle(tex);

    sceKernelDcacheWritebackRange(tex->data, tex->size);

	// Move the texture to the vram.
	if (vram)
		g2dTexToVram(tex);

    return tex;

    // Load failure... abort
error:
    if (fp != NULL)
        fclose(fp);

    g2dTexFree(&tex);

    return NULL;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Saves an image.
int g2dTexSave(const char *path, g2dTexture* tex, bool saveAlpha, bool usePalette)
{
    if (tex == NULL)
		return -1;

	if (tex->swizzled)
		return -2;

	int intError = 0;
    FILE *fp = NULL;

    if (path == NULL)
        return -3;
    if ((fp = fopen(path, "wb")) == NULL)
        return -4;

#ifdef USE_PNG
	intError = _g2dTexSavePNG(fp, tex, saveAlpha, usePalette);
#endif

    fclose(fp);
    fp = NULL;

    return (intError ? intError - 4 : 0);
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Creates a palette from a texture.
g2dPalette* g2dTexCreatePalette (g2dTexture *tex, g2dPalette_Type type)
{
	if (tex == NULL)
        return NULL;

	// Set the palette size.
	unsigned int count = 0;
	switch (type){
		case G2D_PALETTE_T8: count = 256; break;
		case G2D_PALETTE_T4: count = 16; break;
		default: type = G2D_PALETTE_NONE; break;}

	if (count == 0)
		return NULL;

	g2dPalette *plt = g2dPaletteCreate(type, count);
	if (plt == NULL)
		return NULL;

	int i, iPlt;
	int iMax = tex->tw * tex->sh;
	int iPltMax = count;
	g2dColor32 Clr;
	unsigned int* pltData = NULL;

	// Get the first 16 or 256 color out of the texture.
	switch (tex->psm)
	{
		case G2D_PSM_8888: // Truecolor, 32-bit
			for (i = 0; (i < iMax) && count; ++i){
				Clr = (g2dColor32)(*((g2dColor32*)tex->data + i));
				for (iPlt = 0; iPlt < iPltMax; ++iPlt) if (plt->data[iPlt] == Clr) break;
				if (iPlt == iPltMax) plt->data[iPltMax - count--] = Clr;}
			break;
		case G2D_PSM_4444: // Hicolor, 16-bit
			for (i = 0; (i < iMax) && count; ++i){
				Clr = (g2dColor32)(*((g2dColor16*)tex->data + i)); Clr = G2D_COLOR_FROM16(Clr);
				for (iPlt = 0; iPlt < iPltMax; ++iPlt) if (plt->data[iPlt] == Clr) break;
				if (iPlt == iPltMax) plt->data[iPltMax - count--] = Clr;}
			break;
		case G2D_PSM_T8: // Indexed, 8-bit
			pltData = (tex->plt && tex->plt->type == G2D_PALETTE_T8 ? tex->plt->data : G2D_PLT_NULL);
			for (i = 0; (i < iMax) && count; ++i){
				Clr = (g2dColor32)(*((g2dColor8*)tex->data + i)); Clr = G2D_COLOR_FROM8(Clr, pltData);
				for (iPlt = 0; iPlt < iPltMax; ++iPlt) if (plt->data[iPlt] == Clr) break;
				if (iPlt == iPltMax) plt->data[iPltMax - count--] = Clr;}
			break;
		case G2D_PSM_T4: // Indexed, 4-bit (2 pixels per byte)
			pltData = (tex->plt && tex->plt->type == G2D_PALETTE_T4 ? tex->plt->data : G2D_PLT_NULL);
			for (i = 0; (i < iMax) && count; ++i){
				Clr = (g2dColor32)((*((g2dColor8*)tex->data + (i >> 1)) >> (4*(i&0x1))) & 0xF); Clr = G2D_COLOR_FROM8(Clr, pltData);
				for (iPlt = 0; iPlt < iPltMax; ++iPlt) if (plt->data[iPlt] == Clr) break;
				if (iPlt == iPltMax) plt->data[iPltMax - count--] = Clr;}
			break;
		default:
			g2dPaletteFree(&plt);
			return NULL;
	}

	// Sort the Color palette.
	g2dPaletteSort(plt);

	// Return the palette.
    return plt;
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
g2dColor g2dGetPixel(int x, int y)
{
	if (x < 0 || x >= g2d_draw_buffer.tw || y < 0 || y >= g2d_draw_buffer.sh)
		return 0;
	return G2D_TARGET_GETPIXEL((g2dColor*)g2d_draw_buffer.data, x, y, g2d_draw_buffer.tw);
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
g2dColor g2dTexGetPixel(g2dTexture* tex, int x, int y)
{
	if (tex == NULL)
		return 0;

	if (x < 0 || x >= tex->tw || y < 0 || y >= tex->sh)
		return 0;

	//g2dColor32 Clr = 0x0;
	unsigned int* pltData = NULL;
	switch (tex->psm)
	{
		case G2D_PSM_8888: // Truecolor, 32-bit
			return G2D_TARGET_GETPIXEL((g2dColor32*)tex->data, x, y, tex->tw);
		case G2D_PSM_4444: // Hicolor, 16-bit
			return G2D_COLOR_FROM16(G2D_TARGET_GETPIXEL((g2dColor16*)tex->data, x, y, tex->tw));
		case G2D_PSM_T8: // Indexed, 8-bit
			pltData = (tex->plt && tex->plt->type == G2D_PALETTE_T8 ? tex->plt->data : G2D_PLT_NULL);
			return G2D_COLOR_FROM8(G2D_TARGET_GETPIXEL((g2dColor8*)tex->data, x, y, tex->tw), pltData);
		case G2D_PSM_T4: // Indexed, 4-bit (2 pixels per byte)
			pltData = (tex->plt && tex->plt->type == G2D_PALETTE_T4 ? tex->plt->data : G2D_PLT_NULL);
			x = x + y * tex->tw;
			return G2D_COLOR_FROM4((*((g2dColor8*)tex->data + (x >> 1)) >> (4*(x&0x1))) & 0xF, pltData);
		default:
			return 0;
	}
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
void g2dSetPixel(int x, int y, g2dColor color, bool blend)
{
	if (x < 0 || x >= g2d_draw_buffer.tw || y < 0 || y >= g2d_draw_buffer.sh)
		return;

	if (blend && (G2D_GET_A(color) < 255))
	{
		g2dColor Ct = G2D_TARGET_GETPIXEL((g2dColor*)g2d_draw_buffer.data, x, y, g2d_draw_buffer.tw);
		g2dColor Ac = G2D_GET_A(Ct);
		g2dColor Af = G2D_GET_A(color);
		Ac = G2D_ALPHA_GETAC(Ac, Af);
		color = G2D_COLOR_BLEND(Ct, color, Af, Ac);
	}
	G2D_TARGET_SETPIXEL((g2dColor*)g2d_draw_buffer.data, x, y, g2d_draw_buffer.tw, color);
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
void g2dTexSetPixel(g2dTexture* tex, int x, int y, g2dColor color, bool blend)
{
	if (tex == NULL)
		return;

	if (x < 0 || x >= tex->tw || y < 0 || y >= tex->sh)
		return;

	if (blend && (G2D_GET_A(color) < 255))
	{
		g2dColor Ct = g2dTexGetPixel(tex, x, y);
		g2dColor Ac = G2D_GET_A(Ct);
		g2dColor Af = G2D_GET_A(color);
		Ac = G2D_ALPHA_GETAC(Ac, Af);
		color = G2D_COLOR_BLEND(Ct, color, Af, Ac);
	}

	unsigned int* pltData = NULL;
	switch (tex->psm)
	{
		case G2D_PSM_8888: // Truecolor, 32-bit
			G2D_TARGET_SETPIXEL((g2dColor32*)tex->data, x, y, tex->tw, color);
			break;
		case G2D_PSM_4444: // Hicolor, 16-bit
			G2D_TARGET_SETPIXEL((g2dColor16*)tex->data, x, y, tex->tw, G2D_COLOR_TO16(color));
			break;
		case G2D_PSM_T8: // Indexed, 8-bit
			pltData = (tex->plt && tex->plt->type == G2D_PALETTE_T8 ? tex->plt->data : G2D_PLT_NULL);
			// Convert color with palette.
			if (pltData)
			{
				g2dColor32 i,c,t,b;
				for (i = 0, c = 0x0, b = 0xffffffff; (i < 256) && b; ++i)
					{t = G2D_COLOR_DELTA(pltData[i], color); if (b >= t) {c = i; b = t;}}
				color = G2D_COLOR_FROM8(c, G2D_PLT_NULL);
			}
			G2D_TARGET_SETPIXEL((g2dColor8*)tex->data, x, y, tex->tw, G2D_COLOR_TO8(color));
			break;
		case G2D_PSM_T4: // Indexed, 4-bit (2 pixels per byte)
			pltData = (tex->plt && tex->plt->type == G2D_PALETTE_T4 ? tex->plt->data : G2D_PLT_NULL);
			// Convert color with palette.
			if (pltData)
			{
				g2dColor32 i,c,t,b;
				for (i = 0, c = 0x0, b = 0xffffffff; (i < 16) && b; ++i)
					{t = G2D_COLOR_DELTA(pltData[i], color); if (b >= t) {c = i; b = t;}}
				color = G2D_COLOR_FROM4(c, G2D_PLT_NULL);
			}
			x = x + y * tex->tw;
			if (x & 0x1)
				*((g2dColor8*)tex->data + (x >> 1)) = (G2D_COLOR_TO4(color) << 4) | (*((g2dColor8*)tex->data + (x >> 1)) & 0x0F);
			else
				*((g2dColor8*)tex->data + (x >> 1)) = (*((g2dColor8*)tex->data + (x >> 1)) & 0xF0) | G2D_COLOR_TO4(color);
			break;
	}
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// returns 0, if nothing to blit
int _g2dAdjustBlitRect(int csw, int csh, int cdw, int cdh, int *psx, int *psy, int *pw, int *ph, int *pdx, int *pdy)
{
	if (*pw <= 0 || *ph <= 0) return 0;  // zero area, nothing to blit
	if (*psx < 0 || *psy < 0) return 0;  // illegal, source is not clipped
	if (*pdx < 0) {*pw += *pdx; if (*pw <= 0) return 0; *psx -= *pdx; *pdx = 0; if (*psx >= cdw) return 0;}
	if (*pdy < 0) {*ph += *pdy; if (*ph <= 0) return 0; *psy -= *pdy; *pdy = 0; if (*psy >= cdh) return 0;}
	if (*pdx + *pw > cdw) {*pw = cdw - *pdx; if (*pw <= 0) return 0;}
	if (*pdy + *ph > cdh) {*ph = cdw - *pdy; if (*ph <= 0) return 0;}
	return 1;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Blit the texture to a texture.
void g2dTexBlitToTex(g2dTexture* target, int dx, int dy, g2dTexture* tex, int sx, int sy, int sw, int sh, bool blend)
{
	if (target == NULL || tex == NULL)
		return;

    if (_g2dAdjustBlitRect(tex->tw, tex->sh, target->tw, target->sh, &sx, &sy, &sw, &sh, &dx, &dy))
	{
		g2dColor Ct = 0;
		int x0,y0;
		for (y0 = 0 ; y0 < sh; ++y0)
			for (x0 = 0 ; x0 < sw; ++x0)
			{
				G2D_TEX_GETPIXEL(tex, x0+sx, y0+sy, &Ct, tex->plt)
				G2D_TEX_SETPIXEL(target, x0+dx, y0+dy, Ct, blend, target->plt)
			}
	}
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
void g2dTexDrawLine(g2dTexture* target, int x0, int y0, int x1, int y1, g2dColor color, bool blend)
{
	if (target == NULL)
		return;

	int dy = y1 - y0;
	int dx = x1 - x0;
	int stepx, stepy;

	if (dy < 0) {dy = -dy; stepy = -1;} else stepy = 1;
	if (dx < 0) {dx = -dx; stepx = -1;} else stepx = 1;
	dy <<= 1;
	dx <<= 1;

	G2D_TEX_SETPIXEL(target, x0, y0, color, blend, target->plt)
	if (dx > dy) {
		int fraction = dy - (dx >> 1);
		while (x0 != x1) {
			if (fraction >= 0) {y0 += stepy; fraction -= dx;}
			x0 += stepx;
			fraction += dy;
			G2D_TEX_SETPIXEL(target, x0, y0, color, blend, target->plt)
		}
	} else {
		int fraction = dx - (dy >> 1);
		while (y0 != y1) {
			if (fraction >= 0) { x0 += stepx; fraction -= dy;}
			y0 += stepy;
			fraction += dx;
			G2D_TEX_SETPIXEL(target, x0, y0, color, blend, target->plt)
		}
	}
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Draw Rectangle.
void g2dTexDrawRect(g2dTexture* target, int x, int y, int w, int h, g2dColor color, bool blend)
{
	if (target == NULL)
		return;
	if (w < 0) {x += w; w = -w;}
	if (h < 0) {y += h; h = -h;}
	w += x-1;
	h += y-1;
	x = G2D_CLAMP(x, -1, target->tw);
	y = G2D_CLAMP(y, -1, target->sh);
	w = G2D_CLAMP(w, -1, target->tw);
	h = G2D_CLAMP(h, -1, target->sh);

	if (w - x < 2 || h - y < 2)
		return;

	g2dTexDrawLine(target, x, y, w, y, color, blend);
	g2dTexDrawLine(target, x, y+1, x, h-1, color, blend);
	g2dTexDrawLine(target, w, y+1, w, h-1, color, blend);
	g2dTexDrawLine(target, x, h, w, h, color, blend);
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Fill Rectangle.
void g2dTexFillRect(g2dTexture* target, int x, int y, int w, int h, g2dColor color, bool blend)
{
	if (target == NULL)
		return;
	if (w < 0) {x += w; w = -w;}
	if (h < 0) {y += h; h = -h;}
	w += x;
	h += y;
	x = G2D_CLAMP(x, 0, target->tw);
	y = G2D_CLAMP(y, 0, target->sh);
	w = G2D_CLAMP(w, 0, target->tw);
	h = G2D_CLAMP(h, 0, target->sh);

	if (w - x < 1 || h - y < 1)
		return;

	int x0,y0;
	for (y0 = y ; y0 < h; ++y0)
		for (x0 = x ; x0 < w; ++x0)
			G2D_TEX_SETPIXEL(target, x0, y0, color, blend, target->plt)
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Clear the texture.
void g2dTexClear(g2dTexture* target, g2dColor color, bool blend)
{
	g2dTexFillRect(target, 0, 0, 512, 512, color, blend);
}
// -----------------------------------------------------------------------------------------------------
