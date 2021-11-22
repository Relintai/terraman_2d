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

#include "terra_chunk_2d.h"

#include "terra_world_2d.h"

#include "../thirdparty/lz4/lz4.h"

#include "../defines.h"

#include "jobs/terra_job_2d.h"
#include "terra_structure_2d.h"

#if THREAD_POOL_PRESENT
#include "../../thread_pool/thread_pool.h"
#endif

_FORCE_INLINE_ bool TerraChunk2D::get_process() const {
	return _is_processing;
}
_FORCE_INLINE_ void TerraChunk2D::set_process(const bool value) {
	_is_processing = value;
}

_FORCE_INLINE_ bool TerraChunk2D::get_physics_process() const {
	return _is_phisics_processing;
}
_FORCE_INLINE_ void TerraChunk2D::set_physics_process(const bool value) {
	_is_phisics_processing = value;
}

bool TerraChunk2D::get_visible() const {
	return _is_visible;
}
void TerraChunk2D::set_visible(const bool value) {
	_is_visible = value;

	visibility_changed(value);
}

_FORCE_INLINE_ bool TerraChunk2D::get_is_generating() const {
	return _is_generating;
}
_FORCE_INLINE_ void TerraChunk2D::set_is_generating(const bool value) {
	_is_generating = value;
}

bool TerraChunk2D::is_in_tree() const {
	return _is_in_tree;
}

_FORCE_INLINE_ bool TerraChunk2D::get_dirty() const {
	return _dirty;
}
_FORCE_INLINE_ void TerraChunk2D::set_dirty(const bool value) {
	_dirty = value;
}

_FORCE_INLINE_ int TerraChunk2D::get_state() const {
	return _state;
}
_FORCE_INLINE_ void TerraChunk2D::set_state(const int value) {
	_state = value;
}

_FORCE_INLINE_ int TerraChunk2D::get_position_x() const {
	return _position_x;
}
void TerraChunk2D::set_position_x(const int value) {
	_position_x = value;
}
_FORCE_INLINE_ int TerraChunk2D::get_position_z() const {
	return _position_z;
}
void TerraChunk2D::set_position_z(const int value) {
	_position_z = value;
}

_FORCE_INLINE_ Vector2 TerraChunk2D::get_position() const {
	return Vector2(_position_x, _position_z);
}
_FORCE_INLINE_ Vector2 TerraChunk2D::get_world_position() const {
	return Vector2(_position_x * _size_x * _voxel_scale, _position_z * _size_z * _voxel_scale);
}

_FORCE_INLINE_ Vector3 TerraChunk2D::get_world_size() const {
	return Vector3(_size_x * _voxel_scale, _world_height * _voxel_scale, _size_z * _voxel_scale);
}

_FORCE_INLINE_ AABB TerraChunk2D::get_world_aabb() const {
	Vector2 v = get_world_position();

	return AABB(Vector3(v.x, 0, v.y), get_world_size());
}

_FORCE_INLINE_ int TerraChunk2D::get_size_x() const {
	return _size_x;
}
_FORCE_INLINE_ int TerraChunk2D::get_size_z() const {
	return _size_z;
}

_FORCE_INLINE_ void TerraChunk2D::set_size_x(const int value) {
	_size_x = value;
}
_FORCE_INLINE_ void TerraChunk2D::set_size_z(const int value) {
	_size_z = value;
}

_FORCE_INLINE_ Vector3 TerraChunk2D::get_size() const {
	return Vector3(_size_x, _world_height, _size_z);
}

_FORCE_INLINE_ int TerraChunk2D::get_data_size_x() const {
	return _data_size_x;
}
_FORCE_INLINE_ int TerraChunk2D::get_data_size_z() const {
	return _data_size_z;
}

_FORCE_INLINE_ void TerraChunk2D::set_data_size_x(const int value) {
	_data_size_x = value;
}
_FORCE_INLINE_ void TerraChunk2D::set_data_size_z(const int value) {
	_data_size_z = value;
}

_FORCE_INLINE_ float TerraChunk2D::get_world_height() const {
	return _world_height;
}
void TerraChunk2D::set_world_height(const float value) {
	_world_height = value;
}

void TerraChunk2D::set_position(const int x, const int z) {
	_position_x = x;
	_position_z = z;
}

_FORCE_INLINE_ int TerraChunk2D::get_margin_start() const {
	return _margin_start;
}
_FORCE_INLINE_ int TerraChunk2D::get_margin_end() const {
	return _margin_end;
}

_FORCE_INLINE_ void TerraChunk2D::set_margin_start(const int value) {
	_margin_start = value;
}
_FORCE_INLINE_ void TerraChunk2D::set_margin_end(const int value) {
	_margin_end = value;
}

int TerraChunk2D::material_cache_key_get() const {
	return _material_cache_key;
}
void TerraChunk2D::material_cache_key_set(const int value) {
	_material_cache_key = value;
}

bool TerraChunk2D::material_cache_key_has() const {
	return _material_cache_key_has;
}
void TerraChunk2D::material_cache_key_has_set(const bool value) {
	_material_cache_key_has = value;
}

int TerraChunk2D::liquid_material_cache_key_get() const {
	return _liquid_material_cache_key;
}
void TerraChunk2D::liquid_material_cache_key_set(const int value) {
	_liquid_material_cache_key = value;
}

bool TerraChunk2D::liquid_material_cache_key_has() const {
	return _liquid_material_cache_key_has;
}
void TerraChunk2D::liquid_material_cache_key_has_set(const bool value) {
	_liquid_material_cache_key_has = value;
}

int TerraChunk2D::prop_material_cache_key_get() const {
	return _prop_material_cache_key;
}
void TerraChunk2D::prop_material_cache_key_set(const int value) {
	_prop_material_cache_key = value;
}

bool TerraChunk2D::prop_material_cache_key_has() const {
	return _prop_material_cache_key_has;
}
void TerraChunk2D::prop_material_cache_key_has_set(const bool value) {
	_prop_material_cache_key_has = value;
}

Ref<TerramanLibrary2D> TerraChunk2D::get_library() {
	return _library;
}
void TerraChunk2D::set_library(const Ref<TerramanLibrary2D> &value) {
	_library = value;
}

float TerraChunk2D::get_voxel_scale() const {
	return _voxel_scale;
}
void TerraChunk2D::set_voxel_scale(const float value) {
	_voxel_scale = value;
}

TerraWorld2D *TerraChunk2D::get_voxel_world() const {
	return _voxel_world;
}
void TerraChunk2D::set_voxel_world(TerraWorld2D *world) {
	_voxel_world = world;
}
void TerraChunk2D::set_voxel_world_bind(Node *world) {
	if (world == NULL) {
		_voxel_world = NULL;
		return;
	}

	_voxel_world = Object::cast_to<TerraWorld2D>(world);
}

Ref<TerraJob2D> TerraChunk2D::job_get(int index) const {
	ERR_FAIL_INDEX_V(index, _jobs.size(), Ref<TerraJob2D>());

	return _jobs.get(index);
}
void TerraChunk2D::job_set(int index, const Ref<TerraJob2D> &job) {
	ERR_FAIL_INDEX(index, _jobs.size());

	_jobs.set(index, job);
}
void TerraChunk2D::job_remove(const int index) {
	ERR_FAIL_INDEX(index, _jobs.size());

	_jobs.remove(index);
}
void TerraChunk2D::job_add(const Ref<TerraJob2D> &job) {
	_jobs.push_back(job);
}
int TerraChunk2D::job_get_count() const {
	return _jobs.size();
}

