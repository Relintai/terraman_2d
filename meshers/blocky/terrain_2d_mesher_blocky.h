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

#ifndef TERRAIN_MESHER_BLOCKY_H
#define TERRAIN_MESHER_BLOCKY_H

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

class Terrain2DMesherBlocky : public Terrain2DMesherDefault {
	GDCLASS(Terrain2DMesherBlocky, Terrain2DMesherDefault);

public:
	bool get_always_add_colors() const;
	void set_always_add_colors(const bool value);

	void _add_chunk(Ref<Terrain2DChunk> p_chunk);

	void add_chunk_normal(Ref<Terrain2DChunkDefault> chunk);

	void add_chunk_lod(Ref<Terrain2DChunkDefault> chunk);
	void create_margin_zmin(Ref<Terrain2DChunkDefault> chunk);
	void create_margin_zmax(Ref<Terrain2DChunkDefault> chunk);
	void create_margin_xmin(Ref<Terrain2DChunkDefault> chunk);
	void create_margin_xmax(Ref<Terrain2DChunkDefault> chunk);
	void create_margin_corners(Ref<Terrain2DChunkDefault> chunk);
	void create_face(Ref<Terrain2DChunkDefault> chunk, int dataxmin, int dataxmax, int datazmin, int datazmax);

	Terrain2DMesherBlocky();
	~Terrain2DMesherBlocky();

protected:
	static void _bind_methods();

private:
	bool _always_add_colors;
};

#endif