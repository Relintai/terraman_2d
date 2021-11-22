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

#include "terraman_library_2d.h"

#include "../world/terra_chunk_2d.h"

#include "terra_material_cache_2d.h"

#ifdef PROPS_PRESENT
#include "../../props/props/prop_data.h"
#endif

#include "scene/resources/packed_scene.h"

#include "../defines.h"

bool TerramanLibrary2D::get_initialized() const {
	return _initialized;
}
void TerramanLibrary2D::set_initialized(const bool value) {
	_initialized = value;
}

bool TerramanLibrary2D::supports_caching() {
	return call("_supports_caching");
}
bool TerramanLibrary2D::_supports_caching() {
	return false;
}

//Materials
Ref<Material> TerramanLibrary2D::material_get(const int index) {
	ERR_FAIL_INDEX_V(index, _materials.size(), Ref<Material>(NULL));

	return _materials[index];
}

Ref<Material> TerramanLibrary2D::material_lod_get(const int index) {
	ERR_FAIL_COND_V(_materials.size() == 0, Ref<Material>(NULL));

	if (index < 0) {
		return _materials[0];
	}

	if (index >= _materials.size()) {
		return _materials[_materials.size() - 1];
	}

	return _materials[index];
}

void TerramanLibrary2D::material_cache_get_key(Ref<TerraChunk2D> chunk) {
	call("_material_cache_get_key", chunk);
}

void TerramanLibrary2D::_material_cache_get_key(Ref<TerraChunk2D> chunk) {
}

Ref<TerraMaterialCache2D> TerramanLibrary2D::material_cache_get(const int key) {
	return call("_material_cache_get", key);
}

Ref<TerraMaterialCache2D> TerramanLibrary2D::_material_cache_get(const int key) {
	ERR_FAIL_V_MSG(Ref<TerraMaterialCache2D>(), "This TerramanLibrary2D doesn't support cached materials!");
}

void TerramanLibrary2D::material_cache_unref(const int key) {
	call("_material_cache_unref", key);
}
void TerramanLibrary2D::_material_cache_unref(const int key) {
}

void TerramanLibrary2D::material_add(const Ref<Material> &value) {
	ERR_FAIL_COND(!value.is_valid());

	_materials.push_back(value);
}

void TerramanLibrary2D::material_set(const int index, const Ref<Material> &value) {
	ERR_FAIL_INDEX(index, _materials.size());

	_materials.set(index, value);
}

void TerramanLibrary2D::material_remove(const int index) {
	_materials.remove(index);
}

int TerramanLibrary2D::material_get_num() const {
	return _materials.size();
}

void TerramanLibrary2D::materials_clear() {
	_materials.clear();
}

Vector<Variant> TerramanLibrary2D::materials_get() {
	VARIANT_ARRAY_GET(_materials);
}

void TerramanLibrary2D::materials_set(const Vector<Variant> &materials) {
	_materials.clear();

	for (int i = 0; i < materials.size(); i++) {
		Ref<Material> material = Ref<Material>(materials[i]);

		_materials.push_back(material);
	}
}

//Liquid Materials
Ref<Material> TerramanLibrary2D::liquid_material_get(const int index) {
	ERR_FAIL_INDEX_V(index, _liquid_materials.size(), Ref<Material>(NULL));

	return _liquid_materials[index];
}

Ref<Material> TerramanLibrary2D::liquid_material_lod_get(const int index) {
	ERR_FAIL_COND_V(_materials.size() == 0, Ref<Material>(NULL));

	if (index < 0) {
		return _liquid_materials[0];
	}

	if (index >= _liquid_materials.size()) {
		return _liquid_materials[_liquid_materials.size() - 1];
	}

	return _liquid_materials[index];
}

void TerramanLibrary2D::liquid_material_cache_get_key(Ref<TerraChunk2D> chunk) {
	call("_liquid_material_cache_get_key", chunk);
}

void TerramanLibrary2D::_liquid_material_cache_get_key(Ref<TerraChunk2D> chunk) {
}

Ref<TerraMaterialCache2D> TerramanLibrary2D::liquid_material_cache_get(const int key) {
	return call("_liquid_material_cache_get", key);
}

Ref<TerraMaterialCache2D> TerramanLibrary2D::_liquid_material_cache_get(const int key) {
	ERR_FAIL_V_MSG(Ref<TerraMaterialCache2D>(), "This TerramanLibrary2D doesn't support cached liquid materials!");
}

