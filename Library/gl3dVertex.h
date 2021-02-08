// gl3dVertex - Vertex support library for Lua_GL3D. ( FileName: gl3dVertex.h )
// ------------------------------------------------------------------------
// Version: 1.01
// Copyright (c) 2014 M4MSOFT
// ------------------------------------------------------------------------

#ifndef GL3DVERTEX_H_
#define GL3DVERTEX_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    int prim;                 /**< What kind of primitives to render. */
    int vtype;                /**< Vertex type to process. */
	int count;                /**< How many vertices in the vertices list. */
	unsigned int size;       /**< The Size of the data. */
    void *data;               /**< Pointer to raw data. */
	int count_index;          /**< How many indices in the index list. */
	unsigned int size_index; /**< The Size of the _index data. */
    void *data_index;         /**< Pointer to raw _index data. */
	int n, len;               /**< the start and process lenght of the indices list. */
} gumVertices;

// -----------------------------------------------------------------------------------------------------
// Creates a new blank vertices list.
//
// @param: prim - What kind of primitives vertex to store.
// @param: vtype - Vertex type to process.
// @param: quads - size of one vertex.
// @param: count - How many vertices in the vertices list.
// @param: count_index - How many indeices in the index list.
//
// @return: This function returns NULL on allocation fail.
//
gumVertices* gl3dVerticesCreate(int prim, int vtype, int quads, int count, int count_index);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Frees a vertices & set its pointer to NULL.
//
// @param: vert - Pointer to the variable which contains the vertices pointer.
//
// @return: void
//   This function is used to gain memory when a vertices is useless.
//   Must pass the pointer to the variable which contains the pointer,
//   to set it to NULL.
//
void gl3dVerticesFree(gumVertices **vert);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
void gl3dVerticesEnable (gumVertices *vert, int prim, int vtype);
// -----------------------------------------------------------------------------------------------------
void gl3dVerticesDisable (gumVertices *vert, int prim, int vtype);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Load a vertices list from a file.
//
// @param path - The name of the file.
//
// @return: This function returns NULL on allocation fail.
//
gumVertices* gl3dVerticesLoad (const char *path);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// save a vertices list to a file.
//
// @param path - The name of the file.
// @param vert - Pointer to a vertices list to save
//
// @return: int - 0 on succes.
//
int gl3dVerticesSave (const char *path, gumVertices *vert);
// -----------------------------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif  // GL3DVERTEX_H_
