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

#include "terrain_2d_world_blocky.h"

#include "terrain_2d_chunk_blocky.h"

#include "../../meshers/blocky/terrain_2d_mesher_blocky.h"
#include "../jobs/terrain_2d_light_job.h"
#include "../jobs/terrain_2d_prop_job.h"
#include "../jobs/terrain_2d_terrain_job.h"

Ref<Terrain2DChunk> Terrain2DWorldBlocky::_create_chunk(int x, int z, Ref<Terrain2DChunk> chunk) {
	if (!chunk.is_valid()) {
		chunk = Ref<Terrain2DChunk>(memnew(Terrain2DChunkBlocky));
	}

	if (chunk->job_get_count() == 0) {
		Ref<Terrain2DTerrain2DJob> tj;
		tj.instance();

		Ref<Terrain2DLightJob> lj;
		lj.instance();

		Ref<Terrain2DMesherJobStep> s;
		s.instance();
		s->set_job_type(Terrain2DMesherJobStep::TYPE_NORMAL);
		tj->add_jobs_step(s);

		s.instance();
		s->set_job_type(Terrain2DMesherJobStep::TYPE_NORMAL_LOD);
		s->set_lod_index(1);
		tj->add_jobs_step(s);

		s.instance();
		s->set_job_type(Terrain2DMesherJobStep::TYPE_NORMAL_LOD);
		s->set_lod_index(2);
		tj->add_jobs_step(s);

		s.instance();
		s->set_job_type(Terrain2DMesherJobStep::TYPE_MERGE_VERTS);
		tj->add_jobs_step(s);

		s.instance();
		s->set_job_type(Terrain2DMesherJobStep::TYPE_BAKE_TEXTURE);
		tj->add_jobs_step(s);

		tj->set_mesher(Ref<Terrain2DMesher>(memnew(Terrain2DMesherBlocky())));

		Ref<Terrain2DMesherBlocky> liquid_mesher;
		liquid_mesher.instance();
		liquid_mesher->set_channel_index_type(Terrain2DChunkDefault::DEFAULT_CHANNEL_LIQUID_TYPE);
		liquid_mesher->set_channel_index_isolevel(Terrain2DChunkDefault::DEFAULT_CHANNEL_LIQUID_ISOLEVEL);
		tj->set_liquid_mesher(liquid_mesher);

		Ref<Terrain2DPropJob> pj;
		pj.instance();
		pj->set_prop_mesher(Ref<Terrain2DMesher>(memnew(Terrain2DMesherBlocky)));

		s.instance();
		s->set_job_type(Terrain2DMesherJobStep::TYPE_NORMAL);
		pj->add_jobs_step(s);

		s.instance();
		s->set_job_type(Terrain2DMesherJobStep::TYPE_MERGE_VERTS);
		pj->add_jobs_step(s);

		s.instance();
		s->set_job_type(Terrain2DMesherJobStep::TYPE_BAKE_TEXTURE);
		pj->add_jobs_step(s);

		s.instance();
		s->set_job_type(Terrain2DMesherJobStep::TYPE_SIMPLIFY_MESH);
#ifdef MESH_UTILS_PRESENT
		Ref<FastQuadraticMeshSimplifier> fqms;
		fqms.instance();
		s->set_fqms(fqms);
		s->set_simplification_steps(2);
#endif
		pj->add_jobs_step(s);

		chunk->job_add(lj);
		chunk->job_add(tj);
		chunk->job_add(pj);
	}

	return Terrain2DWorld::_create_chunk(x, z, chunk);
}

Terrain2DWorldBlocky::Terrain2DWorldBlocky() {
	set_data_margin_start(1);
	set_data_margin_end(1);
}

Terrain2DWorldBlocky ::~Terrain2DWorldBlocky() {
}

void Terrain2DWorldBlocky::_bind_methods() {
}