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

#include "terra_world_default_2d.h"

#include "terra_chunk_default_2d.h"

#include "../../defines.h"

#include "../../meshers/default/terra_mesher_default_2d.h"
#include "../jobs/terra_light_job_2d.h"
#include "../jobs/terra_prop_job_2d.h"
#include "../jobs/terra_terrarin_job_2d.h"

_FORCE_INLINE_ int TerraWorldDefault2D::get_build_flags() const {
	return _build_flags;
}
_FORCE_INLINE_ void TerraWorldDefault2D::set_build_flags(const int flags) {
	_build_flags = flags;

	for (int i = 0; i < chunk_get_count(); ++i) {
		Ref<TerraChunkDefault2D> c = chunk_get_index(i);

		if (!c.is_valid())
			continue;

		c->set_build_flags(_build_flags);
	}
}

float TerraWorldDefault2D::get_lod_update_interval() const {
	return _lod_update_interval;
}
void TerraWorldDefault2D::set_lod_update_interval(const float value) {
	_lod_update_interval = value;
}

int TerraWorldDefault2D::get_num_lods() const {
	return _num_lods;
}
void TerraWorldDefault2D::set_num_lods(const int value) {
	_num_lods = value;
}

void TerraWorldDefault2D::update_lods() {
	if (!get_active()) {
		return;
	}

	call("_update_lods");
}

int TerraWorldDefault2D::get_chunk_lod_first_falloff() const {
	return _chunk_lod_first_falloff;
}
void TerraWorldDefault2D::set_chunk_lod_first_falloff(const int value) {
	_chunk_lod_first_falloff = value;
}

int TerraWorldDefault2D::get_chunk_lod_falloff() const {
	return _chunk_lod_falloff;
}
void TerraWorldDefault2D::set_chunk_lod_falloff(const int value) {
	_chunk_lod_falloff = value;
}

PoolColorArray TerraWorldDefault2D::get_vertex_colors(const Transform &transform, const PoolVector3Array &vertices, const float base_light_value, const float ao_strength) {
	PoolColorArray arr;
	arr.resize(vertices.size());

	for (int i = 0; i < vertices.size(); ++i) {
		Vector3 v = transform.xform(vertices[i]);

		Vector3 pos = v / get_voxel_scale();

		//Note: floor is needed to handle negative numbers proiberly
		int x = static_cast<int>(Math::floor(pos.x / get_chunk_size_x()));
		int z = static_cast<int>(Math::floor(pos.z / get_chunk_size_z()));

		int bx = static_cast<int>(Math::floor(pos.x)) % get_chunk_size_x();
		int bz = static_cast<int>(Math::floor(pos.z)) % get_chunk_size_z();

		if (bx < 0) {
			bx += get_chunk_size_x();
		}

		if (bz < 0) {
			bz += get_chunk_size_z();
		}

		Ref<TerraChunk2D> chunk = chunk_get(x, z);

		if (chunk.is_valid()) {
			Color light = Color(
					chunk->get_voxel(bx, bz, TerraChunkDefault2D::DEFAULT_CHANNEL_LIGHT_COLOR_R) / 255.0,
					chunk->get_voxel(bx, bz, TerraChunkDefault2D::DEFAULT_CHANNEL_LIGHT_COLOR_G) / 255.0,
					chunk->get_voxel(bx, bz, TerraChunkDefault2D::DEFAULT_CHANNEL_LIGHT_COLOR_B) / 255.0);

			float ao = (chunk->get_voxel(bx, bz, TerraChunkDefault2D::DEFAULT_CHANNEL_AO) / 255.0) * ao_strength;
			float rao = chunk->get_voxel(bx, bz, TerraChunkDefault2D::DEFAULT_CHANNEL_RANDOM_AO) / 255.0;

			ao += rao;

			light.r += base_light_value;
			light.g += base_light_value;
			light.b += base_light_value;

			light.r -= ao;
			light.g -= ao;
			light.b -= ao;

			light.r = CLAMP(light.r, 0, 1.0);
			light.g = CLAMP(light.g, 0, 1.0);
			light.b = CLAMP(light.b, 0, 1.0);

			arr.set(i, light);
		} else {
			arr.set(i, Color(base_light_value, base_light_value, base_light_value));
		}
	}

	return arr;
}

