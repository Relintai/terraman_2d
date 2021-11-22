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

#include "terra_world_blocky_2d.h"

#include "terra_chunk_blocky_2d.h"

#include "../../meshers/blocky/terra_mesher_blocky_2d.h"
#include "../../meshers/blocky/terra_mesher_liquid_blocky_2d.h"
#include "../jobs/terra_light_job_2d.h"
#include "../jobs/terra_prop_job_2d.h"
#include "../jobs/terra_terrarin_job_2d.h"

Ref<TerraChunk2D> TerraWorldBlocky2D::_create_chunk(int x, int z, Ref<TerraChunk2D> chunk) {
	if (!chunk.is_valid()) {
		chunk = Ref<TerraChunk2D>(memnew(TerraChunkBlocky2D));
	}

	if (chunk->job_get_count() == 0) {
		Ref<TerraTerrarinJob2D> tj;
		tj.instance();

		Ref<TerraLightJob2D> lj;
		lj.instance();

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

		tj->set_mesher(Ref<TerraMesher2D>(memnew(TerraMesherBlocky2D())));
		tj->set_liquid_mesher(Ref<TerraMesher2D>(memnew(TerraMesherLiquidBlocky2D())));

		Ref<TerraPropJob2D> pj;
		pj.instance();
		pj->set_prop_mesher(Ref<TerraMesher2D>(memnew(TerraMesherBlocky2D)));

		s.instance();
		s->set_job_type(TerraMesherJobStep2D::TYPE_NORMAL);
		pj->add_jobs_step(s);

		s.instance();
		s->set_job_type(TerraMesherJobStep2D::TYPE_MERGE_VERTS);
		pj->add_jobs_step(s);

		s.instance();
		s->set_job_type(TerraMesherJobStep2D::TYPE_BAKE_TEXTURE);
		pj->add_jobs_step(s);

		s.instance();
		s->set_job_type(TerraMesherJobStep2D::TYPE_SIMPLIFY_MESH);
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

	return TerraWorld2D::_create_chunk(x, z, chunk);
}

TerraWorldBlocky2D::TerraWorldBlocky2D() {
	set_data_margin_start(1);
	set_data_margin_end(1);
}

TerraWorldBlocky2D ::~TerraWorldBlocky2D() {
}

void TerraWorldBlocky2D::_bind_methods() {
}