int TerraChunk2D::job_get_current_index() {
	return _current_job;
}
void TerraChunk2D::job_next() {
	_THREAD_SAFE_METHOD_

	++_current_job;

	if (_current_job >= _jobs.size()) {
		_current_job = -1;
		set_is_generating(false);
		finalize_build();
		return;
	}

	Ref<TerraJob2D> j = _jobs[_current_job];

	if (!j.is_valid()) {
		//skip if invalid
		job_next();
	}

	j->reset();
	j->set_complete(false);

	if (j->get_build_phase_type() == TerraJob2D::BUILD_PHASE_TYPE_NORMAL) {
#if THREAD_POOL_PRESENT
		ThreadPool::get_singleton()->add_job(j);
#else
		j->execute();
#endif
	}
}
Ref<TerraJob2D> TerraChunk2D::job_get_current() {
	_THREAD_SAFE_METHOD_

	if (_current_job < 0 || _current_job >= _jobs.size()) {
		return Ref<TerraJob2D>();
	}

	return _jobs[_current_job];
}

//Terra Data
void TerraChunk2D::channel_setup() {
	ERR_FAIL_COND_MSG(!has_method("_channel_setup"), "TerraChunk2D: _setup_channels() is missing! Please implement it!");

	call("_channel_setup");
}

void TerraChunk2D::set_size(const int size_x, const int size_z, const int margin_start, const int margin_end) {
	if (_size_x == size_x && _size_z == size_z && _margin_start == margin_start && _margin_end == margin_end) {
		return;
	}

	_size_x = size_x;
	_size_z = size_z;

	_data_size_x = size_x + margin_start + margin_end;
	_data_size_z = size_z + margin_start + margin_end;

	_margin_start = margin_start;
	_margin_end = margin_end;

	for (int i = 0; i < _channels.size(); ++i) {
		uint8_t *ch = _channels[i];

		if (ch != NULL) {
			memdelete_arr(ch);
		}
	}

	_channels.clear();

	channel_setup();
}

bool TerraChunk2D::validate_data_position(const int x, const int z) const {
	return x < _data_size_x && z < _data_size_z;
}

uint8_t TerraChunk2D::get_voxel(const int p_x, const int p_z, const int p_channel_index) const {
	int x = p_x + _margin_start;
	int z = p_z + _margin_start;

	ERR_FAIL_INDEX_V(p_channel_index, _channels.size(), 0);
	ERR_FAIL_COND_V_MSG(!validate_data_position(x, z), 0, "Error, index out of range! " + String::num(x) + " " + String::num(z));

	uint8_t *ch = _channels.get(p_channel_index);

	if (!ch)
		return 0;

	return ch[get_data_index(x, z)];
}
void TerraChunk2D::set_voxel(const uint8_t p_value, const int p_x, const int p_z, const int p_channel_index) {
	int x = p_x + _margin_start;
	int z = p_z + _margin_start;

	ERR_FAIL_INDEX(p_channel_index, _channels.size());
	ERR_FAIL_COND_MSG(!validate_data_position(x, z), "Error, index out of range! " + String::num(x) + " " + String::num(z));

	uint8_t *ch = channel_get_valid(p_channel_index);

	ch[get_data_index(x, z)] = p_value;
}

int TerraChunk2D::channel_get_count() const {
	return _channels.size();
}

void TerraChunk2D::channel_set_count(const int count) {
	if (count == _channels.size())
		return;

	if (_channels.size() >= count) {
		for (int i = count; i < _channels.size(); ++i) {
			uint8_t *ch = _channels[i];

			if (ch != NULL) {
				memdelete_arr(ch);
			}

			_channels.set(i, NULL);
		}

		_channels.resize(count);
		return;
	}

	int s = _channels.size();
	_channels.resize(count);

	for (int i = s; i < count; ++i) {
		_channels.set(i, NULL);
	}
}
bool TerraChunk2D::channel_is_allocated(const int channel_index) {
	ERR_FAIL_INDEX_V(channel_index, _channels.size(), false);

	return _channels[channel_index] != NULL;
}
void TerraChunk2D::channel_ensure_allocated(const int channel_index, const uint8_t default_value) {
	ERR_FAIL_INDEX(channel_index, _channels.size());

	if (_channels[channel_index] == NULL)
		channel_allocate(channel_index, default_value);
}
void TerraChunk2D::channel_allocate(const int channel_index, const uint8_t default_value) {
	ERR_FAIL_INDEX(channel_index, _channels.size());

	if (_channels[channel_index] != NULL)
		return;

	uint32_t size = _data_size_x * _data_size_z;

	uint8_t *ch = memnew_arr(uint8_t, size);
	memset(ch, default_value, size);

	_channels.set(channel_index, ch);
}
void TerraChunk2D::channel_fill(const uint8_t value, const int channel_index) {
	ERR_FAIL_INDEX(channel_index, _channels.size());

	uint8_t *ch = _channels.get(channel_index);

	if (ch == NULL) {
		channel_allocate(channel_index, value);
		return;
	}

	uint32_t size = get_data_size();

	for (uint32_t i = 0; i < size; ++i) {
		ch[i] = value;
	}
}
void TerraChunk2D::channel_dealloc(const int channel_index) {
	ERR_FAIL_INDEX(channel_index, _channels.size());

	uint8_t *ch = _channels.get(channel_index);

	if (ch != NULL) {
		memdelete_arr(ch);

		_channels.set(channel_index, NULL);
	}
}

uint8_t *TerraChunk2D::channel_get(const int channel_index) {
	ERR_FAIL_INDEX_V(channel_index, _channels.size(), NULL);

	return _channels.get(channel_index);
}
uint8_t *TerraChunk2D::channel_get_valid(const int channel_index, const uint8_t default_value) {
	ERR_FAIL_INDEX_V(channel_index, _channels.size(), 0);

	uint8_t *ch = _channels.get(channel_index);

	if (ch == NULL) {
		channel_allocate(channel_index, default_value);

		return _channels.get(channel_index);
	}

	return ch;
}

PoolByteArray TerraChunk2D::channel_get_array(const int channel_index) const {
	PoolByteArray arr;

	uint32_t size = _data_size_x * _data_size_z;

	if (channel_index >= _channels.size())
		return arr;

	uint8_t *ch = _channels.get(channel_index);

	if (ch == NULL)
		return arr;

	arr.resize(size);

	for (uint32_t i = 0; i < size; ++i) {
		arr.set(i, ch[i]);
	}

	return arr;
}
void TerraChunk2D::channel_set_array(const int channel_index, const PoolByteArray &array) {
	if (array.size() == 0)
		return;

	if (_channels.size() <= channel_index)
		channel_set_count(channel_index + 1);

	uint8_t *ch = _channels.get(channel_index);

	if (ch == NULL) {
		if (_channels[channel_index] != NULL)
			return;

		ch = memnew_arr(uint8_t, array.size());
		_channels.set(channel_index, ch);
	}

	for (int i = 0; i < array.size(); ++i) {
		ch[i] = array[i];
	}
}

