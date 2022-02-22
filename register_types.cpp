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

#include "register_types.h"

#include "library/terrain_2d_surface.h"
#include "library/terrain_2d_surface_simple.h"

#include "library/terrain_2d_material_cache.h"
#include "library/terrain_2d_library.h"
#include "library/terrain_2d_library_simple.h"

#ifdef TEXTURE_PACKER_PRESENT
#include "library/terrain_2d_surface_merger.h"
#include "library/terrain_2d_library_merger.h"
#include "library/terrain_2d_library_merger_pcm.h"
#include "library/terrain_2d_material_cache_pcm.h"
#endif

#include "data/terrain_2d_light.h"
#include "meshers/terrain_2d_mesher.h"

#include "world/block_terrain_2d_structure.h"
#include "world/terrain_2d_chunk.h"
#include "world/terrain_2d_environment_data.h"
#include "world/terrain_2d_structure.h"
#include "world/terrain_2d_world.h"

#include "world/default/terrain_2d_chunk_default.h"
#include "world/default/terrain_2d_world_default.h"

#include "level_generator/terrain_2d_level_generator.h"
#include "level_generator/terrain_2d_level_generator_flat.h"

#include "areas/terrain_2d_world_area.h"

#include "world/terrain_2d_world_editor.h"

#include "meshers/blocky/terrain_2d_mesher_blocky.h"

#include "world/blocky/terrain_2d_chunk_blocky.h"
#include "world/blocky/terrain_2d_world_blocky.h"

#include "nodes/terrain_2d_light_node.h"

#include "world/jobs/terrain_2d_job.h"
#include "world/jobs/terrain_2d_light_job.h"
#include "world/jobs/terrain_2d_mesher_job_step.h"
#include "world/jobs/terrain_2d_prop_job.h"
#include "world/jobs/terrain_2d_terrain_job.h"

void register_terraman_2d_types() {
	ClassDB::register_class<Terrain2DMesher>();
	ClassDB::register_class<Terrain2DMesherDefault>();

	ClassDB::register_class<Terrain2DSurface>();
	ClassDB::register_class<Terrain2DSurfaceSimple>();

	ClassDB::register_class<Terrain2DLibrary>();
	ClassDB::register_class<Terrain2DLibrarySimple>();

	ClassDB::register_class<Terrain2DMaterialCache>();

#ifdef TEXTURE_PACKER_PRESENT
	ClassDB::register_class<Terrain2DSurfaceMerger>();
	ClassDB::register_class<Terrain2DLibraryMerger>();
	ClassDB::register_class<Terrain2DLibraryMergerPCM>();
	ClassDB::register_class<Terrain2DMaterialCachePCM>();
#endif

	ClassDB::register_class<Terrain2DLight>();
	ClassDB::register_class<Terrain2DLightNode>();

	ClassDB::register_class<Terrain2DWorld>();
	ClassDB::register_class<Terrain2DChunk>();
	ClassDB::register_class<Terrain2DStructure>();
	ClassDB::register_class<BlockTerrain2DStructure>();
	ClassDB::register_class<Terrain2DEnvironmentData>();

	ClassDB::register_class<Terrain2DChunkDefault>();
	ClassDB::register_class<Terrain2DWorldDefault>();

	ClassDB::register_class<Terrain2DMesherBlocky>();
	ClassDB::register_class<Terrain2DWorldBlocky>();
	ClassDB::register_class<Terrain2DChunkBlocky>();

	ClassDB::register_class<Terrain2DLevelGenerator>();
	ClassDB::register_class<Terrain2DLevelGeneratorFlat>();

	ClassDB::register_class<Terrain2DWorldArea>();

	ClassDB::register_class<Terrain2DJob>();
	ClassDB::register_class<Terrain2DTerrain2DJob>();
	ClassDB::register_class<Terrain2DMesherJobStep>();
	ClassDB::register_class<Terrain2DLightJob>();
	ClassDB::register_class<Terrain2DPropJob>();

#ifdef TOOLS_ENABLED
	EditorPlugins::add_by_type<Terrain2DWorldEditorPlugin>();
#endif
}

void unregister_terraman_2d_types() {
}
