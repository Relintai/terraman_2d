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

#include "terraman_library_merger_2d.h"

#include "scene/resources/packed_scene.h"
#include "scene/resources/texture.h"

#ifdef PROPS_PRESENT
#include "../../props/props/prop_data.h"

#include "../../props/props/prop_data_prop.h"

#if MESH_DATA_RESOURCE_PRESENT
#include "../../mesh_data_resource/props/prop_data_mesh_data.h"
#endif
#endif

#include "../defines.h"

int TerramanLibraryMerger2D::get_texture_flags() const {
	return _packer->get_texture_flags();
}
void TerramanLibraryMerger2D::set_texture_flags(const int flags) {
	_packer->set_texture_flags(flags);
	_prop_packer->set_texture_flags(flags);
}

int TerramanLibraryMerger2D::get_max_atlas_size() const {
	return _packer->get_max_atlas_size();
}
void TerramanLibraryMerger2D::set_max_atlas_size(const int size) {
	_packer->set_max_atlas_size(size);
	_prop_packer->set_max_atlas_size(size);
}

bool TerramanLibraryMerger2D::get_keep_original_atlases() const {
	return _packer->get_keep_original_atlases();
}
void TerramanLibraryMerger2D::set_keep_original_atlases(const bool value) {
	_packer->set_keep_original_atlases(value);
	_prop_packer->set_keep_original_atlases(value);
}

Color TerramanLibraryMerger2D::get_background_color() const {
	return _packer->get_background_color();
}
void TerramanLibraryMerger2D::set_background_color(const Color &color) {
	_packer->set_background_color(color);
	_prop_packer->set_background_color(color);
}

int TerramanLibraryMerger2D::get_margin() const {
	return _packer->get_margin();
}
void TerramanLibraryMerger2D::set_margin(const int margin) {
	_packer->set_margin(margin);
	_prop_packer->set_margin(margin);
}

//Surfaces
Ref<TerraSurface2D> TerramanLibraryMerger2D::voxel_surface_get(const int index) {
	ERR_FAIL_INDEX_V(index, _voxel_surfaces.size(), Ref<TerraSurface2D>(NULL));

	return _voxel_surfaces[index];
}

void TerramanLibraryMerger2D::voxel_surface_add(Ref<TerraSurface2D> value) {
	ERR_FAIL_COND(!value.is_valid());

	value->set_library(Ref<TerramanLibraryMerger2D>(this));
	value->set_id(_voxel_surfaces.size());

	_voxel_surfaces.push_back(value);
}

void TerramanLibraryMerger2D::voxel_surface_set(const int index, Ref<TerraSurface2D> value) {
	ERR_FAIL_COND(index < 0);

	if (_voxel_surfaces.size() < index) {
		_voxel_surfaces.resize(index + 1);
	}

	if (_voxel_surfaces[index].is_valid()) {
		_voxel_surfaces.get(index)->set_library(Ref<TerramanLibraryMerger2D>(NULL));
	}

	if (value.is_valid()) {
		value->set_library(Ref<TerramanLibraryMerger2D>(this));

		_voxel_surfaces.set(index, value);
	}
}

void TerramanLibraryMerger2D::voxel_surface_remove(const int index) {
	_voxel_surfaces.remove(index);
}

int TerramanLibraryMerger2D::voxel_surface_get_num() const {
	return _voxel_surfaces.size();
}

void TerramanLibraryMerger2D::voxel_surfaces_clear() {
	_packer->clear();

	for (int i = 0; i < _voxel_surfaces.size(); i++) {
		Ref<TerraSurfaceMerger2D> surface = _voxel_surfaces[i];

		if (surface.is_valid()) {
			surface->set_library(NULL);
		}
	}

	_voxel_surfaces.clear();
}

Vector<Variant> TerramanLibraryMerger2D::get_voxel_surfaces() {
	VARIANT_ARRAY_GET(_voxel_surfaces);
}