PoolByteArray TerraChunk2D::channel_get_compressed(const int channel_index) const {
	PoolByteArray arr;

	int size = _data_size_x * _data_size_z;

	if (channel_index >= _channels.size())
		return arr;

	uint8_t *ch = _channels.get(channel_index);

	if (ch == NULL)
		return arr;

	int bound = LZ4_compressBound(size);
	arr.resize(bound);

#if !GODOT4
	PoolByteArray::Write w = arr.write();

	int ns = LZ4_compress_default(reinterpret_cast<char *>(ch), reinterpret_cast<char *>(w.ptr()), size, bound);

	w.release();
#else
	int ns = LZ4_compress_default(reinterpret_cast<char *>(ch), reinterpret_cast<char *>(arr.ptrw()), size, bound);
#endif
	arr.resize(ns);

	return arr;
}
void TerraChunk2D::channel_set_compressed(const int channel_index, const PoolByteArray &data) {
	if (data.size() == 0)
		return;

	int size = _data_size_x * _data_size_z;

	if (_channels.size() <= channel_index)
		channel_set_count(channel_index + 1);

	uint8_t *ch = _channels.get(channel_index);

	if (ch == NULL) {
		if (_channels[channel_index] != NULL)
			return;

		ch = memnew_arr(uint8_t, size);
		_channels.set(channel_index, ch);
	}

	int ds = data.size();

#if !GODOT4
	PoolByteArray::Read r = data.read();

	//We are not going to write to it
	uint8_t *data_arr = const_cast<uint8_t *>(r.ptr());

	LZ4_decompress_safe(reinterpret_cast<char *>(data_arr), reinterpret_cast<char *>(ch), ds, size);
#else
	//We are not going to write to it
	uint8_t *data_arr = const_cast<uint8_t *>(data.ptr());

	LZ4_decompress_safe(reinterpret_cast<char *>(data_arr), reinterpret_cast<char *>(ch), ds, size);
#endif
}

_FORCE_INLINE_ int TerraChunk2D::get_index(const int x, const int z) const {
	return ((x + _margin_start) + _data_size_x * (z + _margin_start));
}

_FORCE_INLINE_ int TerraChunk2D::get_data_index(const int x, const int z) const {
	return (x + _data_size_x * z);
}

_FORCE_INLINE_ int TerraChunk2D::get_data_size() const {
	return _data_size_x * _data_size_z;
}

//Terra Structures

Ref<TerraStructure2D> TerraChunk2D::voxel_structure_get(const int index) const {
	ERR_FAIL_INDEX_V(index, _voxel_structures.size(), Ref<TerraStructure2D>());

	return _voxel_structures.get(index);
}
void TerraChunk2D::voxel_structure_add(const Ref<TerraStructure2D> &structure) {
	_voxel_structures.push_back(structure);
}
void TerraChunk2D::voxel_structure_remove(const Ref<TerraStructure2D> &structure) {
	if (!structure.is_valid())
		return;

	int index = _voxel_structures.find(structure);

	if (index != -1)
		_voxel_structures.remove(index);
}
void TerraChunk2D::voxel_structure_remove_index(const int index) {
	ERR_FAIL_INDEX(index, _voxel_structures.size());

	_voxel_structures.remove(index);
}
void TerraChunk2D::voxel_structure_clear() {
	_voxel_structures.clear();
}
int TerraChunk2D::voxel_structure_get_count() const {
	return _voxel_structures.size();
}
void TerraChunk2D::voxel_structure_add_at_position(Ref<TerraStructure2D> structure, const Vector3 &world_position) {
	ERR_FAIL_COND(!structure.is_valid());

	structure->set_position_x(static_cast<int>(world_position.x / _voxel_scale));
	structure->set_position_y(static_cast<int>(world_position.y / _voxel_scale));
	structure->set_position_z(static_cast<int>(world_position.z / _voxel_scale));

	voxel_structure_add(structure);
}

Vector<Variant> TerraChunk2D::voxel_structures_get() {
	VARIANT_ARRAY_GET(_voxel_structures);
}
void TerraChunk2D::voxel_structures_set(const Vector<Variant> &structures) {
	voxel_structure_clear();

	for (int i = 0; i < structures.size(); ++i) {
		Ref<TerraLight2D> structure = Ref<TerraLight2D>(structures[i]);

		voxel_structure_add(structure);
	}
}

void TerraChunk2D::build() {
	ERR_FAIL_COND(!INSTANCE_VALIDATE(get_voxel_world()));
	ERR_FAIL_COND(!get_voxel_world()->is_inside_tree());
	ERR_FAIL_COND(!is_in_tree());

	call("_build");
}

void TerraChunk2D::_build() {
	if (get_is_generating()) {
		_queued_generation = true;
		return;
	}

	_is_generating = true;

	job_next();
}

void TerraChunk2D::clear() {
	ERR_FAIL_COND_MSG(!has_method("_clear"), "TerraChunk2D: _clear() is missing! Please implement it!");

	call("_clear");
}

void TerraChunk2D::finalize_build() {
	if (has_method("_finalize_build")) {
		call("_finalize_build");
	}
}

void TerraChunk2D::bake_lights() {
	if (has_method("_bake_lights"))
		call("_bake_lights");
}
void TerraChunk2D::bake_light(Ref<TerraLight2D> light) {
	if (!light.is_valid())
		return;

	if (has_method("_bake_lights"))
		call("_bake_light", light);
}
void TerraChunk2D::clear_baked_lights() {
	if (has_method("_clear_baked_lights"))
		call("_clear_baked_lights");
}

#if PROPS_PRESENT
void TerraChunk2D::prop_add(const Transform &tarnsform, const Ref<PropData> &prop) {
	ERR_FAIL_COND(!prop.is_valid());

	PropDataStore s;
	s.transform = tarnsform;
	s.prop = prop;

	_props.push_back(s);
}
Ref<PropData> TerraChunk2D::prop_get(int index) {
	ERR_FAIL_INDEX_V(index, _props.size(), Ref<PropData>());

	return _props.get(index).prop;
}
Transform TerraChunk2D::prop_get_tarnsform(const int index) {
	ERR_FAIL_INDEX_V(index, _props.size(), Transform());

	return _props.get(index).transform;
}
int TerraChunk2D::prop_get_count() const {
	return _props.size();
}
void TerraChunk2D::prop_remove(const int index) {
	ERR_FAIL_INDEX(index, _props.size());

	_props.remove(index);
}
void TerraChunk2D::props_clear() {
	_props.clear();
}
#endif

#if MESH_DATA_RESOURCE_PRESENT
int TerraChunk2D::mesh_data_resource_addv(const Vector3 &local_data_pos, const Ref<MeshDataResource> &mesh, const Ref<Texture> &texture, const Color &color, const bool apply_voxel_scale) {
	ERR_FAIL_COND_V(!mesh.is_valid(), 0);

	int index = _mesh_data_resources.size();

	MeshDataResourceEntry e;

	if (apply_voxel_scale) {
		e.transform = Transform(Basis().scaled(Vector3(_voxel_scale, _voxel_scale, _voxel_scale)));
		e.transform.origin = local_data_pos * _voxel_scale;
	} else {
		e.transform.origin = local_data_pos;
	}

	e.mesh = mesh;
	e.texture = texture;
	e.color = color;

	AABB aabb = AABB(Vector3(), get_world_size());
	AABB mesh_aabb = e.transform.xform(mesh->get_aabb());
	e.is_inside = aabb.encloses(mesh_aabb);

#if PROPS_PRESENT
	if (get_library().is_valid() && texture.is_valid()) {
		e.uv_rect = get_library()->get_prop_uv_rect(texture);
	} else {
		e.uv_rect = Rect2(0, 0, 1, 1);
	}
#else
	e.uv_rect = Rect2(0, 0, 1, 1);
#endif

	_mesh_data_resources.push_back(e);

	if (has_method("_mesh_data_resource_added"))
		call("_mesh_data_resource_added", index);

	return index;
}

