// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------
#pragma once
#include <glm/glm.hpp>

class VertexArray
{
public:
	VertexArray(void * verts, int numVerts,
		int * indices, int numIndices, int rowSize);
	~VertexArray();

	// Activate this vertex array (so we can draw it)
	void SetActive();

	unsigned int GetNumIndices() const { return mNumIndices; }
	unsigned int GetNumVerts() const { return mNumVerts; }
private:
	// How many vertices in the vertex buffer?
	unsigned int mNumVerts;
	// How many indices in the index buffer
	unsigned int mNumIndices;
	// OpenGL ID of the vertex buffer
	unsigned int mVertexBuffer;
	// OpenGL ID of the index buffer
	unsigned int mIndexBuffer;
	// OpenGL ID of the vertex array object
	unsigned int mVertexArray;
};