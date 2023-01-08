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

#include "core/object/class_db.h"

#include "library/terrain_2d_surface.h"
#include "library/terrain_2d_surface_simple.h"

#include "library/terrain_2d_library.h"
#include "library/terrain_2d_library_simple.h"
#include "library/terrain_2d_material_cache.h"

#ifdef TEXTURE_PACKER_PRESENT
#include "library/terrain_2d_library_merger.h"
#include "library/terrain_2d_library_merger_pcm.h"
#include "library/terrain_2d_material_cache_pcm.h"
#include "library/terrain_2d_surface_merger.h"
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

#include "meshers/simple/terrain_2d_mesher_simple.h"
#include "world/simple/terrain_2d_chunk_simple.h"
#include "world/simple/terrain_2d_world_simple.h"

#include "meshers/isometric/terrain_2d_mesher_isometric.h"
#include "world/isometric/terrain_2d_chunk_isometric.h"
#include "world/isometric/terrain_2d_world_isometric.h"

#include "nodes/terrain_2d_light_node.h"

#include "world/jobs/terrain_2d_job.h"
#include "world/jobs/terrain_2d_light_job.h"
#include "world/jobs/terrain_2d_prop_job.h"
#include "world/jobs/terrain_2d_terrain_job.h"

void initialize_terraman_2d_module(ModuleInitializationLevel p_level) {
	if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE) {
		GDREGISTER_CLASS(Terrain2DMesher);
		GDREGISTER_CLASS(Terrain2DMesherDefault);

		GDREGISTER_CLASS(Terrain2DSurface);
		GDREGISTER_CLASS(Terrain2DSurfaceSimple);

		GDREGISTER_CLASS(Terrain2DLibrary);
		GDREGISTER_CLASS(Terrain2DLibrarySimple);

		GDREGISTER_CLASS(Terrain2DMaterialCache);

#ifdef TEXTURE_PACKER_PRESENT
		GDREGISTER_CLASS(Terrain2DSurfaceMerger);
		GDREGISTER_CLASS(Terrain2DLibraryMerger);
		GDREGISTER_CLASS(Terrain2DLibraryMergerPCM);
		GDREGISTER_CLASS(Terrain2DMaterialCachePCM);
#endif

		GDREGISTER_CLASS(Terrain2DLight);
		GDREGISTER_CLASS(Terrain2DLightNode);

		GDREGISTER_CLASS(Terrain2DWorld);
		GDREGISTER_CLASS(Terrain2DChunk);
		GDREGISTER_CLASS(Terrain2DStructure);
		GDREGISTER_CLASS(BlockTerrain2DStructure);
		GDREGISTER_CLASS(Terrain2DEnvironmentData);

		GDREGISTER_CLASS(Terrain2DChunkDefault);
		GDREGISTER_CLASS(Terrain2DWorldDefault);

		GDREGISTER_CLASS(Terrain2DMesherSimple);
		GDREGISTER_CLASS(Terrain2DWorldSimple);
		GDREGISTER_CLASS(Terrain2DChunkSimple);

		GDREGISTER_CLASS(Terrain2DMesherIsometric);
		GDREGISTER_CLASS(Terrain2DWorldIsometric);
		GDREGISTER_CLASS(Terrain2DChunkIsometric);

		GDREGISTER_CLASS(Terrain2DLevelGenerator);
		GDREGISTER_CLASS(Terrain2DLevelGeneratorFlat);

		GDREGISTER_CLASS(Terrain2DWorldArea);

		GDREGISTER_CLASS(Terrain2DJob);
		GDREGISTER_CLASS(Terrain2DTerrain2DJob);
		GDREGISTER_CLASS(Terrain2DLightJob);
		GDREGISTER_CLASS(Terrain2DProp2DJob);
	}

#ifdef TOOLS_ENABLED
	if (p_level == MODULE_INITIALIZATION_LEVEL_EDITOR) {
		EditorPlugins::add_by_type<Terrain2DWorldEditorPlugin>();
	}
#endif
}

void uninitialize_terraman_2d_module(ModuleInitializationLevel p_level) {
}
