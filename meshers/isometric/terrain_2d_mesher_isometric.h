/*
Copyright (c) 2019-2022 Péter Magyar

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef TERRAIN_2D_MESHER_ISOMETRIC_H
#define TERRAIN_2D_MESHER_ISOMETRIC_H

#include "core/version.h"

#if VERSION_MAJOR > 3
#include "core/math/color.h"
#else
#include "core/color.h"
#endif

#include "core/math/vector2.h"
#include "core/math/vector3.h"

#include "../default/terrain_2d_mesher_default.h"
#include "../../world/default/terrain_2d_chunk_default.h"

class Terrain2DMesherIsometric : public Terrain2DMesherDefault {
	GDCLASS(Terrain2DMesherIsometric, Terrain2DMesherDefault);

public:
	void _add_chunk(Ref<Terrain2DChunk> p_chunk);

	void add_chunk_normal(Ref<Terrain2DChunkDefault> chunk);
	void mesh_base(Ref<Terrain2DChunkDefault> chunk);
	void mesh_walls(Ref<Terrain2DChunkDefault> chunk);

	Terrain2DMesherIsometric();
	~Terrain2DMesherIsometric();

protected:
	static void _bind_methods();
};

#endif
