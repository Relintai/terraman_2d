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

#include "terra_job_2d.h"

#include "../default/terra_chunk_default_2d.h"

#include "../../../opensimplex/open_simplex_noise.h"

const String TerraJob2D::BINDING_STRING_ACTIVE_BUILD_PHASE_TYPE = "Normal,Process,Physics Process";

TerraJob2D::ActiveBuildPhaseType TerraJob2D::get_build_phase_type() {
	return _build_phase_type;
}
void TerraJob2D::set_build_phase_type(TerraJob2D::ActiveBuildPhaseType build_phase_type) {
	_build_phase_type = build_phase_type;
}

void TerraJob2D::set_chunk(const Ref<TerraChunk2D> &chunk) {
	_chunk = chunk;

	_in_tree = true;
}

int TerraJob2D::get_phase() {
	return _phase;
}
void TerraJob2D::set_phase(const int phase) {
	_phase = phase;
}
void TerraJob2D::next_phase() {
	++_phase;
}

bool TerraJob2D::get_build_done() {
	return _build_done;
}
void TerraJob2D::set_build_done(const bool val) {
	_build_done = val;
}

void TerraJob2D::next_job() {
	_chunk->job_next();
	set_build_done(true);
}

void TerraJob2D::reset() {
	call("_reset");
}
void TerraJob2D::_reset() {
	_build_done = false;
	_phase = 0;
}

void TerraJob2D::_execute() {

	ActiveBuildPhaseType origpt = _build_phase_type;

	while (!get_cancelled() && _in_tree && !_build_done && origpt == _build_phase_type && !should_return()) {
		execute_phase();
	}

	if (!_in_tree) {
		_chunk.unref();
	}
}

void TerraJob2D::execute_phase() {
	call("_execute_phase");
}

void TerraJob2D::_execute_phase() {
	next_job();
}

void TerraJob2D::process(const float delta) {
	if (has_method("_process"))
		call("_process", delta);
}
void TerraJob2D::physics_process(const float delta) {
	if (has_method("_physics_process"))
		call("_physics_process", delta);
}

//Data Management functions
void TerraJob2D::generate_ao() {
	ERR_FAIL_COND(!_chunk.is_valid());

	int data_size_x = _chunk->get_data_size_x();
	int data_size_z = _chunk->get_data_size_z();

	ERR_FAIL_COND(data_size_x == 0 || data_size_z == 0);

	int margin_start = _chunk->get_margin_start();
	int margin_end = _chunk->get_margin_end();

	int ssize_x = _chunk->get_size_x();
	int ssize_z = _chunk->get_size_z();

	int size_x = ssize_x + margin_end;
	int size_z = ssize_z + margin_end;

	for (int z = margin_start - 1; z < size_z - 1; ++z) {
		for (int x = margin_start - 1; x < size_x - 1; ++x) {
			int current = _chunk->get_voxel(x, z, TerraChunkDefault2D::DEFAULT_CHANNEL_ISOLEVEL);

			int sum = _chunk->get_voxel(x + 1, z, TerraChunkDefault2D::DEFAULT_CHANNEL_ISOLEVEL);
			sum += _chunk->get_voxel(x - 1, z, TerraChunkDefault2D::DEFAULT_CHANNEL_ISOLEVEL);
			sum += _chunk->get_voxel(x, z + 1, TerraChunkDefault2D::DEFAULT_CHANNEL_ISOLEVEL);
			sum += _chunk->get_voxel(x, z - 1, TerraChunkDefault2D::DEFAULT_CHANNEL_ISOLEVEL);

			sum /= 6;

			sum -= current;

			if (sum < 0)
				sum = 0;

			_chunk->set_voxel(sum, x, z, TerraChunkDefault2D::DEFAULT_CHANNEL_AO);
		}
	}
}

void TerraJob2D::generate_random_ao(int seed, int octaves, int period, float persistence, float scale_factor) {
	ERR_FAIL_COND(!_chunk.is_valid());

	int margin_start = _chunk->get_margin_start();
	int margin_end = _chunk->get_margin_end();

	int size_x = _chunk->get_size_x();
	int size_z = _chunk->get_size_z();

	int position_x = _chunk->get_position_x();
	int position_z = _chunk->get_position_z();

	Ref<OpenSimplexNoise> noise;
	noise.instance();

	noise->set_seed(seed);
	noise->set_octaves(octaves);
	noise->set_period(period);
	noise->set_persistence(persistence);

	for (int x = -margin_start; x < size_x + margin_end; ++x) {
		for (int z = -margin_start; z < size_z + margin_end; ++z) {

			float val = noise->get_noise_3d(x + (position_x * size_x), 0, z + (position_z * size_z));

			val *= scale_factor;

			if (val > 1)
				val = 1;

			if (val < 0)
				val = -val;

			_chunk->set_voxel(int(val * 255.0), x, z, TerraChunkDefault2D::DEFAULT_CHANNEL_RANDOM_AO);
		}
	}
}

