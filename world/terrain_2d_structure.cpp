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

#include "terrain_2d_structure.h"

bool Terrain2DStructure::get_use_aabb() const {
	return _use_aabb;
}
void Terrain2DStructure::set_use_aabb(const bool value) {
	_use_aabb = value;
}

AABB Terrain2DStructure::get_chunk_aabb() const {
	return _chunk_aabb;
}
void Terrain2DStructure::set_chunk_aabb(const AABB &value) {
	_chunk_aabb = value;
}

int Terrain2DStructure::get_position_x() const {
	return _position_x;
}
void Terrain2DStructure::set_position_x(const int value) {
	_position_x = value;
}

int Terrain2DStructure::get_position_y() const {
	return _position_y;
}
void Terrain2DStructure::set_position_y(const int value) {
	_position_y = value;
}

int Terrain2DStructure::get_position_z() const {
	return _position_z;
}
void Terrain2DStructure::set_position_z(const int value) {
	_position_z = value;
}

void Terrain2DStructure::set_position(const int x, const int y, const int z) {
	_position_x = x;
	_position_y = y;
	_position_z = z;
}

void Terrain2DStructure::write_to_chunk(Ref<Terrain2DChunk> chunk) {
	ERR_FAIL_COND(!chunk.is_valid());

	if (has_method("_write_to_chunk")) {
		CALL(_write_to_chunk, chunk);
	}
}

Terrain2DStructure::Terrain2DStructure() {
	_use_aabb = true;
	_position_x = 0;
	_position_y = 0;
	_position_z = 0;
}

Terrain2DStructure::~Terrain2DStructure() {
}

void Terrain2DStructure::_bind_methods() {
#if VERSION_MAJOR < 4
	BIND_VMETHOD(MethodInfo("_write_to_chunk", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "Terrain2DChunk")));
#else
	GDVIRTUAL_BIND(_write_to_chunk, "chunk");
#endif

	ClassDB::bind_method(D_METHOD("get_use_aabb"), &Terrain2DStructure::get_use_aabb);
	ClassDB::bind_method(D_METHOD("set_use_aabb", "value"), &Terrain2DStructure::set_use_aabb);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_aabb"), "set_use_aabb", "get_use_aabb");

	ClassDB::bind_method(D_METHOD("get_chunk_aabb"), &Terrain2DStructure::get_chunk_aabb);
	ClassDB::bind_method(D_METHOD("set_chunk_aabb", "value"), &Terrain2DStructure::set_chunk_aabb);
	ADD_PROPERTY(PropertyInfo(Variant::AABB, "chunk_aabb"), "set_chunk_aabb", "get_chunk_aabb");

	ClassDB::bind_method(D_METHOD("get_position_x"), &Terrain2DStructure::get_position_x);
	ClassDB::bind_method(D_METHOD("set_position_x", "value"), &Terrain2DStructure::set_position_x);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "position_x"), "set_position_x", "get_position_x");

	ClassDB::bind_method(D_METHOD("get_position_y"), &Terrain2DStructure::get_position_y);
	ClassDB::bind_method(D_METHOD("set_position_y", "value"), &Terrain2DStructure::set_position_y);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "position_y"), "set_position_y", "get_position_y");

	ClassDB::bind_method(D_METHOD("get_position_z"), &Terrain2DStructure::get_position_z);
	ClassDB::bind_method(D_METHOD("set_position_z", "value"), &Terrain2DStructure::set_position_z);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "position_z"), "set_position_z", "get_position_z");

	ClassDB::bind_method(D_METHOD("set_position", "x", "y", "z"), &Terrain2DStructure::set_position);

	ClassDB::bind_method(D_METHOD("write_to_chunk", "chunk"), &Terrain2DStructure::write_to_chunk);
}
