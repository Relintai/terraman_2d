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

#include "terrain_2d_library.h"

#include "../world/terrain_2d_chunk.h"

#include "terrain_2d_material_cache.h"

#ifdef PROPS_2D_PRESENT
#include "../../props_2d/props/prop_2d_data.h"
#endif

#include "scene/resources/packed_scene.h"

#include "../defines.h"

#if VERSION_MAJOR > 3

#define TMCGDVIRTUAL1(m_class, m_name, m_type1)                                                                                                                                                                 \
	bool m_class::_gdvirtual_##m_name##_call(m_type1 arg1) {                                                                                                                                                    \
		ScriptInstance *script_instance = ((Object *)(this))->get_script_instance();                                                                                                                            \
		if (script_instance) {                                                                                                                                                                                  \
			Callable::CallError ce;                                                                                                                                                                             \
			Variant vargs[1] = { Variant(arg1) };                                                                                                                                                               \
			const Variant *vargptrs[1] = { &vargs[0] };                                                                                                                                                         \
                                                                                                                                                                                                                \
			script_instance->call(_gdvirtual_##m_name##_sn, (const Variant **)vargptrs, 1, ce);                                                                                                                 \
			if (ce.error == Callable::CallError::CALL_OK) {                                                                                                                                                     \
				return true;                                                                                                                                                                                    \
			}                                                                                                                                                                                                   \
		}                                                                                                                                                                                                       \
		if (unlikely(_get_extension() && !_gdvirtual_##m_name##_initialized)) {                                                                                                                                 \
			_gdvirtual_##m_name = (_get_extension() && _get_extension()->get_virtual) ? _get_extension()->get_virtual(_get_extension()->class_userdata, #m_name) : (GDNativeExtensionClassCallVirtual) nullptr; \
			_gdvirtual_##m_name##_initialized = true;                                                                                                                                                           \
		}                                                                                                                                                                                                       \
		if (_gdvirtual_##m_name) {                                                                                                                                                                              \
			PtrToArg<m_type1>::EncodeT argval1 = arg1;                                                                                                                                                          \
			const GDNativeTypePtr argptrs[1] = { &argval1 };                                                                                                                                                    \
                                                                                                                                                                                                                \
			_gdvirtual_##m_name(_get_extension_instance(), (const GDNativeTypePtr *)argptrs, nullptr);                                                                                                          \
                                                                                                                                                                                                                \
			return true;                                                                                                                                                                                        \
		}                                                                                                                                                                                                       \
                                                                                                                                                                                                                \
		return false;                                                                                                                                                                                           \
	}                                                                                                                                                                                                           \
	bool m_class::_gdvirtual_##m_name##_overridden() const {                                                                                                                                                    \
		ScriptInstance *script_instance = ((Object *)(this))->get_script_instance();                                                                                                                            \
		if (script_instance) {                                                                                                                                                                                  \
			return script_instance->has_method(_gdvirtual_##m_name##_sn);                                                                                                                                       \
		}                                                                                                                                                                                                       \
		if (unlikely(_get_extension() && !_gdvirtual_##m_name##_initialized)) {                                                                                                                                 \
			_gdvirtual_##m_name = (_get_extension() && _get_extension()->get_virtual) ? _get_extension()->get_virtual(_get_extension()->class_userdata, #m_name) : (GDNativeExtensionClassCallVirtual) nullptr; \
			_gdvirtual_##m_name##_initialized = true;                                                                                                                                                           \
		}                                                                                                                                                                                                       \
		if (_gdvirtual_##m_name) {                                                                                                                                                                              \
			return true;                                                                                                                                                                                        \
		}                                                                                                                                                                                                       \
		return false;                                                                                                                                                                                           \
	}                                                                                                                                                                                                           \
                                                                                                                                                                                                                \
	MethodInfo m_class::_gdvirtual_##m_name##_get_method_info() {                                                                                                                                               \
		MethodInfo method_info;                                                                                                                                                                                 \
		method_info.name = #m_name;                                                                                                                                                                             \
		method_info.flags = METHOD_FLAG_VIRTUAL;                                                                                                                                                                \
		method_info.arguments.push_back(GetTypeInfo<m_type1>::get_class_info());                                                                                                                                \
                                                                                                                                                                                                                \
		return method_info;                                                                                                                                                                                     \
	}

#define TMCGDVIRTUAL1R(m_class, m_ret, m_name, m_type1)                                                                                                                                                         \
	bool m_class::_gdvirtual_##m_name##_call(m_type1 arg1, m_ret &r_ret) {                                                                                                                                      \
		ScriptInstance *script_instance = ((Object *)(this))->get_script_instance();                                                                                                                            \
		if (script_instance) {                                                                                                                                                                                  \
			Callable::CallError ce;                                                                                                                                                                             \
			Variant vargs[1] = { Variant(arg1) };                                                                                                                                                               \
			const Variant *vargptrs[1] = { &vargs[0] };                                                                                                                                                         \
                                                                                                                                                                                                                \
			Variant ret = script_instance->call(_gdvirtual_##m_name##_sn, (const Variant **)vargptrs, 1, ce);                                                                                                   \
			if (ce.error == Callable::CallError::CALL_OK) {                                                                                                                                                     \
				r_ret = VariantCaster<m_ret>::cast(ret);                                                                                                                                                        \
				return true;                                                                                                                                                                                    \
			}                                                                                                                                                                                                   \
		}                                                                                                                                                                                                       \
		if (unlikely(_get_extension() && !_gdvirtual_##m_name##_initialized)) {                                                                                                                                 \
			_gdvirtual_##m_name = (_get_extension() && _get_extension()->get_virtual) ? _get_extension()->get_virtual(_get_extension()->class_userdata, #m_name) : (GDNativeExtensionClassCallVirtual) nullptr; \
			_gdvirtual_##m_name##_initialized = true;                                                                                                                                                           \
		}                                                                                                                                                                                                       \
		if (_gdvirtual_##m_name) {                                                                                                                                                                              \
			PtrToArg<m_type1>::EncodeT argval1 = arg1;                                                                                                                                                          \
			const GDNativeTypePtr argptrs[1] = { &argval1 };                                                                                                                                                    \
                                                                                                                                                                                                                \
			PtrToArg<m_ret>::EncodeT ret;                                                                                                                                                                       \
			_gdvirtual_##m_name(_get_extension_instance(), (const GDNativeTypePtr *)argptrs, &ret);                                                                                                             \
			r_ret = (m_ret)ret;                                                                                                                                                                                 \
			return true;                                                                                                                                                                                        \
		}                                                                                                                                                                                                       \
                                                                                                                                                                                                                \
		return false;                                                                                                                                                                                           \
	}                                                                                                                                                                                                           \
	bool m_class::_gdvirtual_##m_name##_overridden() const {                                                                                                                                                    \
		ScriptInstance *script_instance = ((Object *)(this))->get_script_instance();                                                                                                                            \
		if (script_instance) {                                                                                                                                                                                  \
			return script_instance->has_method(_gdvirtual_##m_name##_sn);                                                                                                                                       \
		}                                                                                                                                                                                                       \
		if (unlikely(_get_extension() && !_gdvirtual_##m_name##_initialized)) {                                                                                                                                 \
			_gdvirtual_##m_name = (_get_extension() && _get_extension()->get_virtual) ? _get_extension()->get_virtual(_get_extension()->class_userdata, #m_name) : (GDNativeExtensionClassCallVirtual) nullptr; \
			_gdvirtual_##m_name##_initialized = true;                                                                                                                                                           \
		}                                                                                                                                                                                                       \
		if (_gdvirtual_##m_name) {                                                                                                                                                                              \
			return true;                                                                                                                                                                                        \
		}                                                                                                                                                                                                       \
		return false;                                                                                                                                                                                           \
	}                                                                                                                                                                                                           \
                                                                                                                                                                                                                \
	MethodInfo m_class::_gdvirtual_##m_name##_get_method_info() {                                                                                                                                               \
		MethodInfo method_info;                                                                                                                                                                                 \
		method_info.name = #m_name;                                                                                                                                                                             \
		method_info.flags = METHOD_FLAG_VIRTUAL;                                                                                                                                                                \
		method_info.return_val = GetTypeInfo<m_ret>::get_class_info();                                                                                                                                          \
		method_info.arguments.push_back(GetTypeInfo<m_type1>::get_class_info());                                                                                                                                \
                                                                                                                                                                                                                \
		return method_info;                                                                                                                                                                                     \
	}

#endif

bool Terrain2DLibrary::get_initialized() const {
	return _initialized;
}
void Terrain2DLibrary::set_initialized(const bool value) {
	_initialized = value;
}

bool Terrain2DLibrary::supports_caching() {
	RETURN_CALLD(bool, false, _supports_caching);
}
bool Terrain2DLibrary::_supports_caching() {
	return false;
}

//Textures

Ref<Texture> Terrain2DLibrary::texture_get() {
	return _texture;
}
void Terrain2DLibrary::texture_set(const Ref<Texture> &value) {
	_texture = value;
}

Ref<Texture> Terrain2DLibrary::liquid_texture_get() {
	return _liquid_texture;
}
void Terrain2DLibrary::liquid_texture_set(const Ref<Texture> &value) {
	_liquid_texture = value;
}

Ref<Texture> Terrain2DLibrary::prop_texture_get() {
	return _prop_texture;
}
void Terrain2DLibrary::prop_texture_set(const Ref<Texture> &value) {
	_prop_texture = value;
}

//Materials

void Terrain2DLibrary::material_cache_get_key(Ref<Terrain2DChunk> chunk) {
	CALL(_material_cache_get_key, chunk);
}

void Terrain2DLibrary::_material_cache_get_key(Ref<Terrain2DChunk> chunk) {
}

Ref<Terrain2DMaterialCache> Terrain2DLibrary::material_cache_get(const int key) {
	RETURN_CALLP(Ref<Terrain2DMaterialCache>, _material_cache_get, key);
}

Ref<Terrain2DMaterialCache> Terrain2DLibrary::_material_cache_get(const int key) {
	ERR_FAIL_V_MSG(Ref<Terrain2DMaterialCache>(), "This Terrain2DLibrary doesn't support cached materials!");
}

void Terrain2DLibrary::material_cache_unref(const int key) {
	CALL(_material_cache_unref, key);
}
void Terrain2DLibrary::_material_cache_unref(const int key) {
}

Ref<Material> Terrain2DLibrary::material_get() {
	return _material;
}

void Terrain2DLibrary::material_set(const Ref<Material> &value) {
	_material = value;
}

//Liquid Materials
void Terrain2DLibrary::liquid_material_cache_get_key(Ref<Terrain2DChunk> chunk) {
	CALL(_liquid_material_cache_get_key, chunk);
}

void Terrain2DLibrary::_liquid_material_cache_get_key(Ref<Terrain2DChunk> chunk) {
}

Ref<Terrain2DMaterialCache> Terrain2DLibrary::liquid_material_cache_get(const int key) {
	RETURN_CALLP(Ref<Terrain2DMaterialCache>, _liquid_material_cache_get, key);
}

Ref<Terrain2DMaterialCache> Terrain2DLibrary::_liquid_material_cache_get(const int key) {
	ERR_FAIL_V_MSG(Ref<Terrain2DMaterialCache>(), "This Terrain2DLibrary doesn't support cached liquid materials!");
}

void Terrain2DLibrary::liquid_material_cache_unref(const int key) {
	CALL(_liquid_material_cache_unref, key);
}
void Terrain2DLibrary::_liquid_material_cache_unref(const int key) {
}

Ref<Material> Terrain2DLibrary::liquid_material_get() {
	return _liquid_material;
}

void Terrain2DLibrary::liquid_material_set(const Ref<Material> &value) {
	_liquid_material = value;
}

//Prop2D Materials

void Terrain2DLibrary::prop_material_cache_get_key(Ref<Terrain2DChunk> chunk) {
	CALL(_prop_material_cache_get_key, chunk);
}

void Terrain2DLibrary::_prop_material_cache_get_key(Ref<Terrain2DChunk> chunk) {
}

Ref<Terrain2DMaterialCache> Terrain2DLibrary::prop_material_cache_get(const int key) {
	RETURN_CALLP(Ref<Terrain2DMaterialCache>, _prop_material_cache_get, key);
}

Ref<Terrain2DMaterialCache> Terrain2DLibrary::_prop_material_cache_get(const int key) {
	ERR_FAIL_V_MSG(Ref<Terrain2DMaterialCache>(), "This Terrain2DLibrary doesn't support cached prop materials!");
}

void Terrain2DLibrary::prop_material_cache_unref(const int key) {
	CALL(_prop_material_cache_unref, key);
}
void Terrain2DLibrary::_prop_material_cache_unref(const int key) {
}

Ref<Material> Terrain2DLibrary::prop_material_get() {
	return _prop_material;
}

void Terrain2DLibrary::prop_material_set(const Ref<Material> &value) {
	_prop_material = value;
}

//Surfaces
Ref<Terrain2DSurface> Terrain2DLibrary::terra_surface_get(const int index) {
	return Ref<Terrain2DSurface>();
}
void Terrain2DLibrary::terra_surface_add(Ref<Terrain2DSurface> value) {
}
void Terrain2DLibrary::terra_surface_set(int index, Ref<Terrain2DSurface> value) {
}
void Terrain2DLibrary::terra_surface_remove(const int index) {
}
int Terrain2DLibrary::terra_surface_get_num() const {
	return 0;
}
void Terrain2DLibrary::terra_surfaces_clear() {
}

Ref<PackedScene> Terrain2DLibrary::scene_get(const int id) {
	return Ref<PackedScene>();
}
void Terrain2DLibrary::scene_add(Ref<PackedScene> value) {
}
void Terrain2DLibrary::scene_set(int id, Ref<PackedScene> value) {
}
void Terrain2DLibrary::scene_remove(const int id) {
}
int Terrain2DLibrary::scene_get_num() const {
	return 0;
}
void Terrain2DLibrary::scenes_clear() {
}

#ifdef PROPS_2D_PRESENT
Ref<Prop2DData> Terrain2DLibrary::prop_get(const int id) {
	return Ref<Prop2DData>();
}
void Terrain2DLibrary::prop_add(Ref<Prop2DData> value) {
}
bool Terrain2DLibrary::prop_has(const Ref<Prop2DData> &value) const {
	return false;
}
void Terrain2DLibrary::prop_set(int id, Ref<Prop2DData> value) {
}
void Terrain2DLibrary::prop_remove(const int id) {
}
int Terrain2DLibrary::prop_get_num() const {
	return 0;
}
void Terrain2DLibrary::props_clear() {
}

Rect2 Terrain2DLibrary::get_prop_uv_rect(const Ref<Texture> &texture) {
	return Rect2(0, 0, 1, 1);
}
#endif

//Rects
void Terrain2DLibrary::refresh_rects() {
	_initialized = true;
}

void Terrain2DLibrary::setup_material_albedo(int material_index, Ref<Texture> texture) {
	if (has_method("_setup_material_albedo")) {
		CALL(_setup_material_albedo, material_index, texture);
	}
}

Terrain2DLibrary::Terrain2DLibrary() {
	_initialized = false;
}

Terrain2DLibrary::~Terrain2DLibrary() {
	_material.unref();
	_liquid_material.unref();
	_prop_material.unref();
}

#if VERSION_MAJOR >= 4
TMCGDVIRTUAL1(Terrain2DLibrary, _material_cache_get_key, Ref<Terrain2DChunk>);
TMCGDVIRTUAL1R(Terrain2DLibrary, Ref<Terrain2DMaterialCache>, _material_cache_get, int);
TMCGDVIRTUAL1(Terrain2DLibrary, _material_cache_unref, int);

TMCGDVIRTUAL1(Terrain2DLibrary, _liquid_material_cache_get_key, Ref<Terrain2DChunk>);
TMCGDVIRTUAL1R(Terrain2DLibrary, Ref<Terrain2DMaterialCache>, _liquid_material_cache_get, int);
TMCGDVIRTUAL1(Terrain2DLibrary, _liquid_material_cache_unref, int);

TMCGDVIRTUAL1(Terrain2DLibrary, _prop_material_cache_get_key, Ref<Terrain2DChunk>);
TMCGDVIRTUAL1R(Terrain2DLibrary, Ref<Terrain2DMaterialCache>, _prop_material_cache_get, int);
TMCGDVIRTUAL1(Terrain2DLibrary, _prop_material_cache_unref, int);
#endif

void Terrain2DLibrary::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_initialized"), &Terrain2DLibrary::get_initialized);
	ClassDB::bind_method(D_METHOD("set_initialized", "value"), &Terrain2DLibrary::set_initialized);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "initialized", PROPERTY_HINT_NONE, "", 0), "set_initialized", "get_initialized");