Array TerraJob2D::merge_mesh_array(Array arr) const {
	ERR_FAIL_COND_V(arr.size() != VisualServer::ARRAY_MAX, arr);

	PoolVector3Array verts = arr[VisualServer::ARRAY_VERTEX];
	PoolVector3Array normals = arr[VisualServer::ARRAY_NORMAL];
	PoolVector2Array uvs = arr[VisualServer::ARRAY_TEX_UV];
	PoolColorArray colors = arr[VisualServer::ARRAY_COLOR];
	PoolIntArray indices = arr[VisualServer::ARRAY_INDEX];

	bool has_normals = normals.size() > 0;
	bool has_uvs = uvs.size() > 0;
	bool has_colors = colors.size() > 0;

	int i = 0;
	while (i < verts.size()) {
		Vector3 v = verts[i];

		Array equals;
		for (int j = i + 1; j < verts.size(); ++j) {
			Vector3 vc = verts[j];

			if (Math::is_equal_approx(v.x, vc.x) && Math::is_equal_approx(v.y, vc.y) && Math::is_equal_approx(v.z, vc.z))
				equals.push_back(j);
		}

		for (int k = 0; k < equals.size(); ++k) {
			int rem = equals[k];
			int remk = rem - k;

			verts.remove(remk);

			if (has_normals)
				normals.remove(remk);
			if (has_uvs)
				uvs.remove(remk);
			if (has_colors)
				colors.remove(remk);

			for (int j = 0; j < indices.size(); ++j) {
				int indx = indices[j];

				if (indx == remk)
					indices.set(j, i);
				else if (indx > remk)
					indices.set(j, indx - 1);
			}
		}

		++i;
	}

	arr[VisualServer::ARRAY_VERTEX] = verts;

	if (has_normals)
		arr[VisualServer::ARRAY_NORMAL] = normals;
	if (has_uvs)
		arr[VisualServer::ARRAY_TEX_UV] = uvs;
	if (has_colors)
		arr[VisualServer::ARRAY_COLOR] = colors;

	arr[VisualServer::ARRAY_INDEX] = indices;

	return arr;
}
Array TerraJob2D::bake_mesh_array_uv(Array arr, Ref<Texture> tex, const float mul_color) const {
	ERR_FAIL_COND_V(arr.size() != VisualServer::ARRAY_MAX, arr);
	ERR_FAIL_COND_V(!tex.is_valid(), arr);

	Ref<Image> img = tex->get_data();

	ERR_FAIL_COND_V(!img.is_valid(), arr);

	Vector2 imgsize = img->get_size();

	PoolVector2Array uvs = arr[VisualServer::ARRAY_TEX_UV];
	PoolColorArray colors = arr[VisualServer::ARRAY_COLOR];

	if (colors.size() < uvs.size())
		colors.resize(uvs.size());

#if !GODOT4
	img->lock();
#endif

	for (int i = 0; i < uvs.size(); ++i) {
		Vector2 uv = uvs[i];
		uv *= imgsize;

		int ux = static_cast<int>(CLAMP(uv.x, 0, imgsize.x - 1));
		int uy = static_cast<int>(CLAMP(uv.y, 0, imgsize.y - 1));

		Color c = img->get_pixel(ux, uy);

		colors.set(i, colors[i] * c * mul_color);
	}

#if !GODOT4
	img->unlock();
#endif

	arr[VisualServer::ARRAY_COLOR] = colors;

	return arr;
}

void TerraJob2D::chunk_exit_tree() {

	_in_tree = false;

	if (get_complete()) {
		_chunk.unref();
	} else {
		set_cancelled(true);
	}
}

TerraJob2D::TerraJob2D() {
	_in_tree = false;

	_build_phase_type = BUILD_PHASE_TYPE_NORMAL;
	_build_done = true;
	_phase = 0;

#if !THREAD_POOL_PRESENT
	_complete = true;
	_cancelled = false;

	_max_allocated_time = 0;
	_start_time = 0;

	_current_run_stage = 0;
	_stage = 0;
#endif
}

TerraJob2D::~TerraJob2D() {
	_chunk.unref();
}