void TerraWorldDefault2D::_update_lods() {
	if (!get_player() || !INSTANCE_VALIDATE(get_player())) {
		return;
	}

	if (_num_lods <= 1)
		return;

	Vector3 ppos = get_player()->get_transform().origin;

	int ppx = int(ppos.x / get_chunk_size_x() / get_voxel_scale());
	int ppz = int(ppos.z / get_chunk_size_z() / get_voxel_scale());

	for (int i = 0; i < chunk_get_count(); ++i) {
		Ref<TerraChunkDefault2D> c = chunk_get_index(i);

		if (!c.is_valid())
			continue;

		int dx = Math::abs(ppx - c->get_position_x());
		int dz = Math::abs(ppz - c->get_position_z());

		int mr = MAX(dx, dz);

		mr -= _chunk_lod_first_falloff;
		mr /= _chunk_lod_falloff;

		mr = CLAMP(mr, 0, _num_lods - 1);

		if (c->get_current_lod_level() != mr)
			c->set_current_lod_level(mr);
	}
}

Ref<TerraChunk2D> TerraWorldDefault2D::_create_chunk(int x, int z, Ref<TerraChunk2D> chunk) {
	if (!chunk.is_valid()) {
		chunk = Ref<TerraChunk2D>(memnew(TerraChunkDefault2D));
	}

	if (chunk->job_get_count() == 0) {
		Ref<TerraTerrarinJob2D> tj;
		tj.instance();

		Ref<TerraLightJob2D> lj;
		lj.instance();

		Ref<TerraPropJob2D> pj;
		pj.instance();
		pj->set_prop_mesher(Ref<TerraMesher2D>(memnew(TerraMesherDefault2D)));

		Ref<TerraMesherJobStep2D> s;
		s.instance();
		s->set_job_type(TerraMesherJobStep2D::TYPE_NORMAL);
		tj->add_jobs_step(s);

		s.instance();
		s->set_job_type(TerraMesherJobStep2D::TYPE_NORMAL_LOD);
		s->set_lod_index(1);
		tj->add_jobs_step(s);

		s.instance();
		s->set_job_type(TerraMesherJobStep2D::TYPE_NORMAL_LOD);
		s->set_lod_index(2);
		tj->add_jobs_step(s);

		s.instance();
		s->set_job_type(TerraMesherJobStep2D::TYPE_MERGE_VERTS);
		tj->add_jobs_step(s);

		s.instance();
		s->set_job_type(TerraMesherJobStep2D::TYPE_BAKE_TEXTURE);
		tj->add_jobs_step(s);

		chunk->job_add(lj);
		chunk->job_add(tj);
		chunk->job_add(pj);
	}

	Ref<TerraChunkDefault2D> vcd = chunk;

	if (vcd.is_valid()) {
		vcd->set_build_flags(_build_flags);
		vcd->set_lod_num(_num_lods);
	}

	return TerraWorld2D::_create_chunk(x, z, chunk);
}

void TerraWorldDefault2D::_chunk_added(Ref<TerraChunk2D> chunk) {
	Ref<TerraChunkDefault2D> c = chunk;

	if (c.is_valid()) {
		c->set_build_flags(_build_flags);
	}
}

int TerraWorldDefault2D::_get_channel_index_info(const TerraWorld2D::ChannelTypeInfo channel_type) {
	switch (channel_type) {
		case CHANNEL_TYPE_INFO_TYPE:
			return TerraChunkDefault2D::DEFAULT_CHANNEL_TYPE;
		case CHANNEL_TYPE_INFO_ISOLEVEL:
			return TerraChunkDefault2D::DEFAULT_CHANNEL_ISOLEVEL;
		case CHANNEL_TYPE_INFO_LIQUID_FLOW:
			return TerraChunkDefault2D::DEFAULT_CHANNEL_LIQUID_FLOW;
		default:
			return -1;
	}
}

TerraWorldDefault2D::TerraWorldDefault2D() {
	_chunk_lod_first_falloff = 2;
	_chunk_lod_falloff = 2;
	_lod_update_timer = 0;
	_lod_update_interval = 0.5;
	_build_flags = TerraChunkDefault2D::BUILD_FLAG_CREATE_COLLIDER | TerraChunkDefault2D::BUILD_FLAG_CREATE_LODS;
	_num_lods = 4;

	set_data_margin_start(1);
	set_data_margin_end(1);
}