void TerramanLibraryMerger2D::set_voxel_surfaces(const Vector<Variant> &surfaces) {
	_voxel_surfaces.clear();

	for (int i = 0; i < surfaces.size(); i++) {
		Ref<TerraSurfaceMerger2D> surface = Ref<TerraSurfaceMerger2D>(surfaces[i]);

		if (surface.is_valid()) {
			surface->set_library(Ref<TerramanLibraryMerger2D>(this));
		}

		_voxel_surfaces.push_back(surface);
	}
}

#ifdef PROPS_PRESENT
Ref<PropData> TerramanLibraryMerger2D::get_prop(const int index) {
	ERR_FAIL_INDEX_V(index, _props.size(), Ref<PropData>());

	return _props[index];
}
void TerramanLibraryMerger2D::add_prop(Ref<PropData> value) {
	_props.push_back(value);
}
bool TerramanLibraryMerger2D::has_prop(const Ref<PropData> &value) const {
	return _props.find(value) != -1;
}
void TerramanLibraryMerger2D::set_prop(const int index, const Ref<PropData> &value) {
	ERR_FAIL_INDEX(index, _props.size());

	_props.write[index] = value;
}
void TerramanLibraryMerger2D::remove_prop(const int index) {
	ERR_FAIL_INDEX(index, _props.size());

	_props.remove(index);
}
int TerramanLibraryMerger2D::get_num_props() const {
	return _props.size();
}
void TerramanLibraryMerger2D::clear_props() {
	_props.clear();
}

Vector<Variant> TerramanLibraryMerger2D::get_props() {
	VARIANT_ARRAY_GET(_props);
}

void TerramanLibraryMerger2D::set_props(const Vector<Variant> &props) {
	VARIANT_ARRAY_SET(props, _props, PropData);
}

Rect2 TerramanLibraryMerger2D::get_prop_uv_rect(const Ref<Texture> &texture) {
	if (!texture.is_valid()) {
		return Rect2(0, 0, 1, 1);
	}

	Ref<AtlasTexture> at = _prop_packer->get_texture(texture);

	if (!at.is_valid()) {
		return Rect2(0, 0, 1, 1);
	}

	Rect2 region = at->get_region();

	Ref<Texture> tex = at->get_atlas();

	if (!tex.is_valid()) {
		return Rect2(0, 0, 1, 1);
	}

	Ref<Image> image = tex->get_data();

	if (!image.is_valid()) {
		return Rect2(0, 0, 1, 1);
	}

	float w = image->get_width();
	float h = image->get_height();

	region.position = Size2(region.position.x / w, region.position.y / h);
	region.size = Size2(region.size.x / w, region.size.y / h);

	return region;
}

Ref<TexturePacker> TerramanLibraryMerger2D::get_prop_packer() {
	return _prop_packer;
}
#endif

void TerramanLibraryMerger2D::refresh_rects() {
	bool texture_added = false;
	for (int i = 0; i < _voxel_surfaces.size(); i++) {
		Ref<TerraSurfaceMerger2D> surface = Ref<TerraSurfaceMerger2D>(_voxel_surfaces[i]);

		if (surface.is_valid()) {
			for (int j = 0; j < TerraSurface2D::TERRA_SIDES_COUNT; ++j) {
				Ref<Texture> tex = surface->get_texture(static_cast<TerraSurface2D::TerraSurfaceSides2D>(j));

				if (!tex.is_valid())
					continue;

				if (!_packer->contains_texture(tex)) {
					texture_added = true;
					surface->set_region(static_cast<TerraSurface2D::TerraSurfaceSides2D>(j), _packer->add_texture(tex));
				} else {
					surface->set_region(static_cast<TerraSurface2D::TerraSurfaceSides2D>(j), _packer->get_texture(tex));
				}
			}
		}
	}

	if (texture_added) {
		_packer->merge();

		ERR_FAIL_COND(_packer->get_texture_count() == 0);

		Ref<Texture> tex = _packer->get_generated_texture(0);

		setup_material_albedo(MATERIAL_INDEX_TERRAS, tex);
		setup_material_albedo(MATERIAL_INDEX_LIQUID, tex);
	}

#ifdef PROPS_PRESENT
	//todo add this back
	//texture_added = false;
	for (int i = 0; i < _props.size(); i++) {
		Ref<PropData> prop = _props.get(i);

		if (prop.is_valid()) {
			if (process_prop_textures(prop))
				texture_added = true;
		}
	}

	//if (texture_added) {
	if (_prop_packer->get_texture_count() > 0) {
		_prop_packer->merge();

		//ERR_FAIL_COND(_prop_packer->get_texture_count() == 0);

		Ref<Texture> tex = _prop_packer->get_generated_texture(0);

		setup_material_albedo(MATERIAL_INDEX_PROP, tex);
	}
#endif

	for (int i = 0; i < _voxel_surfaces.size(); i++) {
		Ref<TerraSurfaceMerger2D> surface = _voxel_surfaces[i];

		if (surface.is_valid()) {
			surface->refresh_rects();
		}
	}

	set_initialized(true);
}