#if VERSION_MAJOR < 4
	//BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::BOOL, "ret"), "_supports_caching"));
#else
	GDVIRTUAL_BIND(_supports_caching);
#endif

	ClassDB::bind_method(D_METHOD("_supports_caching"), &Terrain2DLibrary::_supports_caching);
	ClassDB::bind_method(D_METHOD("supports_caching"), &Terrain2DLibrary::supports_caching);

	ClassDB::bind_method(D_METHOD("texture_get"), &Terrain2DLibrary::texture_get);
	ClassDB::bind_method(D_METHOD("texture_set", "value"), &Terrain2DLibrary::texture_set);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), "texture_set", "texture_get");

	ClassDB::bind_method(D_METHOD("liquid_texture_get"), &Terrain2DLibrary::liquid_texture_get);
	ClassDB::bind_method(D_METHOD("liquid_texture_set", "value"), &Terrain2DLibrary::liquid_texture_set);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "liquid_texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), "liquid_texture_set", "liquid_texture_get");

	ClassDB::bind_method(D_METHOD("prop_texture_get"), &Terrain2DLibrary::prop_texture_get);
	ClassDB::bind_method(D_METHOD("prop_texture_set", "value"), &Terrain2DLibrary::prop_texture_set);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "prop_texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), "prop_texture_set", "prop_texture_get");