void TerramanLibrary2D::liquid_material_cache_unref(const int key) {
	call("_liquid_material_cache_unref", key);
}
void TerramanLibrary2D::_liquid_material_cache_unref(const int key) {
}

void TerramanLibrary2D::liquid_material_add(const Ref<Material> &value) {
	ERR_FAIL_COND(!value.is_valid());

	_liquid_materials.push_back(value);
}

void TerramanLibrary2D::liquid_material_set(const int index, const Ref<Material> &value) {
	ERR_FAIL_INDEX(index, _liquid_materials.size());

	_liquid_materials.set(index, value);
}

void TerramanLibrary2D::liquid_material_remove(const int index) {
	_liquid_materials.remove(index);
}

int TerramanLibrary2D::liquid_material_get_num() const {
	return _liquid_materials.size();
}

void TerramanLibrary2D::liquid_materials_clear() {
	_liquid_materials.clear();
}

Vector<Variant> TerramanLibrary2D::liquid_materials_get() {
	VARIANT_ARRAY_GET(_liquid_materials);
}

void TerramanLibrary2D::liquid_materials_set(const Vector<Variant> &materials) {
	_liquid_materials.clear();

	for (int i = 0; i < materials.size(); i++) {
		Ref<Material> material = Ref<Material>(materials[i]);

		_liquid_materials.push_back(material);
	}
}

//Prop Materials
Ref<Material> TerramanLibrary2D::prop_material_get(const int index) {
	ERR_FAIL_INDEX_V(index, _prop_materials.size(), Ref<Material>(NULL));

	return _prop_materials[index];
}

Ref<Material> TerramanLibrary2D::prop_material_lod_get(const int index) {
	ERR_FAIL_COND_V(_prop_materials.size() == 0, Ref<Material>(NULL));

	if (index < 0) {
		return _prop_materials[0];
	}

	if (index >= _prop_materials.size()) {
		return _prop_materials[_prop_materials.size() - 1];
	}

	return _prop_materials[index];
}

void TerramanLibrary2D::prop_material_cache_get_key(Ref<TerraChunk2D> chunk) {
	call("_prop_material_cache_get_key", chunk);
}

void TerramanLibrary2D::_prop_material_cache_get_key(Ref<TerraChunk2D> chunk) {
}

Ref<TerraMaterialCache2D> TerramanLibrary2D::prop_material_cache_get(const int key) {
	return call("_prop_material_cache_get", key);
}

Ref<TerraMaterialCache2D> TerramanLibrary2D::_prop_material_cache_get(const int key) {
	ERR_FAIL_V_MSG(Ref<TerraMaterialCache2D>(), "This TerramanLibrary2D doesn't support cached prop materials!");
}

void TerramanLibrary2D::prop_material_cache_unref(const int key) {
	call("_prop_material_cache_unref", key);
}
void TerramanLibrary2D::_prop_material_cache_unref(const int key) {
}

void TerramanLibrary2D::prop_material_add(const Ref<Material> &value) {
	ERR_FAIL_COND(!value.is_valid());

	_prop_materials.push_back(value);
}

void TerramanLibrary2D::prop_material_set(const int index, const Ref<Material> &value) {
	ERR_FAIL_INDEX(index, _prop_materials.size());

	_prop_materials.set(index, value);
}

void TerramanLibrary2D::prop_material_remove(const int index) {
	_prop_materials.remove(index);
}

int TerramanLibrary2D::prop_material_get_num() const {
	return _prop_materials.size();
}

void TerramanLibrary2D::prop_materials_clear() {
	_prop_materials.clear();
}

Vector<Variant> TerramanLibrary2D::prop_materials_get() {
	VARIANT_ARRAY_GET(_prop_materials);
}

void TerramanLibrary2D::prop_materials_set(const Vector<Variant> &materials) {
	VARIANT_ARRAY_SET(materials, _prop_materials, Material);
}

//Surfaces
Ref<TerraSurface2D> TerramanLibrary2D::voxel_surface_get(const int index) {
	return Ref<TerraSurface2D>();
}
void TerramanLibrary2D::voxel_surface_add(Ref<TerraSurface2D> value) {
}
void TerramanLibrary2D::voxel_surface_set(int index, Ref<TerraSurface2D> value) {
}
void TerramanLibrary2D::voxel_surface_remove(const int index) {
}
int TerramanLibrary2D::voxel_surface_get_num() const {
	return 0;
}
void TerramanLibrary2D::voxel_surfaces_clear() {
}