int TerraChunk2D::mesh_data_resource_add(const Transform &local_transform, const Ref<MeshDataResource> &mesh, const Ref<Texture> &texture, const Color &color, const bool apply_voxel_scale) {
	ERR_FAIL_COND_V(!mesh.is_valid(), 0);

	int index = _mesh_data_resources.size();

	MeshDataResourceEntry e;

	e.transform = local_transform;

	if (apply_voxel_scale) {
		e.transform.basis = e.transform.basis.scaled(Vector3(_voxel_scale, _voxel_scale, _voxel_scale));
		e.transform.origin = e.transform.origin * _voxel_scale;
	}

	e.mesh = mesh;
	e.texture = texture;
	e.color = color;

	AABB aabb = AABB(Vector3(), get_world_size());
	AABB mesh_aabb = e.transform.xform(mesh->get_aabb());
	e.is_inside = aabb.encloses(mesh_aabb);

#if PROPS_PRESENT
	if (get_library().is_valid() && texture.is_valid())
		e.uv_rect = get_library()->get_prop_uv_rect(texture);
	else
		e.uv_rect = Rect2(0, 0, 1, 1);
#else
	e.uv_rect = Rect2(0, 0, 1, 1);
#endif

	_mesh_data_resources.push_back(e);

	if (has_method("_mesh_data_resource_added"))
		call("_mesh_data_resource_added", index);

	return index;
}

Ref<MeshDataResource> TerraChunk2D::mesh_data_resource_get(const int index) {
	ERR_FAIL_INDEX_V(index, _mesh_data_resources.size(), Ref<MeshDataResource>());

	return _mesh_data_resources[index].mesh;
}

void TerraChunk2D::mesh_data_resource_set(const int index, const Ref<MeshDataResource> &mesh) {
	ERR_FAIL_INDEX(index, _mesh_data_resources.size());
}

Ref<Texture> TerraChunk2D::mesh_data_resource_get_texture(const int index) {
	ERR_FAIL_INDEX_V(index, _mesh_data_resources.size(), Ref<Texture>());

	return _mesh_data_resources[index].texture;
}
void TerraChunk2D::mesh_data_resource_set_texture(const int index, const Ref<Texture> &texture) {
	ERR_FAIL_INDEX(index, _mesh_data_resources.size());

	_mesh_data_resources.write[index].texture = texture;
}

Color TerraChunk2D::mesh_data_resource_get_color(const int index) {
	ERR_FAIL_INDEX_V(index, _mesh_data_resources.size(), Color());

	return _mesh_data_resources[index].color;
}
void TerraChunk2D::mesh_data_resource_set_color(const int index, const Color &color) {
	ERR_FAIL_INDEX(index, _mesh_data_resources.size());

	_mesh_data_resources.write[index].color = color;
}

Rect2 TerraChunk2D::mesh_data_resource_get_uv_rect(const int index) {
	ERR_FAIL_INDEX_V(index, _mesh_data_resources.size(), Rect2());

	return _mesh_data_resources[index].uv_rect;
}
void TerraChunk2D::mesh_data_resource_set_uv_rect(const int index, const Rect2 &uv_rect) {
	ERR_FAIL_INDEX(index, _mesh_data_resources.size());

	_mesh_data_resources.write[index].uv_rect = uv_rect;
}

Transform TerraChunk2D::mesh_data_resource_get_transform(const int index) {
	ERR_FAIL_INDEX_V(index, _mesh_data_resources.size(), Transform());

	return _mesh_data_resources.write[index].transform;
}
void TerraChunk2D::mesh_data_resource_set_transform(const int index, const Transform &transform) {
	ERR_FAIL_INDEX(index, _mesh_data_resources.size());

	_mesh_data_resources.write[index].transform = transform;
}

bool TerraChunk2D::mesh_data_resource_get_is_inside(const int index) {
	ERR_FAIL_INDEX_V(index, _mesh_data_resources.size(), true);

	return _mesh_data_resources[index].is_inside;
}
void TerraChunk2D::mesh_data_resource_set_is_inside(const int index, const bool inside) {
	ERR_FAIL_INDEX(index, _mesh_data_resources.size());

	_mesh_data_resources.write[index].is_inside = inside;
}

int TerraChunk2D::mesh_data_resource_get_count() const {
	return _mesh_data_resources.size();
}
void TerraChunk2D::mesh_data_resource_remove(const int index) {
	ERR_FAIL_INDEX(index, _mesh_data_resources.size());

	_mesh_data_resources.remove(index);
}
void TerraChunk2D::mesh_data_resource_clear() {
	_mesh_data_resources.clear();
}

#endif

int TerraChunk2D::collider_add(const Transform &local_transform, const Ref<Shape> &shape, const RID &shape_rid, const RID &body) {
	ERR_FAIL_COND_V(!shape.is_valid() && shape_rid == RID(), 0);

	int index = _colliders.size();

	ColliderBody e;
	e.transform = local_transform;
	e.body = body;
	e.shape = shape;
	e.shape_rid = shape_rid;

	_colliders.push_back(e);

	return index;
}

Transform TerraChunk2D::collider_get_transform(const int index) {
	ERR_FAIL_INDEX_V(index, _colliders.size(), Transform());

	return _colliders[index].transform;
}
void TerraChunk2D::collider_set_transform(const int index, const Transform &transform) {
	ERR_FAIL_INDEX(index, _colliders.size());

	_colliders.write[index].transform = transform;
}

Ref<Shape> TerraChunk2D::collider_get_shape(const int index) {
	ERR_FAIL_INDEX_V(index, _colliders.size(), Ref<Shape>());

	return _colliders[index].shape;
}

void TerraChunk2D::collider_set_shape(const int index, const Ref<Shape> &shape) {
	ERR_FAIL_INDEX(index, _colliders.size());

	_colliders.write[index].shape = shape;
}

RID TerraChunk2D::collider_get_shape_rid(const int index) {
	ERR_FAIL_INDEX_V(index, _colliders.size(), RID());

	return _colliders[index].shape_rid;
}
void TerraChunk2D::collider_set_shape_rid(const int index, const RID &rid) {
	ERR_FAIL_INDEX(index, _colliders.size());

	_colliders.write[index].shape_rid = rid;
}

RID TerraChunk2D::collider_get_body(const int index) {
	ERR_FAIL_INDEX_V(index, _colliders.size(), RID());

	return _colliders[index].body;
}
void TerraChunk2D::collider_set_body(const int index, const RID &rid) {
	ERR_FAIL_INDEX(index, _colliders.size());

	_colliders.write[index].body = rid;
}

int TerraChunk2D::collider_get_count() const {
	return _colliders.size();
}
void TerraChunk2D::collider_remove(const int index) {
	ERR_FAIL_INDEX(index, _colliders.size());

	_colliders.remove(index);
}
void TerraChunk2D::colliders_clear() {
	_colliders.clear();
}

