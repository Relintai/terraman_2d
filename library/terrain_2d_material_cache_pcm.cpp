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

#include "terrain_2d_material_cache_pcm.h"

#include "../../texture_packer/texture_packer.h"
#include "scene/resources/texture.h"
#include "terrain_2d_surface.h"
#include "terrain_2d_surface_merger.h"

int Terrain2DMaterialCachePCM::get_texture_flags() const {
	return _packer->get_texture_flags();
}
void Terrain2DMaterialCachePCM::set_texture_flags(const int flags) {
	_packer->set_texture_flags(flags);
}

int Terrain2DMaterialCachePCM::get_max_atlas_size() const {
	return _packer->get_max_atlas_size();
}
void Terrain2DMaterialCachePCM::set_max_atlas_size(const int size) {
	_packer->set_max_atlas_size(size);
}

bool Terrain2DMaterialCachePCM::get_keep_original_atlases() const {
	return _packer->get_keep_original_atlases();
}
void Terrain2DMaterialCachePCM::set_keep_original_atlases(const bool value) {
	_packer->set_keep_original_atlases(value);
}

Color Terrain2DMaterialCachePCM::get_background_color() const {
	return _packer->get_background_color();
}
void Terrain2DMaterialCachePCM::set_background_color(const Color &color) {
	_packer->set_background_color(color);
}

int Terrain2DMaterialCachePCM::get_margin() const {
	return _packer->get_margin();
}
void Terrain2DMaterialCachePCM::set_margin(const int margin) {
	_packer->set_margin(margin);
}

Ref<AtlasTexture> Terrain2DMaterialCachePCM::additional_texture_get_atlas_tex(const Ref<Texture> &texture) {
	if (!_packer->contains_texture(texture)) {
		return Ref<AtlasTexture>();
	}

	return _packer->get_texture(texture);
}
Rect2 Terrain2DMaterialCachePCM::additional_texture_get_uv_rect(const Ref<Texture> &texture) {
	if (!texture.is_valid()) {
		return Rect2(0, 0, 1, 1);
	}

	Ref<AtlasTexture> at = _packer->get_texture(texture);

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

	float w = tex->get_width();
	float h = tex->get_height();

	region.position = Size2(region.position.x / w, region.position.y / h);
	region.size = Size2(region.size.x / w, region.size.y / h);

	return region;
}

void Terrain2DMaterialCachePCM::refresh_rects() {
	bool texture_added = false;
	for (int i = 0; i < _surfaces.size(); i++) {
		Ref<Terrain2DSurfaceMerger> surface = Ref<Terrain2DSurfaceMerger>(_surfaces[i]);

		if (surface.is_valid()) {
			for (int j = 0; j < Terrain2DSurface::TERRAIN_2D_SIDES_COUNT; ++j) {
				Ref<Texture> tex = surface->get_texture(static_cast<Terrain2DSurface::Terrain2DSurfaceSides>(j));

				if (!tex.is_valid())
					continue;

				if (!_packer->contains_texture(tex)) {
					texture_added = true;
					surface->set_region(static_cast<Terrain2DSurface::Terrain2DSurfaceSides>(j), _packer->add_texture(tex));
				} else {
					surface->set_region(static_cast<Terrain2DSurface::Terrain2DSurfaceSides>(j), _packer->get_texture(tex));
				}
			}
		}
	}

	for (int i = 0; i < _additional_textures.size(); i++) {
		Ref<Texture> tex = _additional_textures.get(i);

		ERR_CONTINUE(!tex.is_valid());

		if (!_packer->contains_texture(tex)) {
			_packer->add_texture(tex);
			texture_added = true;
		}
	}

	if (texture_added) {
		_packer->merge();

		ERR_FAIL_COND(_packer->get_texture_count() == 0);

		Ref<Texture> tex = _packer->get_generated_texture(0);

		setup_material_albedo(tex);
	}

	for (int i = 0; i < _surfaces.size(); i++) {
		Ref<Terrain2DSurfaceMerger> surface = _surfaces[i];

		if (surface.is_valid()) {
			surface->refresh_rects();
		}
	}

	_initialized = true;
}

void Terrain2DMaterialCachePCM::_setup_material_albedo(Ref<Texture> texture) {
	int count = material_get_num();

	for (int i = 0; i < count; ++i) {
		Ref<Material> m = material_get(i);

		Ref<SpatialMaterial> spmat = m;

		if (spmat.is_valid()) {
			spmat->set_texture(SpatialMaterial::TEXTURE_ALBEDO, texture);
			return;
		}

		Ref<ShaderMaterial> shmat = m;

		if (shmat.is_valid()) {
			shmat->set_shader_param("texture_albedo", texture);
		}
	}
}

Terrain2DMaterialCachePCM::Terrain2DMaterialCachePCM() {
	_packer.instance();

#if GODOT4
#warning implement
#else
	_packer->set_texture_flags(Texture::FLAG_MIPMAPS | Texture::FLAG_FILTER);
#endif

	_packer->set_max_atlas_size(1024);
	_packer->set_keep_original_atlases(false);
	_packer->set_margin(0);
}

Terrain2DMaterialCachePCM::~Terrain2DMaterialCachePCM() {
	for (int i = 0; i < _surfaces.size(); ++i) {
		Ref<Terrain2DSurface> surface = _surfaces[i];

		if (surface.is_valid()) {
			surface->set_library(Ref<Terrain2DLibrary>());
		}
	}

	_surfaces.clear();

	_packer->clear();
	_packer.unref();
}

void Terrain2DMaterialCachePCM::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_texture_flags"), &Terrain2DMaterialCachePCM::get_texture_flags);
	ClassDB::bind_method(D_METHOD("set_texture_flags", "flags"), &Terrain2DMaterialCachePCM::set_texture_flags);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "texture_flags", PROPERTY_HINT_FLAGS, "Mipmaps,Repeat,Filter,Anisotropic Linear,Convert to Linear,Mirrored Repeat,Video Surface"), "set_texture_flags", "get_texture_flags");

	ClassDB::bind_method(D_METHOD("get_max_atlas_size"), &Terrain2DMaterialCachePCM::get_max_atlas_size);
	ClassDB::bind_method(D_METHOD("set_max_atlas_size", "size"), &Terrain2DMaterialCachePCM::set_max_atlas_size);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "max_atlas_size"), "set_max_atlas_size", "get_max_atlas_size");

	ClassDB::bind_method(D_METHOD("get_keep_original_atlases"), &Terrain2DMaterialCachePCM::get_keep_original_atlases);
	ClassDB::bind_method(D_METHOD("set_keep_original_atlases", "value"), &Terrain2DMaterialCachePCM::set_keep_original_atlases);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "keep_original_atlases"), "set_keep_original_atlases", "get_keep_original_atlases");

	ClassDB::bind_method(D_METHOD("get_background_color"), &Terrain2DMaterialCachePCM::get_background_color);
	ClassDB::bind_method(D_METHOD("set_background_color", "color"), &Terrain2DMaterialCachePCM::set_background_color);
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "background_color"), "set_background_color", "get_background_color");

	ClassDB::bind_method(D_METHOD("get_margin"), &Terrain2DMaterialCachePCM::get_margin);
	ClassDB::bind_method(D_METHOD("set_margin", "size"), &Terrain2DMaterialCachePCM::set_margin);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "margin"), "set_margin", "get_margin");

	ClassDB::bind_method(D_METHOD("_setup_material_albedo", "texture"), &Terrain2DMaterialCachePCM::_setup_material_albedo);
}
