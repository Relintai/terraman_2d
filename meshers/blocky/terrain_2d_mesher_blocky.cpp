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

#include "terrain_2d_mesher_blocky.h"

#include "core/math/math_funcs.h"

#include "../../library/terrain_2d_material_cache.h"

bool Terrain2DMesherBlocky::get_always_add_colors() const {
	return _always_add_colors;
}
void Terrain2DMesherBlocky::set_always_add_colors(const bool value) {
	_always_add_colors = value;
}

void Terrain2DMesherBlocky::_add_chunk(Ref<Terrain2DChunk> p_chunk) {
	Ref<Terrain2DChunkDefault> chunk = p_chunk;

	ERR_FAIL_COND(!chunk.is_valid());
	ERR_FAIL_COND(chunk->get_margin_end() < 1);
	ERR_FAIL_COND(chunk->get_margin_start() < 1);

	add_chunk_normal(chunk);
}

void Terrain2DMesherBlocky::add_chunk_normal(Ref<Terrain2DChunkDefault> chunk) {
	//if ((get_build_flags() & Terrain2DChunkDefault::BUILD_FLAG_GENERATE_AO) != 0)
	//	if (!chunk->get_channel(Terrain2DChunkDefault::DEFAULT_CHANNEL_AO))
	//		chunk->generate_ao();

	int x_size = chunk->get_size_x();
	int z_size = chunk->get_size_z();
	int cell_size_x = get_cell_size_x();
	int cell_size_y = get_cell_size_y();

	uint8_t *channel_type = chunk->channel_get(_channel_index_type);

	if (!channel_type)
		return;

	uint8_t *channel_color_r = NULL;
	uint8_t *channel_color_g = NULL;
	uint8_t *channel_color_b = NULL;
	uint8_t *channel_ao = NULL;
	uint8_t *channel_rao = NULL;

	Color base_light(_base_light_value, _base_light_value, _base_light_value);
	Color light[4]{ Color(1, 1, 1), Color(1, 1, 1), Color(1, 1, 1), Color(1, 1, 1) };

	bool use_lighting = (get_build_flags() & Terrain2DChunkDefault::BUILD_FLAG_USE_LIGHTING) != 0;
	bool use_ao = (get_build_flags() & Terrain2DChunkDefault::BUILD_FLAG_USE_AO) != 0;
	bool use_rao = (get_build_flags() & Terrain2DChunkDefault::BUILD_FLAG_USE_RAO) != 0;

	if (use_lighting) {
		channel_color_r = chunk->channel_get_valid(Terrain2DChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_R);
		channel_color_g = chunk->channel_get_valid(Terrain2DChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_G);
		channel_color_b = chunk->channel_get_valid(Terrain2DChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_B);

		if (use_ao)
			channel_ao = chunk->channel_get_valid(Terrain2DChunkDefault::DEFAULT_CHANNEL_AO);

		if (use_rao)
			channel_rao = chunk->channel_get_valid(Terrain2DChunkDefault::DEFAULT_CHANNEL_RANDOM_AO);
	}

	Ref<Terrain2DMaterialCache> mcache;

	if (chunk->material_cache_key_has()) {
		mcache = _library->material_cache_get(chunk->material_cache_key_get());
	}

	int margin_start = chunk->get_margin_start();
	//z_size + margin_start is fine, x, and z are in data space.
	for (int z = margin_start; z < z_size + margin_start; ++z) {
		for (int x = margin_start; x < x_size + margin_start; ++x) {
			int indexes[4] = {
				chunk->get_data_index(x + 1, z),
				chunk->get_data_index(x, z),
				chunk->get_data_index(x, z + 1),
				chunk->get_data_index(x + 1, z + 1)
			};

			uint8_t type = channel_type[indexes[0]];

			if (type == 0)
				continue;

			Ref<Terrain2DSurface> surface;

			if (!mcache.is_valid()) {
				surface = _library->terra_surface_get(type - 1);
			} else {
				surface = mcache->surface_id_get(type - 1);
			}

			if (!surface.is_valid())
				continue;

			if (use_lighting) {
				for (int i = 0; i < 4; ++i) {
					int indx = indexes[i];

					light[i] = Color(channel_color_r[indx] / 255.0,
							channel_color_g[indx] / 255.0,
							channel_color_b[indx] / 255.0);

					float ao = 0;

					if (use_ao)
						ao = channel_ao[indx] / 255.0;

					if (use_rao) {
						float rao = channel_rao[indx] / 255.0;
						ao += rao;
					}

					light[i] += base_light;

					if (ao > 0)
						light[i] -= Color(ao, ao, ao) * _ao_strength;

					light[i].r = CLAMP(light[i].r, 0, 1.0);
					light[i].g = CLAMP(light[i].g, 0, 1.0);
					light[i].b = CLAMP(light[i].b, 0, 1.0);
				}
			}

			int vc = get_vertex_count();
			add_indices(vc + 2);
			add_indices(vc + 1);
			add_indices(vc + 0);
			add_indices(vc + 3);
			add_indices(vc + 2);
			add_indices(vc + 0);

			Vector2 uvs[] = {
				surface->transform_uv_scaled(Terrain2DSurface::TERRAIN_2D_SIDE_TOP, Vector2(1, 0), x % get_texture_scale(), z % get_texture_scale(), get_texture_scale()),
				surface->transform_uv_scaled(Terrain2DSurface::TERRAIN_2D_SIDE_TOP, Vector2(0, 0), x % get_texture_scale(), z % get_texture_scale(), get_texture_scale()),
				surface->transform_uv_scaled(Terrain2DSurface::TERRAIN_2D_SIDE_TOP, Vector2(0, 1), x % get_texture_scale(), z % get_texture_scale(), get_texture_scale()),
				surface->transform_uv_scaled(Terrain2DSurface::TERRAIN_2D_SIDE_TOP, Vector2(1, 1), x % get_texture_scale(), z % get_texture_scale(), get_texture_scale())
			};

			int xx = (x + 1) * cell_size_x;
			int xx1 = x * cell_size_x;
			int yy = z * cell_size_y;
			int yy1 = (z + 1) * cell_size_y;

			Vector2 verts[] = {
				Vector2(xx1, yy),
				Vector2(xx, yy),
				Vector2(xx, yy1),
				Vector2(xx1, yy1)
			};

			for (int i = 0; i < 4; ++i) {
				if (use_lighting || _always_add_colors)
					add_color(light[i]);

				add_uv(uvs[i]);
				add_vertex(verts[i]);
			}
		}
	}
}

Terrain2DMesherBlocky::Terrain2DMesherBlocky() {
	_always_add_colors = false;
}

Terrain2DMesherBlocky::~Terrain2DMesherBlocky() {
}

void Terrain2DMesherBlocky::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_add_chunk", "buffer"), &Terrain2DMesherBlocky::_add_chunk);

	ClassDB::bind_method(D_METHOD("get_always_add_colors"), &Terrain2DMesherBlocky::get_always_add_colors);
	ClassDB::bind_method(D_METHOD("set_always_add_colors", "value"), &Terrain2DMesherBlocky::set_always_add_colors);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "always_add_colors"), "set_always_add_colors", "get_always_add_colors");
}