void TerramanLibraryMerger2D::_setup_material_albedo(const int material_index, const Ref<Texture> &texture) {
	Ref<SpatialMaterial> mat;

	int count = 0;

	switch (material_index) {
		case MATERIAL_INDEX_TERRAS:
			count = material_get_num();
			break;
		case MATERIAL_INDEX_LIQUID:
			count = liquid_material_get_num();
			break;
		case MATERIAL_INDEX_PROP:
			count = prop_material_get_num();
			break;
	}

	for (int i = 0; i < count; ++i) {
		switch (material_index) {
			case MATERIAL_INDEX_TERRAS:
				mat = material_get(i);
				break;
			case MATERIAL_INDEX_LIQUID:
				mat = liquid_material_get(i);
				break;
			case MATERIAL_INDEX_PROP:
				mat = prop_material_get(i);
				break;
		}

		Ref<SpatialMaterial> spmat;

		if (spmat.is_valid()) {
			spmat->set_texture(SpatialMaterial::TEXTURE_ALBEDO, texture);
			return;
		}

		Ref<ShaderMaterial> shmat;

		switch (material_index) {
			case MATERIAL_INDEX_TERRAS:
				shmat = material_get(i);
				break;
			case MATERIAL_INDEX_LIQUID:
				shmat = liquid_material_get(i);
				break;
			case MATERIAL_INDEX_PROP:
				shmat = prop_material_get(i);
				break;
		}

		if (shmat.is_valid()) {
			shmat->set_shader_param("texture_albedo", texture);
		}
	}
}

TerramanLibraryMerger2D::TerramanLibraryMerger2D() {
	_packer.instance();

#if GODOT4
#warning implement
#else
	_packer->set_texture_flags(Texture::FLAG_MIPMAPS | Texture::FLAG_FILTER);
#endif

	_packer->set_max_atlas_size(1024);
	_packer->set_keep_original_atlases(false);
	_packer->set_margin(0);

	_prop_packer.instance();

#if GODOT4
#warning implement
#else
	_prop_packer->set_texture_flags(Texture::FLAG_MIPMAPS | Texture::FLAG_FILTER);
#endif

	_prop_packer->set_max_atlas_size(1024);
	_prop_packer->set_keep_original_atlases(false);
	_prop_packer->set_margin(0);
}

TerramanLibraryMerger2D::~TerramanLibraryMerger2D() {
	for (int i = 0; i < _voxel_surfaces.size(); ++i) {
		Ref<TerraSurface2D> surface = _voxel_surfaces[i];

		if (surface.is_valid()) {
			surface->set_library(Ref<TerramanLibraryMerger2D>());
		}
	}

	_voxel_surfaces.clear();

	_packer->clear();
	_packer.unref();

	_prop_packer->clear();
	_prop_packer.unref();
}

