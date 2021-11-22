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

#include "terra_surface_simple_2d.h"

#include "terraman_library_simple_2d.h"

int TerraSurfaceSimple2D::get_atlas_x(const TerraSurfaceSides2D side) const {
	int indx = (side * 2);

	return _atlas_positions[indx];
}
void TerraSurfaceSimple2D::set_atlas_x(const TerraSurfaceSides2D side, int value) {
	int indx = (side * 2);

	_atlas_positions[indx] = value;
}

int TerraSurfaceSimple2D::get_atlas_y(const TerraSurfaceSides2D side) const {
	int indx = (side * 2) + 1;

	return _atlas_positions[indx];
}
void TerraSurfaceSimple2D::set_atlas_y(const TerraSurfaceSides2D side, int value) {
	int indx = (side * 2) + 1;

	_atlas_positions[indx] = value;
}

void TerraSurfaceSimple2D::refresh_rects() {
	TerramanLibrarySimple2D *lib = Object::cast_to<TerramanLibrarySimple2D>(_library);

	ERR_FAIL_COND(lib == NULL);

	for (int i = 0; i < TERRA_SIDES_COUNT; ++i) {
		float culomn = 1.0 / static_cast<float>(lib->get_atlas_columns());
		float row = 1.0 / static_cast<float>(lib->get_atlas_rows());

		Rect2 r;

		r.position.x = _atlas_positions[i * 2] * culomn;
		r.position.y = _atlas_positions[i * 2 + 1] * row;

		r.size.x = culomn;
		r.size.y = row;

		_rects[i] = r;
	}
}

TerraSurfaceSimple2D::TerraSurfaceSimple2D() {
	for (int i = 0; i < TERRA_SIDES_ARRAY_SIZE; ++i) {
		_atlas_positions[i] = 0;
	}
}

TerraSurfaceSimple2D::~TerraSurfaceSimple2D() {
}

void TerraSurfaceSimple2D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_atlas_x", "side"), &TerraSurfaceSimple2D::get_atlas_x);
	ClassDB::bind_method(D_METHOD("set_atlas_x", "side", "value"), &TerraSurfaceSimple2D::set_atlas_x);

	ClassDB::bind_method(D_METHOD("get_atlas_y", "side"), &TerraSurfaceSimple2D::get_atlas_y);
	ClassDB::bind_method(D_METHOD("set_atlas_y", "side", "value"), &TerraSurfaceSimple2D::set_atlas_y);

	ADD_PROPERTYI(PropertyInfo(Variant::INT, "top_atlas_x"), "set_atlas_x", "get_atlas_x", TERRA_SIDE_TOP);
	ADD_PROPERTYI(PropertyInfo(Variant::INT, "top_atlas_y"), "set_atlas_y", "get_atlas_y", TERRA_SIDE_TOP);

	ADD_PROPERTYI(PropertyInfo(Variant::INT, "bottom_atlas_x"), "set_atlas_x", "get_atlas_x", TERRA_SIDE_BOTTOM);
	ADD_PROPERTYI(PropertyInfo(Variant::INT, "bottom_atlas_y"), "set_atlas_y", "get_atlas_y", TERRA_SIDE_BOTTOM);

	ADD_PROPERTYI(PropertyInfo(Variant::INT, "side_atlas_x"), "set_atlas_x", "get_atlas_x", TERRA_SIDE_SIDE);
	ADD_PROPERTYI(PropertyInfo(Variant::INT, "side_atlas_y"), "set_atlas_y", "get_atlas_y", TERRA_SIDE_SIDE);
}