#if VERSION_MAJOR < 4
	//BIND_VMETHOD(MethodInfo("_setup_material_albedo", PropertyInfo(Variant::INT, "material_index"), PropertyInfo(Variant::OBJECT, "texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture")));
#else
	GDVIRTUAL_BIND(_setup_material_albedo, "material_index", "texture");
#endif

#if VERSION_MAJOR < 4
	//BIND_VMETHOD(MethodInfo("_material_cache_get_key", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "Terrain2DChunk")));
	//BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::OBJECT, "ret", PROPERTY_HINT_RESOURCE_TYPE, "Terrain2DMaterialCache"), "_material_cache_get", PropertyInfo(Variant::INT, "key")));
	//BIND_VMETHOD(MethodInfo("_material_cache_unref", PropertyInfo(Variant::INT, "key")));
#else
	GDVIRTUAL_BIND(_material_cache_get_key, "chunk", "texture");
	GDVIRTUAL_BIND(_material_cache_get, "key");
	GDVIRTUAL_BIND(_material_cache_unref, "key");
#endif

	ClassDB::bind_method(D_METHOD("material_cache_get_key", "chunk"), &Terrain2DLibrary::material_cache_get_key);
	ClassDB::bind_method(D_METHOD("_material_cache_get_key", "chunk"), &Terrain2DLibrary::_material_cache_get_key);
	ClassDB::bind_method(D_METHOD("material_cache_get", "key"), &Terrain2DLibrary::material_cache_get);
	ClassDB::bind_method(D_METHOD("_material_cache_get", "key"), &Terrain2DLibrary::_material_cache_get);
	ClassDB::bind_method(D_METHOD("material_cache_unref", "key"), &Terrain2DLibrary::material_cache_unref);
	ClassDB::bind_method(D_METHOD("_material_cache_unref", "key"), &Terrain2DLibrary::_material_cache_unref);

	ClassDB::bind_method(D_METHOD("material_get"), &Terrain2DLibrary::material_get);
	ClassDB::bind_method(D_METHOD("material_set", "value"), &Terrain2DLibrary::material_set);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "material", PROPERTY_HINT_RESOURCE_TYPE, "Material"), "material_set", "material_get");

