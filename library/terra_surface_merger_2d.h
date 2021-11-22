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

#ifndef TERRA_SURFACE_MERGER_2D_H
#define TERRA_SURFACE_MERGER_2D_H

#include "terra_surface_2d.h"

#include "scene/resources/texture.h"

#include "../../texture_packer/texture_packer.h"

#include "../defines.h"

#if GODOT4
#define Texture Texture2D
#endif

class TerraSurfaceMerger2D : public TerraSurface2D {
	GDCLASS(TerraSurfaceMerger2D, TerraSurface2D)

public:
	Ref<AtlasTexture> get_region(const TerraSurfaceSides2D side);
	void set_region(const TerraSurfaceSides2D side, const Ref<AtlasTexture> &texture);

	Ref<Texture> get_texture(const TerraSurfaceSides2D side);
	void set_texture(const TerraSurfaceSides2D side, const Ref<Texture> &texture);

	void refresh_rects();

	TerraSurfaceMerger2D();
	~TerraSurfaceMerger2D();

protected:
	static void _bind_methods();

private:
	Ref<AtlasTexture> _regions[TERRA_SIDES_COUNT];
	Ref<Texture> _textures[TERRA_SIDES_COUNT];
};

#endif