Ref<PackedScene> TerramanLibrary2D::scene_get(const int id) {
	return Ref<PackedScene>();
}
void TerramanLibrary2D::scene_add(Ref<PackedScene> value) {
}
void TerramanLibrary2D::scene_set(int id, Ref<PackedScene> value) {
}
void TerramanLibrary2D::scene_remove(const int id) {
}
int TerramanLibrary2D::scene_get_num() const {
	return 0;
}
void TerramanLibrary2D::scenes_clear() {
}

#ifdef PROPS_PRESENT
Ref<PropData> TerramanLibrary2D::prop_get(const int id) {
	return Ref<PropData>();
}
void TerramanLibrary2D::prop_add(Ref<PropData> value) {
}
bool TerramanLibrary2D::prop_has(const Ref<PropData> &value) const {
	return false;
}
void TerramanLibrary2D::prop_set(int id, Ref<PropData> value) {
}
void TerramanLibrary2D::prop_remove(const int id) {
}
int TerramanLibrary2D::prop_get_num() const {
	return 0;
}
void TerramanLibrary2D::props_clear() {
}

Rect2 TerramanLibrary2D::get_prop_uv_rect(const Ref<Texture> &texture) {
	return Rect2(0, 0, 1, 1);
}
#endif

//Rects
void TerramanLibrary2D::refresh_rects() {
	_initialized = true;
}

void TerramanLibrary2D::setup_material_albedo(int material_index, Ref<Texture> texture) {
	if (has_method("_setup_material_albedo"))
		call("_setup_material_albedo", material_index, texture);
}

TerramanLibrary2D::TerramanLibrary2D() {
	_initialized = false;
}

TerramanLibrary2D::~TerramanLibrary2D() {
	_materials.clear();
	_liquid_materials.clear();
	_prop_materials.clear();
}