TerraWorldDefault2D ::~TerraWorldDefault2D() {
}

void TerraWorldDefault2D::_notification(int p_what) {
	TerraWorld2D::_notification(p_what);

	switch (p_what) {
		case NOTIFICATION_ACTIVE_STATE_CHANGED: {
			if (!is_inside_tree()) {
				return;
			}

			bool active = get_active();

			for (int i = 0; i < chunk_get_count(); ++i) {
				Ref<TerraChunk2D> chunk = chunk_get_index(i);

				if (chunk.is_valid()) {
					chunk->set_visible(active);
				}
			}

			if (active) {
				update_lods();
			}

			break;
		}
	}
}

/*
void TerraWorldDefault2D::_notification(int p_what) {
	TerraWorld::_notification(p_what);

	switch (p_what) {
		case NOTIFICATION_INTERNAL_PROCESS: {
			if ((get_build_flags() & TerraChunkDefault2D::BUILD_FLAG_CREATE_LODS) == 0)
				return;

			if (!get_player()) {
				return;
			}

			if (!INSTANCE_VALIDATE(get_player())) {
				set_player(NULL);
				return;
			}

			_lod_update_timer += get_process_delta_time();

			if (_lod_update_timer >= _lod_update_interval) {
				_lod_update_timer = 0;

				update_lods();
			}
		} break;
	}
}*/

void TerraWorldDefault2D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_chunk_added", "chunk"), &TerraWorldDefault2D::_chunk_added);

	ClassDB::bind_method(D_METHOD("get_build_flags"), &TerraWorldDefault2D::get_build_flags);
	ClassDB::bind_method(D_METHOD("set_build_flags", "value"), &TerraWorldDefault2D::set_build_flags);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "build_flags", PROPERTY_HINT_FLAGS, TerraChunkDefault2D::BINDING_STRING_BUILD_FLAGS), "set_build_flags", "get_build_flags");

	ClassDB::bind_method(D_METHOD("get_lod_update_interval"), &TerraWorldDefault2D::get_lod_update_interval);
	ClassDB::bind_method(D_METHOD("set_lod_update_interval", "value"), &TerraWorldDefault2D::set_lod_update_interval);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "lod_update_interval"), "set_lod_update_interval", "get_lod_update_interval");

	ClassDB::bind_method(D_METHOD("get_chunk_lod_first_falloff"), &TerraWorldDefault2D::get_chunk_lod_first_falloff);
	ClassDB::bind_method(D_METHOD("set_chunk_lod_first_falloff", "value"), &TerraWorldDefault2D::set_chunk_lod_first_falloff);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "chunk_lod_first_falloff"), "set_chunk_lod_first_falloff", "get_chunk_lod_first_falloff");

	ClassDB::bind_method(D_METHOD("get_chunk_lod_falloff"), &TerraWorldDefault2D::get_chunk_lod_falloff);
	ClassDB::bind_method(D_METHOD("set_chunk_lod_falloff", "value"), &TerraWorldDefault2D::set_chunk_lod_falloff);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "chunk_lod_falloff"), "set_chunk_lod_falloff", "get_chunk_lod_falloff");

	ClassDB::bind_method(D_METHOD("get_num_lods"), &TerraWorldDefault2D::get_num_lods);
	ClassDB::bind_method(D_METHOD("set_num_lods", "value"), &TerraWorldDefault2D::set_num_lods);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "num_lods"), "set_num_lods", "get_num_lods");

	BIND_VMETHOD(MethodInfo("_update_lods"));
	ClassDB::bind_method(D_METHOD("update_lods"), &TerraWorldDefault2D::update_lods);
	ClassDB::bind_method(D_METHOD("_update_lods"), &TerraWorldDefault2D::_update_lods);

	ClassDB::bind_method(D_METHOD("get_vertex_colors", "transform", "vertices", "base_light_value", "ao_strength"), &TerraWorldDefault2D::get_vertex_colors, DEFVAL(0.45), DEFVAL(0.2));
}