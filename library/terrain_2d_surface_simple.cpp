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

#include "terrain_2d_surface_simple.h"

#include "terrain_2d_library_simple.h"

int Terrain2DSurfaceSimple::get_atlas_x(const Terrain2DSurfaceSides side) const {
	int indx = (side * 2);

	return _atlas_positions[indx];
}
void Terrain2DSurfaceSimple::set_atlas_x(const Terrain2DSurfaceSides side, int value) {
	int indx = (side * 2);

	_atlas_positions[indx] = value;
}

int Terrain2DSurfaceSimple::get_atlas_y(const Terrain2DSurfaceSides side) const {
	int indx = (side * 2) + 1;

	return _atlas_positions[indx];
}
void Terrain2DSurfaceSimple::set_atlas_y(const Terrain2DSurfaceSides side, int value) {
	int indx = (side * 2) + 1;

	_atlas_positions[indx] = value;
}

void Terrain2DSurfaceSimple::refresh_rects() {
	Terrain2DLibrarySimple *lib = Object::cast_to<Terrain2DLibrarySimple>(_library);

	ERR_FAIL_COND(lib == NULL);

	for (int i = 0; i < TERRAIN_2D_SIDES_COUNT; ++i) {
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

Terrain2DSurfaceSimple::Terrain2DSurfaceSimple() {
	for (int i = 0; i < TERRAIN_2D_SIDES_ARRAY_SIZE; ++i) {
		_atlas_positions[i] = 0;
	}
}

Terrain2DSurfaceSimple::~Terrain2DSurfaceSimple() {
}

void Terrain2DSurfaceSimple::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_atlas_x", "side"), &Terrain2DSurfaceSimple::get_atlas_x);
	ClassDB::bind_method(D_METHOD("set_atlas_x", "side", "value"), &Terrain2DSurfaceSimple::set_atlas_x);

	ClassDB::bind_method(D_METHOD("get_atlas_y", "side"), &Terrain2DSurfaceSimple::get_atlas_y);
	ClassDB::bind_method(D_METHOD("set_atlas_y", "side", "value"), &Terrain2DSurfaceSimple::set_atlas_y);

	ADD_PROPERTYI(PropertyInfo(Variant::INT, "top_atlas_x"), "set_atlas_x", "get_atlas_x", TERRAIN_2D_SIDE_TOP);
	ADD_PROPERTYI(PropertyInfo(Variant::INT, "top_atlas_y"), "set_atlas_y", "get_atlas_y", TERRAIN_2D_SIDE_TOP);

	ADD_PROPERTYI(PropertyInfo(Variant::INT, "bottom_atlas_x"), "set_atlas_x", "get_atlas_x", TERRAIN_2D_SIDE_BOTTOM);
	ADD_PROPERTYI(PropertyInfo(Variant::INT, "bottom_atlas_y"), "set_atlas_y", "get_atlas_y", TERRAIN_2D_SIDE_BOTTOM);

	ADD_PROPERTYI(PropertyInfo(Variant::INT, "side_atlas_x"), "set_atlas_x", "get_atlas_x", TERRAIN_2D_SIDE_SIDE);
	ADD_PROPERTYI(PropertyInfo(Variant::INT, "side_atlas_y"), "set_atlas_y", "get_atlas_y", TERRAIN_2D_SIDE_SIDE);
}