void TerramanLibrary2D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_initialized"), &TerramanLibrary2D::get_initialized);
	ClassDB::bind_method(D_METHOD("set_initialized", "value"), &TerramanLibrary2D::set_initialized);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "initialized", PROPERTY_HINT_NONE, "", 0), "set_initialized", "get_initialized");

	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::BOOL, "ret"), "_supports_caching"));
	ClassDB::bind_method(D_METHOD("_supports_caching"), &TerramanLibrary2D::_supports_caching);
	ClassDB::bind_method(D_METHOD("supports_caching"), &TerramanLibrary2D::supports_caching);

	BIND_VMETHOD(MethodInfo("_setup_material_albedo", PropertyInfo(Variant::INT, "material_index"), PropertyInfo(Variant::OBJECT, "texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture")));

	BIND_VMETHOD(MethodInfo("_material_cache_get_key", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "TerraChunk2D")));
	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::OBJECT, "ret", PROPERTY_HINT_RESOURCE_TYPE, "TerraMaterialCache2D"), "_material_cache_get", PropertyInfo(Variant::INT, "key")));
	BIND_VMETHOD(MethodInfo("_material_cache_unref", PropertyInfo(Variant::INT, "key")));

	ClassDB::bind_method(D_METHOD("material_get", "index"), &TerramanLibrary2D::material_get);
	ClassDB::bind_method(D_METHOD("material_lod_get", "index"), &TerramanLibrary2D::material_lod_get);

	ClassDB::bind_method(D_METHOD("material_cache_get_key", "chunk"), &TerramanLibrary2D::material_cache_get_key);
	ClassDB::bind_method(D_METHOD("_material_cache_get_key", "chunk"), &TerramanLibrary2D::_material_cache_get_key);
	ClassDB::bind_method(D_METHOD("material_cache_get", "key"), &TerramanLibrary2D::material_cache_get);
	ClassDB::bind_method(D_METHOD("_material_cache_get", "key"), &TerramanLibrary2D::_material_cache_get);
	ClassDB::bind_method(D_METHOD("material_cache_unref", "key"), &TerramanLibrary2D::material_cache_unref);
	ClassDB::bind_method(D_METHOD("_material_cache_unref", "key"), &TerramanLibrary2D::_material_cache_unref);

	ClassDB::bind_method(D_METHOD("material_add", "value"), &TerramanLibrary2D::material_add);
	ClassDB::bind_method(D_METHOD("material_set", "index", "value"), &TerramanLibrary2D::material_set);
	ClassDB::bind_method(D_METHOD("material_remove", "index"), &TerramanLibrary2D::material_remove);
	ClassDB::bind_method(D_METHOD("material_get_num"), &TerramanLibrary2D::material_get_num);
	ClassDB::bind_method(D_METHOD("materials_clear"), &TerramanLibrary2D::materials_clear);

	ClassDB::bind_method(D_METHOD("materials_get"), &TerramanLibrary2D::materials_get);
	ClassDB::bind_method(D_METHOD("materials_set"), &TerramanLibrary2D::materials_set);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "materials", PROPERTY_HINT_NONE, "17/17:Material", PROPERTY_USAGE_DEFAULT, "Material"), "materials_set", "materials_get");

	BIND_VMETHOD(MethodInfo("_liquid_material_cache_get_key", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "TerraChunk2D")));
	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::OBJECT, "ret", PROPERTY_HINT_RESOURCE_TYPE, "TerraMaterialCache2D"), "_liquid_material_cache_get", PropertyInfo(Variant::INT, "key")));
	BIND_VMETHOD(MethodInfo("_liquid_material_cache_unref", PropertyInfo(Variant::INT, "key")));

	ClassDB::bind_method(D_METHOD("liquid_material_get", "index"), &TerramanLibrary2D::liquid_material_get);
	ClassDB::bind_method(D_METHOD("liquid_material_lod_get", "index"), &TerramanLibrary2D::liquid_material_lod_get);

	ClassDB::bind_method(D_METHOD("liquid_material_cache_get_key", "chunk"), &TerramanLibrary2D::liquid_material_cache_get_key);
	ClassDB::bind_method(D_METHOD("_liquid_material_cache_get_key", "chunk"), &TerramanLibrary2D::_liquid_material_cache_get_key);
	ClassDB::bind_method(D_METHOD("liquid_material_cache_get", "key"), &TerramanLibrary2D::liquid_material_cache_get);
	ClassDB::bind_method(D_METHOD("_liquid_material_cache_get", "key"), &TerramanLibrary2D::_liquid_material_cache_get);
	ClassDB::bind_method(D_METHOD("liquid_material_cache_unref", "key"), &TerramanLibrary2D::liquid_material_cache_unref);
	ClassDB::bind_method(D_METHOD("_liquid_material_cache_unref", "key"), &TerramanLibrary2D::_liquid_material_cache_unref);

	ClassDB::bind_method(D_METHOD("liquid_material_add", "value"), &TerramanLibrary2D::liquid_material_add);
	ClassDB::bind_method(D_METHOD("liquid_material_set", "index", "value"), &TerramanLibrary2D::liquid_material_set);
	ClassDB::bind_method(D_METHOD("liquid_material_remove", "index"), &TerramanLibrary2D::liquid_material_remove);
	ClassDB::bind_method(D_METHOD("liquid_material_get_num"), &TerramanLibrary2D::liquid_material_get_num);
	ClassDB::bind_method(D_METHOD("liquid_materials_clear"), &TerramanLibrary2D::liquid_materials_clear);

	ClassDB::bind_method(D_METHOD("liquid_materials_get"), &TerramanLibrary2D::liquid_materials_get);
	ClassDB::bind_method(D_METHOD("liquid_materials_set"), &TerramanLibrary2D::liquid_materials_set);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "liquid_materials", PROPERTY_HINT_NONE, "17/17:Material", PROPERTY_USAGE_DEFAULT, "Material"), "liquid_materials_set", "liquid_materials_get");

	BIND_VMETHOD(MethodInfo("_prop_material_cache_get_key", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "TerraChunk2D")));
	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::OBJECT, "ret", PROPERTY_HINT_RESOURCE_TYPE, "TerraMaterialCache2D"), "_prop_material_cache_get", PropertyInfo(Variant::INT, "key")));
	BIND_VMETHOD(MethodInfo("_prop_material_cache_unref", PropertyInfo(Variant::INT, "key")));

	ClassDB::bind_method(D_METHOD("prop_material_get", "index"), &TerramanLibrary2D::prop_material_get);
	ClassDB::bind_method(D_METHOD("prop_material_lod_get", "index"), &TerramanLibrary2D::prop_material_lod_get);

	ClassDB::bind_method(D_METHOD("prop_material_cache_get_key", "chunk"), &TerramanLibrary2D::prop_material_cache_get_key);
	ClassDB::bind_method(D_METHOD("_prop_material_cache_get_key", "chunk"), &TerramanLibrary2D::_prop_material_cache_get_key);
	ClassDB::bind_method(D_METHOD("prop_material_cache_get", "key"), &TerramanLibrary2D::prop_material_cache_get);
	ClassDB::bind_method(D_METHOD("_prop_material_cache_get", "key"), &TerramanLibrary2D::_prop_material_cache_get);
	ClassDB::bind_method(D_METHOD("prop_material_cache_unref", "key"), &TerramanLibrary2D::prop_material_cache_unref);
	ClassDB::bind_method(D_METHOD("_prop_material_cache_unref", "key"), &TerramanLibrary2D::_prop_material_cache_unref);

	ClassDB::bind_method(D_METHOD("prop_material_add", "value"), &TerramanLibrary2D::prop_material_add);
	ClassDB::bind_method(D_METHOD("prop_material_set", "index", "value"), &TerramanLibrary2D::prop_material_set);
	ClassDB::bind_method(D_METHOD("prop_material_remove", "index"), &TerramanLibrary2D::prop_material_remove);
	ClassDB::bind_method(D_METHOD("prop_material_get_num"), &TerramanLibrary2D::prop_material_get_num);
	ClassDB::bind_method(D_METHOD("prop_materials_clear"), &TerramanLibrary2D::prop_materials_clear);

	ClassDB::bind_method(D_METHOD("prop_materials_get"), &TerramanLibrary2D::prop_materials_get);
	ClassDB::bind_method(D_METHOD("prop_materials_set"), &TerramanLibrary2D::prop_materials_set);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "prop_materials", PROPERTY_HINT_NONE, "17/17:Material", PROPERTY_USAGE_DEFAULT, "Material"), "prop_materials_set", "prop_materials_get");

	ClassDB::bind_method(D_METHOD("voxel_surface_get", "index"), &TerramanLibrary2D::voxel_surface_get);
	ClassDB::bind_method(D_METHOD("voxel_surface_add", "value"), &TerramanLibrary2D::voxel_surface_add);
	ClassDB::bind_method(D_METHOD("voxel_surface_set", "index", "surface"), &TerramanLibrary2D::voxel_surface_set);
	ClassDB::bind_method(D_METHOD("voxel_surface_remove", "index"), &TerramanLibrary2D::voxel_surface_remove);
	ClassDB::bind_method(D_METHOD("voxel_surface_get_num"), &TerramanLibrary2D::voxel_surface_get_num);
	ClassDB::bind_method(D_METHOD("voxel_surfaces_clear"), &TerramanLibrary2D::voxel_surfaces_clear);

	ClassDB::bind_method(D_METHOD("scene_get", "index"), &TerramanLibrary2D::scene_get);
	ClassDB::bind_method(D_METHOD("scene_add", "value"), &TerramanLibrary2D::scene_add);
	ClassDB::bind_method(D_METHOD("scene_set", "index", "value"), &TerramanLibrary2D::scene_set);
	ClassDB::bind_method(D_METHOD("scene_remove", "index"), &TerramanLibrary2D::scene_remove);
	ClassDB::bind_method(D_METHOD("scene_get_num"), &TerramanLibrary2D::scene_get_num);
	ClassDB::bind_method(D_METHOD("scenes_clear"), &TerramanLibrary2D::scenes_clear);