#ifdef PROPS_PRESENT
bool TerramanLibraryMerger2D::process_prop_textures(Ref<PropData> prop) {
	if (!prop.is_valid()) {
		return false;
	}

	bool texture_added = false;

	for (int i = 0; i < prop->get_prop_count(); ++i) {
#if MESH_DATA_RESOURCE_PRESENT
		Ref<PropDataMeshData> pdm = prop->get_prop(i);

		if (pdm.is_valid()) {
			Ref<Texture> tex = pdm->get_texture();

			if (!tex.is_valid())
				continue;

			if (!_prop_packer->contains_texture(tex)) {
				_prop_packer->add_texture(tex);
				texture_added = true;
			}
		}
#endif

		Ref<PropDataProp> pdp = prop->get_prop(i);

		if (pdp.is_valid()) {
			if (process_prop_textures(pdp))
				texture_added = true;
		}
	}

	return texture_added;
}
#endif

void TerramanLibraryMerger2D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_texture_flags"), &TerramanLibraryMerger2D::get_texture_flags);
	ClassDB::bind_method(D_METHOD("set_texture_flags", "flags"), &TerramanLibraryMerger2D::set_texture_flags);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "texture_flags", PROPERTY_HINT_FLAGS, "Mipmaps,Repeat,Filter,Anisotropic Linear,Convert to Linear,Mirrored Repeat,Video Surface"), "set_texture_flags", "get_texture_flags");

	ClassDB::bind_method(D_METHOD("get_max_atlas_size"), &TerramanLibraryMerger2D::get_max_atlas_size);
	ClassDB::bind_method(D_METHOD("set_max_atlas_size", "size"), &TerramanLibraryMerger2D::set_max_atlas_size);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "max_atlas_size"), "set_max_atlas_size", "get_max_atlas_size");

	ClassDB::bind_method(D_METHOD("get_keep_original_atlases"), &TerramanLibraryMerger2D::get_keep_original_atlases);
	ClassDB::bind_method(D_METHOD("set_keep_original_atlases", "value"), &TerramanLibraryMerger2D::set_keep_original_atlases);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "keep_original_atlases"), "set_keep_original_atlases", "get_keep_original_atlases");

	ClassDB::bind_method(D_METHOD("get_background_color"), &TerramanLibraryMerger2D::get_background_color);
	ClassDB::bind_method(D_METHOD("set_background_color", "color"), &TerramanLibraryMerger2D::set_background_color);
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "background_color"), "set_background_color", "get_background_color");

	ClassDB::bind_method(D_METHOD("get_margin"), &TerramanLibraryMerger2D::get_margin);
	ClassDB::bind_method(D_METHOD("set_margin", "size"), &TerramanLibraryMerger2D::set_margin);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "margin"), "set_margin", "get_margin");

	ClassDB::bind_method(D_METHOD("get_voxel_surfaces"), &TerramanLibraryMerger2D::get_voxel_surfaces);
	ClassDB::bind_method(D_METHOD("set_voxel_surfaces"), &TerramanLibraryMerger2D::set_voxel_surfaces);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "voxel_surfaces", PROPERTY_HINT_NONE, "17/17:TerraSurfaceMerger2D", PROPERTY_USAGE_DEFAULT, "TerraSurfaceMerger2D"), "set_voxel_surfaces", "get_voxel_surfaces");

#ifdef PROPS_PRESENT
	ClassDB::bind_method(D_METHOD("get_props"), &TerramanLibraryMerger2D::get_props);
	ClassDB::bind_method(D_METHOD("set_props"), &TerramanLibraryMerger2D::set_props);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "props", PROPERTY_HINT_NONE, "17/17:PropData", PROPERTY_USAGE_DEFAULT, "PropData"), "set_props", "get_props");

	ClassDB::bind_method(D_METHOD("get_prop_uv_rect", "texture"), &TerramanLibraryMerger2D::get_prop_uv_rect);

	ClassDB::bind_method(D_METHOD("get_prop_packer"), &TerramanLibraryMerger2D::get_prop_packer);
#endif

	ClassDB::bind_method(D_METHOD("_setup_material_albedo", "material_index", "texture"), &TerramanLibraryMerger2D::_setup_material_albedo);
}
