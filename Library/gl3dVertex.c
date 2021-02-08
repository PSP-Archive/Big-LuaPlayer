// gl3dVertex - Vertex support library for Lua_GL3D. ( FileName: gl3dVertex.c )
// ------------------------------------------------------------------------
// Version: 1.01
// Copyright (c) 2014 M4MSOFT
// ------------------------------------------------------------------------

#include "gl3dVertex.h"

#include <pspkernel.h>
#include <pspgu.h>

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// -----------------------------------------------------------------------------------------------------
// Creates a new blank vertices list.
gumVertices* gl3dVerticesCreate(int prim, int vtype, int quads, int count, int count_index)
{
	// Check the primitives type.
	if (prim < GU_POINTS || prim > GU_SPRITES)
		return NULL;

	// Check the vertex type.
	if ((vtype & GU_TEXTURE_32BITF) != GU_TEXTURE_32BITF && (vtype & GU_COLOR_8888) != GU_COLOR_8888 &&
	(vtype & GU_NORMAL_32BITF) != GU_NORMAL_32BITF && (vtype & GU_VERTEX_32BITF) != GU_VERTEX_32BITF)
		return NULL;

	// Creates the object.
	gumVertices *vert = malloc(sizeof(gumVertices));
    if (vert == NULL)
        return NULL;

	// Set the object values.
	vert->prim = prim;
	vert->vtype = 0;
	if (vtype & GU_TEXTURE_32BITF) vert->vtype |= GU_TEXTURE_32BITF;
	if (vtype & GU_COLOR_8888)     vert->vtype |= GU_COLOR_8888;
	if (vtype & GU_NORMAL_32BITF)  vert->vtype |= GU_NORMAL_32BITF;
	if (vtype & GU_VERTEX_32BITF)  vert->vtype |= GU_VERTEX_32BITF;
	if (vtype & GU_TRANSFORM_2D)   vert->vtype |= GU_TRANSFORM_2D;
	else                           vert->vtype |= GU_TRANSFORM_3D;
	vert->count = count;
	vert->size = count * quads*4;
	vert->data = memalign(16, vert->size);
	if (vert->data == NULL)
	{
		free(vert);
		return NULL;
	}
	memset(vert->data, 0, vert->size);

	vert->count_index = count_index;
	vert->size_index = count_index * sizeof(unsigned short);
	if (vert->size_index > 0)
	{
		vert->data_index = memalign(16, vert->size_index);
		if (vert->data_index == NULL)
		{
			free(vert->data);
			free(vert);
			return NULL;
		}
		memset(vert->data_index, 0, vert->size_index);
	}
	else
		vert->data_index = NULL;
	vert->n = 0;
	vert->len = count_index;

	return vert;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Frees a vertices & set its pointer to NULL.
void gl3dVerticesFree(gumVertices **vert)
{
	// Check the pointer.
	if (vert == NULL)
        return;
    if (*vert == NULL)
        return;

	// Free the indices data.
	if ((*vert)->size_index > 0)
		free((*vert)->data_index);

	// Free the vertices data.
	free((*vert)->data);

	// Free the vertices structure,
	free((*vert));
	(*vert) = NULL;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
void gl3dVerticesEnable (gumVertices *vert, int prim, int vtype)
{
	if (vert == NULL) return;
	(vert->prim) |= prim;
	(vert->vtype) |= vtype;
}
// -----------------------------------------------------------------------------------------------------
void gl3dVerticesDisable (gumVertices *vert, int prim, int vtype)
{
	if (vert == NULL) return;
	(vert->prim) &= ~prim;
	(vert->vtype) &= ~vtype;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static gumVertices* _gl3dVerticesLoad (FILE *fp)
{
	long int size = 0;
	char temp[5] = {'\0','\0','\0','\0','\0'};

	// Get the file size.
	fseek(fp, 0, SEEK_END);
	fgetpos(fp, &size);
	rewind(fp);

	// Check the file header.
	fread(temp, sizeof(char), 4, fp);
	if (strcmp(temp,"VRT%"))
		return NULL;

	// Read the file data.
	int prim = 0;
	int vtype = 0;
	int count = 0;
	unsigned int dataSize = 0;
	int count_index = 0;
	unsigned int dataSize_index = 0;

	fread(&prim, sizeof(int), 1, fp);
	fread(&vtype, sizeof(int), 1, fp);
	fread(&count, sizeof(int), 1, fp);
	fread(&dataSize, sizeof(unsigned int), 1, fp);
	fread(&count_index, sizeof(int), 1, fp);
	fread(&dataSize_index, sizeof(unsigned int), 1, fp);

	// Check the file size.
	if (dataSize*sizeof(unsigned char)+dataSize_index*sizeof(unsigned char)+28 != size)
		return NULL;

	// Creates the object.
	gumVertices *vert = malloc(sizeof(gumVertices));
    if (vert == NULL)
        return NULL;

	// Set the object values.
	vert->prim = prim;
	vert->vtype = vtype;
	vert->count = count;
	vert->size = dataSize;
	vert->data = memalign(16, vert->size);
	if (vert->data == NULL)
	{
		free(vert);
		return NULL;
	}
	memset(vert->data, 0, vert->size);

	vert->count_index = count_index;
	vert->size_index = dataSize_index;
	if (vert->size_index > 0)
	{
		vert->data_index = memalign(16, vert->size_index);
		if (vert->data_index == NULL)
		{
			free(vert->data);
			free(vert);
			return NULL;
		}
		memset(vert->data_index, 0, vert->size_index);
	}
	else
		vert->data_index = NULL;
	vert->n = 0;
	vert->len = count_index;

	// Read the file data.
	fread(vert->data, sizeof(unsigned char), vert->size, fp);
	if (vert->size_index > 0)
		fread(vert->data_index, sizeof(unsigned char), vert->size_index, fp);

	return vert;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
gumVertices* gl3dVerticesLoad (const char *path)
{
	gumVertices *vert = NULL;
	FILE *fp = NULL;

	if (path == NULL)
        return NULL;
	if ((fp = fopen(path, "rb")) == NULL)
        return NULL;

	char* suffix = strrchr(path, '.');

	if (stricmp(suffix, ".vrt") == 0)
        vert = _gl3dVerticesLoad(fp);

	if (vert == NULL)
        goto label_error;

	fclose(fp);
    fp = NULL;

	return vert;

	// Load failure... abort
label_error:
    if (fp != NULL)
        fclose(fp);

    gl3dVerticesFree(&vert);

    return NULL;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
int gl3dVerticesSave (const char *path, gumVertices* vert)
{
	if (path == NULL)
        return -1;

	if (vert == NULL)
        return -2;

	FILE * fp = fopen(path, "wb");
	if (fp == NULL)
		return -3;

	char temp[5] = {'V','R','T','%','\0'};

	fwrite(temp, sizeof(char), 4, fp);
	fwrite(&(vert->prim), sizeof(int), 1, fp);
	fwrite(&(vert->vtype), sizeof(int), 1, fp);
	fwrite(&(vert->count), sizeof(int), 1, fp);
	fwrite(&(vert->size), sizeof(unsigned int), 1, fp);
	fwrite(&(vert->count_index), sizeof(int), 1, fp);
	fwrite(&(vert->size_index), sizeof(unsigned int), 1, fp);
	fwrite(vert->data, sizeof(unsigned char), vert->size, fp);
	if (vert->size_index > 0)
		fwrite(vert->data_index, sizeof(unsigned char), vert->size_index, fp);
	fclose(fp);

	return 0;
}
// -----------------------------------------------------------------------------------------------------