#ifdef PROPS_PRESENT
	ClassDB::bind_method(D_METHOD("prop_get", "id"), &TerramanLibrary2D::prop_get);
	ClassDB::bind_method(D_METHOD("prop_add", "value"), &TerramanLibrary2D::prop_add);
	ClassDB::bind_method(D_METHOD("prop_has", "prop"), &TerramanLibrary2D::prop_has);
	ClassDB::bind_method(D_METHOD("prop_set", "id", "surface"), &TerramanLibrary2D::prop_set);
	ClassDB::bind_method(D_METHOD("prop_remove", "id"), &TerramanLibrary2D::prop_remove);
	ClassDB::bind_method(D_METHOD("prop_get_num"), &TerramanLibrary2D::prop_get_num);
	ClassDB::bind_method(D_METHOD("props_clear"), &TerramanLibrary2D::props_clear);
#endif

	ClassDB::bind_method(D_METHOD("refresh_rects"), &TerramanLibrary2D::refresh_rects);

	ClassDB::bind_method(D_METHOD("setup_material_albedo", "material_index", "texture"), &TerramanLibrary2D::setup_material_albedo);

	BIND_CONSTANT(MATERIAL_INDEX_TERRAS);
	BIND_CONSTANT(MATERIAL_INDEX_LIQUID);
	BIND_CONSTANT(MATERIAL_INDEX_PROP);
}