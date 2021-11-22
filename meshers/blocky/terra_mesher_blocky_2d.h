/*
Copyright (c) 2019-2021 Péter Magyar

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

#ifndef TERRA_MESHER_BLOCKY_2D_H
#define TERRA_MESHER_BLOCKY_2D_H

#include "core/version.h"

#if VERSION_MAJOR > 3
#include "core/math/color.h"
#else
#include "core/color.h"
#endif

#include "core/math/vector2.h"
#include "core/math/vector3.h"

#include "../default/terra_mesher_default_2d.h"
#include "../../world/default/terra_chunk_default_2d.h"

class TerraMesherBlocky2D : public TerraMesherDefault2D {
	GDCLASS(TerraMesherBlocky2D, TerraMesherDefault2D);

public:
	bool get_always_add_colors() const;
	void set_always_add_colors(const bool value);

	void _add_chunk(Ref<TerraChunk2D> p_chunk);

	void add_chunk_normal(Ref<TerraChunkDefault2D> chunk);

	void add_chunk_lod(Ref<TerraChunkDefault2D> chunk);
	void create_margin_zmin(Ref<TerraChunkDefault2D> chunk);
	void create_margin_zmax(Ref<TerraChunkDefault2D> chunk);
	void create_margin_xmin(Ref<TerraChunkDefault2D> chunk);
	void create_margin_xmax(Ref<TerraChunkDefault2D> chunk);
	void create_margin_corners(Ref<TerraChunkDefault2D> chunk);
	void create_face(Ref<TerraChunkDefault2D> chunk, int dataxmin, int dataxmax, int datazmin, int datazmax);

	TerraMesherBlocky2D();
	~TerraMesherBlocky2D();

protected:
	static void _bind_methods();

private:
	bool _always_add_colors;
};

#endif