#if VERSION_MAJOR < 4
	//BIND_VMETHOD(MethodInfo("_liquid_material_cache_get_key", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "Terrain2DChunk")));
	//BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::OBJECT, "ret", PROPERTY_HINT_RESOURCE_TYPE, "Terrain2DMaterialCache"), "_liquid_material_cache_get", PropertyInfo(Variant::INT, "key")));
	//BIND_VMETHOD(MethodInfo("_liquid_material_cache_unref", PropertyInfo(Variant::INT, "key")));
#else
	GDVIRTUAL_BIND(_liquid_material_cache_get_key, "chunk", "texture");
	GDVIRTUAL_BIND(_liquid_material_cache_get, "key");
	GDVIRTUAL_BIND(_liquid_material_cache_unref, "key");
#endif

	ClassDB::bind_method(D_METHOD("liquid_material_cache_get_key", "chunk"), &Terrain2DLibrary::liquid_material_cache_get_key);
	ClassDB::bind_method(D_METHOD("_liquid_material_cache_get_key", "chunk"), &Terrain2DLibrary::_liquid_material_cache_get_key);
	ClassDB::bind_method(D_METHOD("liquid_material_cache_get", "key"), &Terrain2DLibrary::liquid_material_cache_get);
	ClassDB::bind_method(D_METHOD("_liquid_material_cache_get", "key"), &Terrain2DLibrary::_liquid_material_cache_get);
	ClassDB::bind_method(D_METHOD("liquid_material_cache_unref", "key"), &Terrain2DLibrary::liquid_material_cache_unref);
	ClassDB::bind_method(D_METHOD("_liquid_material_cache_unref", "key"), &Terrain2DLibrary::_liquid_material_cache_unref);

	ClassDB::bind_method(D_METHOD("liquid_material_get"), &Terrain2DLibrary::liquid_material_get);
	ClassDB::bind_method(D_METHOD("liquid_material_set", "value"), &Terrain2DLibrary::liquid_material_set);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "liquid_material", PROPERTY_HINT_RESOURCE_TYPE, "Material"), "liquid_material_set", "liquid_material_get");