void TerraChunk2D::enter_tree() {
	_is_in_tree = true;

	if (has_method("_enter_tree"))
		call("_enter_tree");
}
void TerraChunk2D::exit_tree() {
	_is_in_tree = false;

	if (_library.is_valid() && _library->supports_caching()) {
		if (material_cache_key_has()) {
			_library->material_cache_unref(material_cache_key_get());
		}
	}

	if (has_method("_exit_tree"))
		call("_exit_tree");
}
void TerraChunk2D::process(const float delta) {
	if (has_method("_process"))
		call("_process", delta);
}
void TerraChunk2D::physics_process(const float delta) {
	if (has_method("_physics_process"))
		call("_physics_process", delta);
}
void TerraChunk2D::world_transform_changed() {
	call("_world_transform_changed");
}
void TerraChunk2D::visibility_changed(const bool visible) {
	if (has_method("_visibility_changed"))
		call("_visibility_changed", _is_visible);
}
void TerraChunk2D::world_light_added(const Ref<TerraLight2D> &light) {
	if (has_method("_world_light_added"))
		call("_world_light_added", light);
}
void TerraChunk2D::world_light_removed(const Ref<TerraLight2D> &light) {
	if (has_method("_world_light_removed"))
		call("_world_light_removed", light);
}
void TerraChunk2D::generation_process(const float delta) {
	call("_generation_process", delta);
}
void TerraChunk2D::generation_physics_process(const float delta) {
	call("_generation_physics_process", delta);
}

Transform TerraChunk2D::get_transform() const {
	return _transform;
}
void TerraChunk2D::set_transform(const Transform &transform) {
	_transform = transform;
}

Transform TerraChunk2D::get_global_transform() const {
	ERR_FAIL_COND_V(!get_voxel_world(), Transform());

	return get_voxel_world()->get_global_transform() * _transform;
}

Vector3 TerraChunk2D::to_local(Vector3 p_global) const {
	return get_global_transform().affine_inverse().xform(p_global);
}

Vector3 TerraChunk2D::to_global(Vector3 p_local) const {
	return get_global_transform().xform(p_local);
}

TerraChunk2D::TerraChunk2D() {
	_is_processing = false;
	_is_phisics_processing = false;
	_is_in_tree = false;

	_is_visible = true;

	_is_generating = false;
	_dirty = false;
	_state = TERRA_CHUNK_STATE_OK;

	_voxel_scale = 1;

	_voxel_world = NULL;

	_position_x = 0;
	_position_z = 0;

	_size_x = 0;
	_size_z = 0;

	_data_size_x = 0;
	_data_size_z = 0;

	_margin_start = 0;
	_margin_end = 0;

	_material_cache_key = 0;
	_material_cache_key_has = false;

	_liquid_material_cache_key = 0;
	_liquid_material_cache_key_has = false;

	_prop_material_cache_key = 0;
	_prop_material_cache_key_has = false;

	_current_job = -1;

	_world_height = 256;

	_queued_generation = false;
}

TerraChunk2D::~TerraChunk2D() {
	if (_library.is_valid()) {
		_library.unref();
	}

#if PROPS_PRESENT
	props_clear();
#endif

#if MESH_DATA_RESOURCE_PRESENT
	mesh_data_resource_clear();
#endif

	for (int i = 0; i < _channels.size(); ++i) {
		uint8_t *ch = _channels[i];

		if (ch != NULL) {
			memdelete_arr(ch);
		}
	}

	for (int i = 0; i < _colliders.size(); ++i) {
		PhysicsServer::get_singleton()->free(_colliders[i].body);
	}

	_colliders.clear();

	_jobs.clear();
}

void TerraChunk2D::_enter_tree() {
	for (int i = 0; i < _jobs.size(); ++i) {
		Ref<TerraJob2D> j = _jobs[i];

		if (j.is_valid()) {
			j->set_chunk(Ref<TerraChunk2D>(this));
		}
	}
}

void TerraChunk2D::_exit_tree() {
	_abort_build = true;

	for (int i = 0; i < _jobs.size(); ++i) {
		Ref<TerraJob2D> j = _jobs[i];

		if (j.is_valid()) {
			j->chunk_exit_tree();
		}
	}
}

void TerraChunk2D::_generation_process(const float delta) {
	_THREAD_SAFE_METHOD_

	if (_current_job < 0 || _current_job >= _jobs.size())
		return;

	Ref<TerraJob2D> job = _jobs[_current_job];

	ERR_FAIL_COND(!job.is_valid());

	if (job->get_build_phase_type() == TerraJob2D::BUILD_PHASE_TYPE_PROCESS) {
		if (!_voxel_world->can_chunk_do_build_step())
			return;

		job->process(delta);

		if (job->get_build_phase_type() == TerraJob2D::BUILD_PHASE_TYPE_NORMAL) {
#if THREAD_POOL_PRESENT
			ThreadPool::get_singleton()->add_job(job);
#else
			job->execute();
#endif
		}
	}
}
void TerraChunk2D::_generation_physics_process(const float delta) {
	_THREAD_SAFE_METHOD_

	if (_current_job < 0 || _current_job >= _jobs.size())
		return;

	Ref<TerraJob2D> job = _jobs[_current_job];

	ERR_FAIL_COND(!job.is_valid());

	if (job->get_build_phase_type() == TerraJob2D::BUILD_PHASE_TYPE_PHYSICS_PROCESS) {
		if (!_voxel_world->can_chunk_do_build_step())
			return;

		job->physics_process(delta);

		if (job->get_build_phase_type() == TerraJob2D::BUILD_PHASE_TYPE_NORMAL) {
#if THREAD_POOL_PRESENT
			ThreadPool::get_singleton()->add_job(job);
#else
			job->execute();
#endif
		}
	}
}

void TerraChunk2D::_world_transform_changed() {
	Transform wt;

	if (_voxel_world != NULL) {
		wt = _voxel_world->get_transform();
	}

	set_transform(wt * Transform(Basis(), Vector3(_position_x * static_cast<int>(_size_x) * _voxel_scale, 0, _position_z * static_cast<int>(_size_z) * _voxel_scale)));
}

/*
bool TerraChunk2D::_set(const StringName &p_name, const Variant &p_value) {
	String name = p_name;

	if (name.begins_with("channels/")) {

		int index = name.get_slicec('/', 1).to_int();

		if (_channels.size() <= index) {
			set_channel_count(index);
		}

		PoolByteArray arr = p_value;

		if (arr.size() == 0)
			return true;

		set_channel_array(index, arr);
	}

	return true;
}

bool TerraChunk2D::_get(const StringName &p_name, Variant &r_ret) const {
	String name = p_name;

	if (name.begins_with("channels/")) {

		int index = name.get_slicec('/', 1).to_int();

		r_ret = get_channel_array(index);

		return true;
	}

	return false;
}

void TerraChunk2D::_get_property_list(List<PropertyInfo> *p_list) const {
	for (int i = 0; i < _channels.size(); ++i) {
		p_list->push_back(PropertyInfo(Variant::POOL_BYTE_ARRAY, "channels/" + String::num(i), PROPERTY_HINT_NONE, "", PROPERTY_USAGE_STORAGE | PROPERTY_USAGE_INTERNAL));
	}
}
*/

