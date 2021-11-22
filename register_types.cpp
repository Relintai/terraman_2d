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

#include "register_types.h"

#include "library/terra_surface_2d.h"
#include "library/terra_surface_simple_2d.h"

#include "library/terra_material_cache_2d.h"
#include "library/terraman_library_2d.h"
#include "library/terraman_library_simple_2d.h"

#ifdef TEXTURE_PACKER_PRESENT
#include "library/terra_surface_merger_2d.h"
#include "library/terraman_library_merger_2d.h"
#include "library/terraman_library_merger_pcm_2d.h"
#include "library/terra_material_cache_pcm_2d.h"
#endif

#include "data/terra_light_2d.h"
#include "meshers/terra_mesher_2d.h"

#include "world/block_terra_structure_2d.h"
#include "world/terra_chunk_2d.h"
#include "world/terra_environment_data_2d.h"
#include "world/terra_structure_2d.h"
#include "world/terra_world_2d.h"

#include "world/default/terra_chunk_default_2d.h"
#include "world/default/terra_world_default_2d.h"

#include "level_generator/terraman_level_generator_2d.h"
#include "level_generator/terraman_level_generator_flat_2d.h"

#include "areas/terra_world_area_2d.h"

#include "world/terra_world_2d_editor.h"

#include "meshers/blocky/terra_mesher_blocky_2d.h"
#include "meshers/blocky/terra_mesher_liquid_blocky_2d.h"

#include "world/blocky/terra_chunk_blocky_2d.h"
#include "world/blocky/terra_world_blocky_2d.h"

#include "nodes/terraman_light_2d.h"

#include "world/jobs/terra_job_2d.h"
#include "world/jobs/terra_light_job_2d.h"
#include "world/jobs/terra_mesher_job_step_2d.h"
#include "world/jobs/terra_prop_job_2d.h"
#include "world/jobs/terra_terrarin_job_2d.h"

void register_terraman_2d_types() {
	ClassDB::register_class<TerraMesher2D>();
	ClassDB::register_class<TerraMesherDefault2D>();

	ClassDB::register_class<TerraSurface2D>();
	ClassDB::register_class<TerraSurfaceSimple2D>();

	ClassDB::register_class<TerramanLibrary2D>();
	ClassDB::register_class<TerramanLibrarySimple2D>();

	ClassDB::register_class<TerraMaterialCache2D>();

#ifdef TEXTURE_PACKER_PRESENT
	ClassDB::register_class<TerraSurfaceMerger2D>();
	ClassDB::register_class<TerramanLibraryMerger2D>();
	ClassDB::register_class<TerramanLibraryMergerPCM2D>();
	ClassDB::register_class<TerraMaterialCachePCM2D>();
#endif

	ClassDB::register_class<TerraLight2D>();
	ClassDB::register_class<TerramanLight2D>();

	ClassDB::register_class<TerraWorld2D>();
	ClassDB::register_class<TerraChunk2D>();
	ClassDB::register_class<TerraStructure2D>();
	ClassDB::register_class<BlockTerraStructure2D>();
	ClassDB::register_class<TerraEnvironmentData2D>();

	ClassDB::register_class<TerraChunkDefault2D>();
	ClassDB::register_class<TerraWorldDefault2D>();

	ClassDB::register_class<TerraMesherBlocky2D>();
	ClassDB::register_class<TerraWorldBlocky2D>();
	ClassDB::register_class<TerraChunkBlocky2D>();
	ClassDB::register_class<TerraMesherLiquidBlocky2D>();

	ClassDB::register_class<TerramanLevelGenerator2D>();
	ClassDB::register_class<TerramanLevelGeneratorFlat2D>();

	ClassDB::register_class<TerraWorldArea2D>();

	ClassDB::register_class<TerraJob2D>();
	ClassDB::register_class<TerraTerrarinJob2D>();
	ClassDB::register_class<TerraMesherJobStep2D>();
	ClassDB::register_class<TerraLightJob2D>();
	ClassDB::register_class<TerraPropJob2D>();

#ifdef TOOLS_ENABLED
	EditorPlugins::add_by_type<TerraWorld2DEditorPlugin>();
#endif
}

void unregister_terraman_2d_types() {
}