#if VERSION_MAJOR < 4
	//BIND_VMETHOD(MethodInfo("_prop_material_cache_get_key", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "Terrain2DChunk")));
	//BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::OBJECT, "ret", PROPERTY_HINT_RESOURCE_TYPE, "Terrain2DMaterialCache"), "_prop_material_cache_get", PropertyInfo(Variant::INT, "key")));
	//BIND_VMETHOD(MethodInfo("_prop_material_cache_unref", PropertyInfo(Variant::INT, "key")));
#else
	GDVIRTUAL_BIND(_prop_material_cache_get_key, "chunk", "texture");
	GDVIRTUAL_BIND(_prop_material_cache_get, "key");
	GDVIRTUAL_BIND(_prop_material_cache_unref, "key");
#endif

	ClassDB::bind_method(D_METHOD("prop_material_cache_get_key", "chunk"), &Terrain2DLibrary::prop_material_cache_get_key);
	ClassDB::bind_method(D_METHOD("_prop_material_cache_get_key", "chunk"), &Terrain2DLibrary::_prop_material_cache_get_key);
	ClassDB::bind_method(D_METHOD("prop_material_cache_get", "key"), &Terrain2DLibrary::prop_material_cache_get);
	ClassDB::bind_method(D_METHOD("_prop_material_cache_get", "key"), &Terrain2DLibrary::_prop_material_cache_get);
	ClassDB::bind_method(D_METHOD("prop_material_cache_unref", "key"), &Terrain2DLibrary::prop_material_cache_unref);
	ClassDB::bind_method(D_METHOD("_prop_material_cache_unref", "key"), &Terrain2DLibrary::_prop_material_cache_unref);

	ClassDB::bind_method(D_METHOD("prop_material_get"), &Terrain2DLibrary::prop_material_get);
	ClassDB::bind_method(D_METHOD("prop_material_set", "value"), &Terrain2DLibrary::prop_material_set);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "prop_material", PROPERTY_HINT_RESOURCE_TYPE, "Material"), "prop_material_set", "prop_material_get");

	ClassDB::bind_method(D_METHOD("terra_surface_get", "index"), &Terrain2DLibrary::terra_surface_get);
	ClassDB::bind_method(D_METHOD("terra_surface_add", "value"), &Terrain2DLibrary::terra_surface_add);
	ClassDB::bind_method(D_METHOD("terra_surface_set", "index", "surface"), &Terrain2DLibrary::terra_surface_set);
	ClassDB::bind_method(D_METHOD("terra_surface_remove", "index"), &Terrain2DLibrary::terra_surface_remove);
	ClassDB::bind_method(D_METHOD("terra_surface_get_num"), &Terrain2DLibrary::terra_surface_get_num);
	ClassDB::bind_method(D_METHOD("terra_surfaces_clear"), &Terrain2DLibrary::terra_surfaces_clear);

	ClassDB::bind_method(D_METHOD("scene_get", "index"), &Terrain2DLibrary::scene_get);
	ClassDB::bind_method(D_METHOD("scene_add", "value"), &Terrain2DLibrary::scene_add);
	ClassDB::bind_method(D_METHOD("scene_set", "index", "value"), &Terrain2DLibrary::scene_set);
	ClassDB::bind_method(D_METHOD("scene_remove", "index"), &Terrain2DLibrary::scene_remove);
	ClassDB::bind_method(D_METHOD("scene_get_num"), &Terrain2DLibrary::scene_get_num);
	ClassDB::bind_method(D_METHOD("scenes_clear"), &Terrain2DLibrary::scenes_clear);