void TerraChunk2D::_bind_methods() {
	ADD_SIGNAL(MethodInfo("mesh_generation_finished", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "TerraChunk2D")));

	BIND_VMETHOD(MethodInfo("_mesh_data_resource_added", PropertyInfo(Variant::INT, "index")));

	BIND_VMETHOD(MethodInfo("_channel_setup"));

	BIND_VMETHOD(MethodInfo("_bake_lights"));
	BIND_VMETHOD(MethodInfo("_bake_light", PropertyInfo(Variant::OBJECT, "light", PROPERTY_HINT_RESOURCE_TYPE, "TerraLight2D")));
	BIND_VMETHOD(MethodInfo("_clear_baked_lights"));

	ClassDB::bind_method(D_METHOD("bake_lights"), &TerraChunk2D::bake_lights);
	ClassDB::bind_method(D_METHOD("bake_light", "light"), &TerraChunk2D::bake_light);
	ClassDB::bind_method(D_METHOD("clear_baked_lights"), &TerraChunk2D::clear_baked_lights);

	BIND_VMETHOD(MethodInfo("_enter_tree"));
	BIND_VMETHOD(MethodInfo("_exit_tree"));
	BIND_VMETHOD(MethodInfo("_process", PropertyInfo(Variant::REAL, "delta")));
	BIND_VMETHOD(MethodInfo("_physics_process", PropertyInfo(Variant::REAL, "delta")));
	BIND_VMETHOD(MethodInfo("_world_transform_changed"));
	BIND_VMETHOD(MethodInfo("_visibility_changed", PropertyInfo(Variant::BOOL, "visible")));
	BIND_VMETHOD(MethodInfo("_world_light_added", PropertyInfo(Variant::OBJECT, "light", PROPERTY_HINT_RESOURCE_TYPE, "TerraLight2D")));
	BIND_VMETHOD(MethodInfo("_world_light_removed", PropertyInfo(Variant::OBJECT, "light", PROPERTY_HINT_RESOURCE_TYPE, "TerraLight2D")));

	BIND_VMETHOD(MethodInfo("_generation_process", PropertyInfo(Variant::REAL, "delta")));
	BIND_VMETHOD(MethodInfo("_generation_physics_process", PropertyInfo(Variant::REAL, "delta")));

	BIND_VMETHOD(MethodInfo("_finalize_build"));

	ClassDB::bind_method(D_METHOD("enter_tree"), &TerraChunk2D::enter_tree);
	ClassDB::bind_method(D_METHOD("exit_tree"), &TerraChunk2D::exit_tree);
	ClassDB::bind_method(D_METHOD("process", "delta"), &TerraChunk2D::process);
	ClassDB::bind_method(D_METHOD("physics_process", "delta"), &TerraChunk2D::physics_process);
	ClassDB::bind_method(D_METHOD("world_transform_changed"), &TerraChunk2D::world_transform_changed);
	ClassDB::bind_method(D_METHOD("visibility_changed", "visible"), &TerraChunk2D::visibility_changed);
	ClassDB::bind_method(D_METHOD("world_light_added", "light"), &TerraChunk2D::world_light_added);
	ClassDB::bind_method(D_METHOD("world_light_removed", "light"), &TerraChunk2D::world_light_removed);

	ClassDB::bind_method(D_METHOD("generation_process", "delta"), &TerraChunk2D::generation_process);
	ClassDB::bind_method(D_METHOD("generation_physics_process", "delta"), &TerraChunk2D::generation_physics_process);

	ClassDB::bind_method(D_METHOD("finalize_build"), &TerraChunk2D::finalize_build);

	ClassDB::bind_method(D_METHOD("get_process"), &TerraChunk2D::get_process);
	ClassDB::bind_method(D_METHOD("set_process", "value"), &TerraChunk2D::set_process);

	ClassDB::bind_method(D_METHOD("get_physics_process"), &TerraChunk2D::get_physics_process);
	ClassDB::bind_method(D_METHOD("set_physics_process", "value"), &TerraChunk2D::set_physics_process);

	ClassDB::bind_method(D_METHOD("is_in_tree"), &TerraChunk2D::is_in_tree);

	ClassDB::bind_method(D_METHOD("get_transform"), &TerraChunk2D::get_transform);
	ClassDB::bind_method(D_METHOD("set_transform", "value"), &TerraChunk2D::set_transform);
	ADD_PROPERTY(PropertyInfo(Variant::TRANSFORM, "transform"), "set_transform", "get_transform");

	ClassDB::bind_method(D_METHOD("get_visible"), &TerraChunk2D::get_visible);
	ClassDB::bind_method(D_METHOD("set_visible", "value"), &TerraChunk2D::set_visible);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "visible"), "set_visible", "get_visible");

	ClassDB::bind_method(D_METHOD("get_is_generating"), &TerraChunk2D::get_is_generating);
	ClassDB::bind_method(D_METHOD("set_is_generating", "value"), &TerraChunk2D::set_is_generating);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_generating", PROPERTY_HINT_NONE, "", 0), "set_is_generating", "get_is_generating");

	ClassDB::bind_method(D_METHOD("get_dirty"), &TerraChunk2D::get_dirty);
	ClassDB::bind_method(D_METHOD("set_dirty", "value"), &TerraChunk2D::set_dirty);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "dirty", PROPERTY_HINT_NONE, "", 0), "set_dirty", "get_dirty");

	ClassDB::bind_method(D_METHOD("get_state"), &TerraChunk2D::get_state);
	ClassDB::bind_method(D_METHOD("set_state", "value"), &TerraChunk2D::set_state);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "state", PROPERTY_HINT_NONE, "", 0), "set_state", "get_state");

	ClassDB::bind_method(D_METHOD("get_position_x"), &TerraChunk2D::get_position_x);
	ClassDB::bind_method(D_METHOD("set_position_x", "value"), &TerraChunk2D::set_position_x);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "position_x"), "set_position_x", "get_position_x");

	ClassDB::bind_method(D_METHOD("get_position_z"), &TerraChunk2D::get_position_z);
	ClassDB::bind_method(D_METHOD("set_position_z", "value"), &TerraChunk2D::set_position_z);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "position_z"), "set_position_z", "get_position_z");

	ClassDB::bind_method(D_METHOD("get_size_x"), &TerraChunk2D::get_size_x);
	ClassDB::bind_method(D_METHOD("set_size_x"), &TerraChunk2D::set_size_x);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "size_x"), "set_size_x", "get_size_x");

	ClassDB::bind_method(D_METHOD("get_size_z"), &TerraChunk2D::get_size_z);
	ClassDB::bind_method(D_METHOD("set_size_z"), &TerraChunk2D::set_size_z);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "size_z"), "set_size_z", "get_size_z");

	ClassDB::bind_method(D_METHOD("get_data_size_x"), &TerraChunk2D::get_data_size_x);
	ClassDB::bind_method(D_METHOD("set_data_size_x"), &TerraChunk2D::set_data_size_x);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "data_size_x"), "set_data_size_x", "get_data_size_x");

	ClassDB::bind_method(D_METHOD("get_data_size_z"), &TerraChunk2D::get_data_size_z);
	ClassDB::bind_method(D_METHOD("set_data_size_z"), &TerraChunk2D::set_data_size_z);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "data_size_z"), "set_data_size_z", "get_data_size_z");

	ClassDB::bind_method(D_METHOD("get_world_height"), &TerraChunk2D::get_world_height);
	ClassDB::bind_method(D_METHOD("set_world_height", "height"), &TerraChunk2D::set_world_height);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "world_height"), "set_world_height", "get_world_height");

	ClassDB::bind_method(D_METHOD("get_position"), &TerraChunk2D::get_position);
	ClassDB::bind_method(D_METHOD("set_position", "x", "z"), &TerraChunk2D::set_position);

	ClassDB::bind_method(D_METHOD("get_world_position"), &TerraChunk2D::get_world_position);
	ClassDB::bind_method(D_METHOD("get_world_size"), &TerraChunk2D::get_world_size);
	ClassDB::bind_method(D_METHOD("get_world_aabb"), &TerraChunk2D::get_world_aabb);

	ClassDB::bind_method(D_METHOD("get_margin_start"), &TerraChunk2D::get_margin_start);
	ClassDB::bind_method(D_METHOD("set_margin_start"), &TerraChunk2D::set_margin_start);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "margin_start"), "set_margin_start", "get_margin_start");

	ClassDB::bind_method(D_METHOD("get_margin_end"), &TerraChunk2D::get_margin_end);
	ClassDB::bind_method(D_METHOD("set_margin_end"), &TerraChunk2D::set_margin_end);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "margin_end"), "set_margin_end", "get_margin_end");

	ClassDB::bind_method(D_METHOD("material_cache_key_get"), &TerraChunk2D::material_cache_key_get);
	ClassDB::bind_method(D_METHOD("material_cache_key_set"), &TerraChunk2D::material_cache_key_set);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "material_cache_key"), "material_cache_key_set", "material_cache_key_get");

	ClassDB::bind_method(D_METHOD("material_cache_key_has"), &TerraChunk2D::material_cache_key_has);
	ClassDB::bind_method(D_METHOD("material_cache_key_has_set"), &TerraChunk2D::material_cache_key_has_set);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "material_cache_key_has"), "material_cache_key_has_set", "material_cache_key_has");

	ClassDB::bind_method(D_METHOD("liquid_material_cache_key_get"), &TerraChunk2D::liquid_material_cache_key_get);
	ClassDB::bind_method(D_METHOD("liquid_material_cache_key_set"), &TerraChunk2D::liquid_material_cache_key_set);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "liquid_material_cache_key"), "liquid_material_cache_key_set", "liquid_material_cache_key_get");

	ClassDB::bind_method(D_METHOD("liquid_material_cache_key_has"), &TerraChunk2D::liquid_material_cache_key_has);
	ClassDB::bind_method(D_METHOD("liquid_material_cache_key_has_set"), &TerraChunk2D::liquid_material_cache_key_has_set);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "liquid_material_cache_key_has"), "liquid_material_cache_key_has_set", "liquid_material_cache_key_has");

	ClassDB::bind_method(D_METHOD("prop_material_cache_key_get"), &TerraChunk2D::prop_material_cache_key_get);
	ClassDB::bind_method(D_METHOD("prop_material_cache_key_set"), &TerraChunk2D::prop_material_cache_key_set);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "prop_material_cache_key"), "prop_material_cache_key_set", "prop_material_cache_key_get");

	ClassDB::bind_method(D_METHOD("prop_material_cache_key_has"), &TerraChunk2D::prop_material_cache_key_has);
	ClassDB::bind_method(D_METHOD("prop_material_cache_key_has_set"), &TerraChunk2D::prop_material_cache_key_has_set);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "prop_material_cache_key_has"), "prop_material_cache_key_has_set", "prop_material_cache_key_has");

	ClassDB::bind_method(D_METHOD("get_library"), &TerraChunk2D::get_library);
	ClassDB::bind_method(D_METHOD("set_library", "value"), &TerraChunk2D::set_library);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "library", PROPERTY_HINT_RESOURCE_TYPE, "TerramanLibrary2D"), "set_library", "get_library");

	ClassDB::bind_method(D_METHOD("get_voxel_scale"), &TerraChunk2D::get_voxel_scale);
	ClassDB::bind_method(D_METHOD("set_voxel_scale", "value"), &TerraChunk2D::set_voxel_scale);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "voxel_scale"), "set_voxel_scale", "get_voxel_scale");

	ClassDB::bind_method(D_METHOD("job_get", "index"), &TerraChunk2D::job_get);
	ClassDB::bind_method(D_METHOD("job_set", "index", "job"), &TerraChunk2D::job_set);
	ClassDB::bind_method(D_METHOD("job_remove", "index"), &TerraChunk2D::job_remove);
	ClassDB::bind_method(D_METHOD("job_add", "job"), &TerraChunk2D::job_add);
	ClassDB::bind_method(D_METHOD("job_get_count"), &TerraChunk2D::job_get_count);

	ClassDB::bind_method(D_METHOD("job_get_current_index"), &TerraChunk2D::job_get_current_index);
	ClassDB::bind_method(D_METHOD("job_next"), &TerraChunk2D::job_next);
	ClassDB::bind_method(D_METHOD("job_get_current"), &TerraChunk2D::job_get_current);

	ClassDB::bind_method(D_METHOD("get_voxel_world"), &TerraChunk2D::get_voxel_world);
	ClassDB::bind_method(D_METHOD("set_voxel_world", "world"), &TerraChunk2D::set_voxel_world_bind);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "voxel_world", PROPERTY_HINT_RESOURCE_TYPE, "TerraWorld2D", 0), "set_voxel_world", "get_voxel_world");

	//Terra Data
	ClassDB::bind_method(D_METHOD("channel_setup"), &TerraChunk2D::channel_setup);

	ClassDB::bind_method(D_METHOD("set_size", "size_x", "size_z", "margin_start", "margin_end"), &TerraChunk2D::set_size, DEFVAL(0), DEFVAL(0));

	ClassDB::bind_method(D_METHOD("validate_data_position", "x", "z"), &TerraChunk2D::validate_data_position);

	ClassDB::bind_method(D_METHOD("get_voxel", "x", "z", "index"), &TerraChunk2D::get_voxel);
	ClassDB::bind_method(D_METHOD("set_voxel", "value", "x", "z", "index"), &TerraChunk2D::set_voxel);

	ClassDB::bind_method(D_METHOD("channel_get_count"), &TerraChunk2D::channel_get_count);
	ClassDB::bind_method(D_METHOD("channel_set_count", "count"), &TerraChunk2D::channel_set_count);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "channel_count"), "channel_set_count", "channel_get_count");

	ClassDB::bind_method(D_METHOD("channel_is_allocated", "index"), &TerraChunk2D::channel_is_allocated);
	ClassDB::bind_method(D_METHOD("channel_ensure_allocated", "index", "default_value"), &TerraChunk2D::channel_ensure_allocated);
	ClassDB::bind_method(D_METHOD("channel_allocate", "index", "default_value"), &TerraChunk2D::channel_allocate);
	ClassDB::bind_method(D_METHOD("channel_fill", "value", "index"), &TerraChunk2D::channel_fill);
	ClassDB::bind_method(D_METHOD("channel_dealloc", "index"), &TerraChunk2D::channel_dealloc);

	ClassDB::bind_method(D_METHOD("channel_get_array", "index"), &TerraChunk2D::channel_get_array);
	ClassDB::bind_method(D_METHOD("channel_set_array", "index", "array"), &TerraChunk2D::channel_set_array);

	ClassDB::bind_method(D_METHOD("channel_get_compressed", "index"), &TerraChunk2D::channel_get_compressed);
	ClassDB::bind_method(D_METHOD("channel_set_compressed", "index", "array"), &TerraChunk2D::channel_set_compressed);

	ClassDB::bind_method(D_METHOD("get_index", "x", "z"), &TerraChunk2D::get_index);
	ClassDB::bind_method(D_METHOD("get_data_index", "x", "z"), &TerraChunk2D::get_data_index);
	ClassDB::bind_method(D_METHOD("get_data_size"), &TerraChunk2D::get_data_size);

	ClassDB::bind_method(D_METHOD("voxel_structure_get", "index"), &TerraChunk2D::voxel_structure_get);
	ClassDB::bind_method(D_METHOD("voxel_structure_add", "structure"), &TerraChunk2D::voxel_structure_add);
	ClassDB::bind_method(D_METHOD("voxel_structure_remove", "structure"), &TerraChunk2D::voxel_structure_remove);
	ClassDB::bind_method(D_METHOD("voxel_structure_remove_index", "index"), &TerraChunk2D::voxel_structure_remove_index);
	ClassDB::bind_method(D_METHOD("voxel_structure_clear"), &TerraChunk2D::voxel_structure_clear);
	ClassDB::bind_method(D_METHOD("voxel_structure_get_count"), &TerraChunk2D::voxel_structure_get_count);
	ClassDB::bind_method(D_METHOD("voxel_structure_add_at_position", "structure", "world_position"), &TerraChunk2D::voxel_structure_add_at_position);

	ClassDB::bind_method(D_METHOD("voxel_structures_get"), &TerraChunk2D::voxel_structures_get);
	ClassDB::bind_method(D_METHOD("voxel_structures_set"), &TerraChunk2D::voxel_structures_set);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "voxel_structures", PROPERTY_HINT_NONE, "17/17:TerraStructure2D", PROPERTY_USAGE_DEFAULT, "TerraStructure2D"), "voxel_structures_set", "voxel_structures_get");

	//Meshes