void TerraJob2D::_bind_methods() {
	BIND_VMETHOD(MethodInfo("_process", PropertyInfo(Variant::REAL, "delta")));
	BIND_VMETHOD(MethodInfo("_physics_process", PropertyInfo(Variant::REAL, "delta")));

	ClassDB::bind_method(D_METHOD("get_build_phase_type"), &TerraJob2D::get_build_phase_type);
	ClassDB::bind_method(D_METHOD("set_build_phase_type", "value"), &TerraJob2D::set_build_phase_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "build_phase_type", PROPERTY_HINT_ENUM, BINDING_STRING_ACTIVE_BUILD_PHASE_TYPE), "set_build_phase_type", "get_build_phase_type");

	ClassDB::bind_method(D_METHOD("set_chunk", "chunk"), &TerraJob2D::set_chunk);

	ClassDB::bind_method(D_METHOD("get_phase"), &TerraJob2D::get_phase);
	ClassDB::bind_method(D_METHOD("set_phase", "phase"), &TerraJob2D::set_phase);
	ClassDB::bind_method(D_METHOD("next_phase"), &TerraJob2D::next_phase);

	ClassDB::bind_method(D_METHOD("get_build_done"), &TerraJob2D::get_build_done);
	ClassDB::bind_method(D_METHOD("set_build_done", "val"), &TerraJob2D::set_build_done);

	ClassDB::bind_method(D_METHOD("next_job"), &TerraJob2D::next_job);

	BIND_VMETHOD(MethodInfo("_reset"));

	ClassDB::bind_method(D_METHOD("reset"), &TerraJob2D::reset);
	ClassDB::bind_method(D_METHOD("_reset"), &TerraJob2D::_reset);

	ClassDB::bind_method(D_METHOD("_execute"), &TerraJob2D::_execute);

	BIND_VMETHOD(MethodInfo("_execute_phase"));

	ClassDB::bind_method(D_METHOD("execute_phase"), &TerraJob2D::execute_phase);
	ClassDB::bind_method(D_METHOD("_execute_phase"), &TerraJob2D::_execute_phase);

	ClassDB::bind_method(D_METHOD("generate_ao"), &TerraJob2D::generate_ao);
	ClassDB::bind_method(D_METHOD("generate_random_ao", "seed", "octaves", "period", "persistence", "scale_factor"), &TerraJob2D::generate_random_ao, DEFVAL(4), DEFVAL(30), DEFVAL(0.3), DEFVAL(0.6));

	ClassDB::bind_method(D_METHOD("chunk_exit_tree"), &TerraJob2D::chunk_exit_tree);

#if !THREAD_POOL_PRESENT
	ClassDB::bind_method(D_METHOD("get_complete"), &TerraJob2D::get_complete);
	ClassDB::bind_method(D_METHOD("set_complete", "value"), &TerraJob2D::set_complete);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "complete"), "set_complete", "get_complete");

	ClassDB::bind_method(D_METHOD("get_start_time"), &TerraJob2D::get_start_time);
	ClassDB::bind_method(D_METHOD("set_start_time", "value"), &TerraJob2D::set_start_time);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "start_time"), "set_start_time", "get_start_time");

	ClassDB::bind_method(D_METHOD("get_current_run_stage"), &TerraJob2D::get_current_run_stage);
	ClassDB::bind_method(D_METHOD("set_current_run_stage", "value"), &TerraJob2D::set_current_run_stage);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "current_run_stage"), "set_current_run_stage", "get_current_run_stage");

	ClassDB::bind_method(D_METHOD("get_stage"), &TerraJob2D::get_stage);
	ClassDB::bind_method(D_METHOD("set_stage", "value"), &TerraJob2D::set_stage);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "stage"), "set_stage", "get_stage");

	ClassDB::bind_method(D_METHOD("get_current_execution_time"), &TerraJob2D::get_current_execution_time);

	ClassDB::bind_method(D_METHOD("should_do", "just_check"), &TerraJob2D::should_do, DEFVAL(false));
	ClassDB::bind_method(D_METHOD("should_return"), &TerraJob2D::should_return);

	BIND_VMETHOD(MethodInfo("_execute"));
	ClassDB::bind_method(D_METHOD("execute"), &TerraJob2D::execute);

	ADD_SIGNAL(MethodInfo("completed"));
#endif
}

#if !THREAD_POOL_PRESENT
bool TerraJob2D::get_complete() const {
	return _complete;
}
void TerraJob2D::set_complete(const bool value) {
	_complete = value;
}

bool TerraJob2D::get_cancelled() const {
	return _cancelled;
}
void TerraJob2D::set_cancelled(const bool value) {
	_cancelled = value;
}

float TerraJob2D::get_max_allocated_time() const {
	return _max_allocated_time;
}
void TerraJob2D::set_max_allocated_time(const float value) {
	_max_allocated_time = value;
}

int TerraJob2D::get_start_time() const {
	return _start_time;
}
void TerraJob2D::set_start_time(const int value) {
	_start_time = value;
}

int TerraJob2D::get_current_run_stage() const {
	return _current_run_stage;
}
void TerraJob2D::set_current_run_stage(const int value) {
	_current_run_stage = value;
}

int TerraJob2D::get_stage() const {
	return _stage;
}
void TerraJob2D::set_stage(const int value) {
	_stage = value;
}

void TerraJob2D::reset_stages() {
	_current_run_stage = 0;
	_stage = 0;
}

float TerraJob2D::get_current_execution_time() {
	return 0;
}

bool TerraJob2D::should_do(const bool just_check) {
	return true;
}
bool TerraJob2D::should_return() {
	if (_cancelled)
		return true;

	return false;
}

void TerraJob2D::execute() {
	ERR_FAIL_COND(!has_method("_execute"));

	call("_execute");
}

#endif