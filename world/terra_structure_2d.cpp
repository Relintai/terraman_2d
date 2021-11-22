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

#include "terra_structure_2d.h"

bool TerraStructure2D::get_use_aabb() const {
	return _use_aabb;
}
void TerraStructure2D::set_use_aabb(const bool value) {
	_use_aabb = value;
}

AABB TerraStructure2D::get_chunk_aabb() const {
	return _chunk_aabb;
}
void TerraStructure2D::set_chunk_aabb(const AABB &value) {
	_chunk_aabb = value;
}

int TerraStructure2D::get_position_x() const {
	return _position_x;
}
void TerraStructure2D::set_position_x(const int value) {
	_position_x = value;
}

int TerraStructure2D::get_position_y() const {
	return _position_y;
}
void TerraStructure2D::set_position_y(const int value) {
	_position_y = value;
}

int TerraStructure2D::get_position_z() const {
	return _position_z;
}
void TerraStructure2D::set_position_z(const int value) {
	_position_z = value;
}

void TerraStructure2D::set_position(const int x, const int y, const int z) {
	_position_x = x;
	_position_y = y;
	_position_z = z;
}

void TerraStructure2D::write_to_chunk(Ref<TerraChunk2D> chunk) {
	ERR_FAIL_COND(!chunk.is_valid());

	if (has_method("_write_to_chunk"))
		call("_write_to_chunk", chunk);
}

TerraStructure2D::TerraStructure2D() {
	_use_aabb = true;
	_position_x = 0;
	_position_y = 0;
	_position_z = 0;
}

TerraStructure2D::~TerraStructure2D() {
}

void TerraStructure2D::_bind_methods() {
	BIND_VMETHOD(MethodInfo("_write_to_chunk", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "TerraChunk2D")));

	ClassDB::bind_method(D_METHOD("get_use_aabb"), &TerraStructure2D::get_use_aabb);
	ClassDB::bind_method(D_METHOD("set_use_aabb", "value"), &TerraStructure2D::set_use_aabb);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_aabb"), "set_use_aabb", "get_use_aabb");

	ClassDB::bind_method(D_METHOD("get_chunk_aabb"), &TerraStructure2D::get_chunk_aabb);
	ClassDB::bind_method(D_METHOD("set_chunk_aabb", "value"), &TerraStructure2D::set_chunk_aabb);
	ADD_PROPERTY(PropertyInfo(Variant::AABB, "chunk_aabb"), "set_chunk_aabb", "get_chunk_aabb");

	ClassDB::bind_method(D_METHOD("get_position_x"), &TerraStructure2D::get_position_x);
	ClassDB::bind_method(D_METHOD("set_position_x", "value"), &TerraStructure2D::set_position_x);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "position_x"), "set_position_x", "get_position_x");

	ClassDB::bind_method(D_METHOD("get_position_y"), &TerraStructure2D::get_position_y);
	ClassDB::bind_method(D_METHOD("set_position_y", "value"), &TerraStructure2D::set_position_y);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "position_y"), "set_position_y", "get_position_y");

	ClassDB::bind_method(D_METHOD("get_position_z"), &TerraStructure2D::get_position_z);
	ClassDB::bind_method(D_METHOD("set_position_z", "value"), &TerraStructure2D::set_position_z);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "position_z"), "set_position_z", "get_position_z");

	ClassDB::bind_method(D_METHOD("set_position", "x", "y", "z"), &TerraStructure2D::set_position);

	ClassDB::bind_method(D_METHOD("write_to_chunk", "chunk"), &TerraStructure2D::write_to_chunk);
}