#if PROPS_PRESENT
	ClassDB::bind_method(D_METHOD("prop_add", "prop"), &TerraChunk2D::prop_add);
	ClassDB::bind_method(D_METHOD("prop_get", "index"), &TerraChunk2D::prop_get);
	ClassDB::bind_method(D_METHOD("prop_get_count"), &TerraChunk2D::prop_get_count);
	ClassDB::bind_method(D_METHOD("prop_remove", "index"), &TerraChunk2D::prop_remove);
	ClassDB::bind_method(D_METHOD("props_clear"), &TerraChunk2D::props_clear);
#endif

#if MESH_DATA_RESOURCE_PRESENT
	ClassDB::bind_method(D_METHOD("mesh_data_resource_addv", "local_data_pos", "mesh", "texture", "color", "apply_voxel_scale"), &TerraChunk2D::mesh_data_resource_addv, DEFVAL(Ref<Texture>()), DEFVAL(Color(1, 1, 1, 1)), DEFVAL(true));
	ClassDB::bind_method(D_METHOD("mesh_data_resource_add", "local_transform", "mesh", "texture", "color", "apply_voxel_scale"), &TerraChunk2D::mesh_data_resource_add, DEFVAL(Ref<Texture>()), DEFVAL(Color(1, 1, 1, 1)), DEFVAL(true));

	ClassDB::bind_method(D_METHOD("mesh_data_resource_get", "index"), &TerraChunk2D::mesh_data_resource_get);
	ClassDB::bind_method(D_METHOD("mesh_data_resource_set", "index", "mesh"), &TerraChunk2D::mesh_data_resource_set);

	ClassDB::bind_method(D_METHOD("mesh_data_resource_get_texture", "index"), &TerraChunk2D::mesh_data_resource_get_texture);
	ClassDB::bind_method(D_METHOD("mesh_data_resource_set_texture", "index", "texture"), &TerraChunk2D::mesh_data_resource_set_texture);

	ClassDB::bind_method(D_METHOD("mesh_data_resource_get_color", "index"), &TerraChunk2D::mesh_data_resource_get_color);
	ClassDB::bind_method(D_METHOD("mesh_data_resource_set_color", "index", "color"), &TerraChunk2D::mesh_data_resource_set_color);

	ClassDB::bind_method(D_METHOD("mesh_data_resource_get_uv_rect", "index"), &TerraChunk2D::mesh_data_resource_get_uv_rect);
	ClassDB::bind_method(D_METHOD("mesh_data_resource_set_uv_rect", "index", "uv_rect"), &TerraChunk2D::mesh_data_resource_set_uv_rect);

	ClassDB::bind_method(D_METHOD("mesh_data_resource_get_transform", "index"), &TerraChunk2D::mesh_data_resource_get_transform);
	ClassDB::bind_method(D_METHOD("mesh_data_resource_set_transform", "index", "transform"), &TerraChunk2D::mesh_data_resource_set_transform);

	ClassDB::bind_method(D_METHOD("mesh_data_resource_get_is_inside", "index"), &TerraChunk2D::mesh_data_resource_get_is_inside);
	ClassDB::bind_method(D_METHOD("mesh_data_resource_set_is_inside", "index", "inside"), &TerraChunk2D::mesh_data_resource_set_is_inside);

	ClassDB::bind_method(D_METHOD("mesh_data_resource_get_count"), &TerraChunk2D::mesh_data_resource_get_count);
	ClassDB::bind_method(D_METHOD("mesh_data_resource_remove", "index"), &TerraChunk2D::mesh_data_resource_remove);
	ClassDB::bind_method(D_METHOD("mesh_data_resource_clear"), &TerraChunk2D::mesh_data_resource_clear);