#ifdef PROPS_2D_PRESENT
	ClassDB::bind_method(D_METHOD("prop_get", "id"), &Terrain2DLibrary::prop_get);
	ClassDB::bind_method(D_METHOD("prop_add", "value"), &Terrain2DLibrary::prop_add);
	ClassDB::bind_method(D_METHOD("prop_has", "prop"), &Terrain2DLibrary::prop_has);
	ClassDB::bind_method(D_METHOD("prop_set", "id", "surface"), &Terrain2DLibrary::prop_set);
	ClassDB::bind_method(D_METHOD("prop_remove", "id"), &Terrain2DLibrary::prop_remove);
	ClassDB::bind_method(D_METHOD("prop_get_num"), &Terrain2DLibrary::prop_get_num);
	ClassDB::bind_method(D_METHOD("props_clear"), &Terrain2DLibrary::props_clear);
#endif

	ClassDB::bind_method(D_METHOD("refresh_rects"), &Terrain2DLibrary::refresh_rects);

	ClassDB::bind_method(D_METHOD("setup_material_albedo", "material_index", "texture"), &Terrain2DLibrary::setup_material_albedo);

	BIND_CONSTANT(MATERIAL_INDEX_TERRAIN);
	BIND_CONSTANT(MATERIAL_INDEX_LIQUID);
	BIND_CONSTANT(MATERIAL_INDEX_PROP);
}
