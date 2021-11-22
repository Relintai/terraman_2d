/*
Copyright (c) 2020 Péter Magyar

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

#ifndef TERRA_WORLD_2D_EDITOR_PLUGIN_H
#define TERRA_WORLD_2D_EDITOR_PLUGIN_H

#include "editor/editor_node.h"
#include "editor/editor_plugin.h"

#include "../defines.h"

class TerraWorld2D;
class SpatialEditorPlugin;

class TerraWorld2DEditor : public PanelContainer {
	GDCLASS(TerraWorld2DEditor, PanelContainer);

public:
	enum TerraWorld2DEditorToolMode {
		TOOL_MODE_ADD = 0,
		TOOL_MODE_REMOVE,
	};

public:
	bool forward_spatial_input_event(Camera *p_camera, const Ref<InputEvent> &p_event);

	void edit(TerraWorld2D *p_world);
	bool do_input_action(Camera *p_camera, const Point2 &p_point, bool p_click);

	TerraWorld2DEditor();
	TerraWorld2DEditor(EditorNode *p_editor);
	~TerraWorld2DEditor();

	HBoxContainer *spatial_editor_hb;

protected:
	static void _bind_methods();
	void _node_removed(Node *p_node);
	void _on_surface_button_pressed();
	void _on_tool_button_pressed();
	void _on_insert_block_at_camera_button_pressed();
	void _on_isolevel_slider_value_changed(float value);

private:
	VBoxContainer *_surfaces_vbox_container;
	Ref<ButtonGroup> _surfaces_button_group;

	Ref<ButtonGroup> _tool_button_group;

	TerraWorld2DEditorToolMode _tool_mode;
	TerraWorld2D *_world;

	HSlider *_isolevel_slider;

	int _selected_type;
	int _current_isolevel;

	SpatialEditorPlugin *spatial_editor;
	EditorNode *_editor;

	int _channel_type;
	int _channel_isolevel;
};

class TerraWorld2DEditorPlugin : public EditorPlugin {
	GDCLASS(TerraWorld2DEditorPlugin, EditorPlugin);

	TerraWorld2DEditor *voxel_world_editor;
	EditorNode *editor;

protected:
	void _notification(int p_what);

public:
	virtual bool forward_spatial_gui_input(Camera *p_camera, const Ref<InputEvent> &p_event) { return voxel_world_editor->forward_spatial_input_event(p_camera, p_event); }
	virtual bool forward_spatial_gui_input(int p_index, Camera *p_camera, const Ref<InputEvent> &p_event) { return voxel_world_editor->forward_spatial_input_event(p_camera, p_event); }
	virtual String get_name() const { return "TerraWorld2DEditor"; }
	bool has_main_screen() const { return false; }
	virtual void edit(Object *p_object);
	virtual bool handles(Object *p_object) const;
	virtual void make_visible(bool p_visible);

	TerraWorld2DEditorPlugin(EditorNode *p_node);
	~TerraWorld2DEditorPlugin();
};

#endif