#endif

	ClassDB::bind_method(D_METHOD("collider_add", "local_transform", "shape", "shape_rid", "body"), &TerraChunk2D::collider_add, DEFVAL(RID()), DEFVAL(RID()));

	ClassDB::bind_method(D_METHOD("collider_get_transform", "index"), &TerraChunk2D::collider_get_transform);
	ClassDB::bind_method(D_METHOD("collider_set_transform", "index", "transform"), &TerraChunk2D::collider_set_transform);

	ClassDB::bind_method(D_METHOD("collider_get_shape", "index"), &TerraChunk2D::collider_get_shape);
	ClassDB::bind_method(D_METHOD("collider_set_shape", "index", "shape"), &TerraChunk2D::collider_set_shape);

	ClassDB::bind_method(D_METHOD("collider_get_shape_rid", "index"), &TerraChunk2D::collider_get_shape_rid);
	ClassDB::bind_method(D_METHOD("collider_set_shape_rid", "index", "rid"), &TerraChunk2D::collider_set_shape_rid);

	ClassDB::bind_method(D_METHOD("collider_get_body", "index"), &TerraChunk2D::collider_get_body);
	ClassDB::bind_method(D_METHOD("collider_set_body", "index", "rid"), &TerraChunk2D::collider_set_body);

	ClassDB::bind_method(D_METHOD("collider_get_count"), &TerraChunk2D::collider_get_count);
	ClassDB::bind_method(D_METHOD("collider_remove", "index"), &TerraChunk2D::collider_remove);
	ClassDB::bind_method(D_METHOD("colliders_clear"), &TerraChunk2D::colliders_clear);

	BIND_VMETHOD(MethodInfo("_build"));
	ClassDB::bind_method(D_METHOD("build"), &TerraChunk2D::build);
	ClassDB::bind_method(D_METHOD("_build"), &TerraChunk2D::_build);

	ClassDB::bind_method(D_METHOD("get_global_transform"), &TerraChunk2D::get_global_transform);
	ClassDB::bind_method(D_METHOD("to_local", "global"), &TerraChunk2D::to_local);
	ClassDB::bind_method(D_METHOD("to_global", "local"), &TerraChunk2D::to_global);

	ClassDB::bind_method(D_METHOD("_world_transform_changed"), &TerraChunk2D::_world_transform_changed);
	ClassDB::bind_method(D_METHOD("_enter_tree"), &TerraChunk2D::_enter_tree);
	ClassDB::bind_method(D_METHOD("_exit_tree"), &TerraChunk2D::_exit_tree);

	ClassDB::bind_method(D_METHOD("_generation_process"), &TerraChunk2D::_generation_process);
	ClassDB::bind_method(D_METHOD("_generation_physics_process"), &TerraChunk2D::_generation_physics_process);
}
