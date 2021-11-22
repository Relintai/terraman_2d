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

#include "terra_world_2d_editor.h"

#include "core/version.h"

#include "editor/editor_scale.h"
#include "editor/editor_settings.h"

#include "terra_world_2d.h"

#include "core/os/keyboard.h"

#include "terra_chunk_2d.h"

#include "../library/terra_surface_2d.h"
#include "../library/terraman_library_2d.h"

#include "../defines.h"

#include core_input_h
#include spatial_editor_plugin_h
#include camera_h

bool TerraWorld2DEditor::forward_spatial_input_event(Camera *p_camera, const Ref<InputEvent> &p_event) {
	if (!_world || !_world->get_editable()) {
		return false;
	}

	Ref<InputEventMouseButton> mb = p_event;

	if (mb.is_valid()) {
		if (mb->is_pressed()) {
			Ref<TerramanLibrary2D> lib = _world->get_library();

			if (!lib.is_valid())
				return false;

			if (mb->get_button_index() == BUTTON_LEFT) {
				return do_input_action(p_camera, Point2(mb->get_position().x, mb->get_position().y), true);
			} else {
				return false;
			}

			//return do_input_action(p_camera, Point2(mb->get_position().x, mb->get_position().y), true);
		}
	}

	return false;
}

bool TerraWorld2DEditor::do_input_action(Camera *p_camera, const Point2 &p_point, bool p_click) {
	if (!spatial_editor || !_world || !_world->is_inside_world())
		return false;

	Camera *camera = p_camera;
	Vector3 from = camera->project_ray_origin(p_point);
	Vector3 to = from + camera->project_ray_normal(p_point) * 10000;
	Transform local_xform = _world->get_global_transform().affine_inverse();

	from = local_xform.xform(from);
	to = local_xform.xform(to);

	PhysicsDirectSpaceState *ss = _world->GET_WORLD()->get_direct_space_state();

	PhysicsDirectSpaceState::RayResult res;

	if (ss->intersect_ray(from, to, res)) {
		int selected_voxel = 0;
		int channel = 0;

		channel = _channel_type;

		if (channel == -1)
			return false;

		int isolevel = _current_isolevel;
		bool mode_add = false;

		if (_tool_mode == TOOL_MODE_ADD) {
			selected_voxel = _selected_type + 1;
			mode_add = true;
		} else if (_tool_mode == TOOL_MODE_REMOVE) {
			selected_voxel = 0;
			isolevel = 0;
			mode_add = false;
		}

		_world->set_voxel_with_tool(mode_add, res.position, res.normal, selected_voxel, isolevel);

		return true;
	}

	return false;
}

void TerraWorld2DEditor::edit(TerraWorld2D *p_world) {
	_world = p_world;

	if (!_world)
		return;

	_channel_type = _world->get_channel_index_info(TerraWorld2D::CHANNEL_TYPE_INFO_TYPE);
	_channel_isolevel = _world->get_channel_index_info(TerraWorld2D::CHANNEL_TYPE_INFO_ISOLEVEL);

	if (_channel_isolevel == -1) {
		_isolevel_slider->hide();
	} else {
		_isolevel_slider->show();
	}

	spatial_editor = Object::cast_to<SpatialEditorPlugin>(_editor->get_editor_plugin_screen());

	for (int i = 0; i < _surfaces_vbox_container->get_child_count(); ++i) {
		Node *child = _surfaces_vbox_container->get_child(i);

		if (!child->is_queued_for_deletion()) {
			child->queue_delete();
		}
	}

	Ref<TerramanLibrary2D> library = _world->get_library();

	if (!library.is_valid())
		return;

	if (!library->get_initialized())
		library->refresh_rects();

	bool f = false;
	for (int i = 0; i < library->voxel_surface_get_num(); ++i) {
		Ref<TerraSurface2D> surface = library->voxel_surface_get(i);

		if (!surface.is_valid())
			continue;

		String text = String::num(i) + " - " + surface->get_name();

		Button *button = memnew(Button);
		button->set_text(text);
		button->set_text_align(Button::ALIGN_LEFT);
		button->set_meta("index", i);
		button->set_toggle_mode(true);
		button->set_button_group(_surfaces_button_group);
		button->set_h_size_flags(SIZE_EXPAND_FILL);

		button->CONNECT("button_up", this, TerraWorld2DEditor, _on_surface_button_pressed);

		_surfaces_vbox_container->add_child(button);

		if (!f) {
			button->set_pressed(true);
			f = true;
		}
	}
}

TerraWorld2DEditor::TerraWorld2DEditor() {
	_world = NULL;
	_selected_type = 0;
	_channel_type = -1;
	_current_isolevel = 255;
	_channel_isolevel = -1;
	_editor = NULL;
	_tool_mode = TOOL_MODE_ADD;
}
TerraWorld2DEditor::TerraWorld2DEditor(EditorNode *p_editor) {
	_world = NULL;
	_selected_type = 0;
	_channel_type = -1;
	_current_isolevel = 255;
	_channel_isolevel = -1;

	_editor = p_editor;
	_tool_mode = TOOL_MODE_ADD;

	spatial_editor_hb = memnew(HBoxContainer);
	spatial_editor_hb->set_h_size_flags(SIZE_EXPAND_FILL);
	spatial_editor_hb->set_alignment(BoxContainer::ALIGN_BEGIN);
	SpatialEditor::get_singleton()->add_control_to_menu_panel(spatial_editor_hb);

	_tool_button_group.instance();

	ToolButton *add_button = memnew(ToolButton);
	add_button->set_text("Add");
	add_button->set_toggle_mode(true);
	add_button->set_pressed(true);
	add_button->set_button_group(_tool_button_group);
	add_button->set_meta("tool_mode", TOOL_MODE_ADD);

	add_button->CONNECT("button_up", this, TerraWorld2DEditor, _on_tool_button_pressed);

	add_button->set_shortcut(ED_SHORTCUT("voxelman_world_editor/add_mode", "Add Mode", KEY_A));
	spatial_editor_hb->add_child(add_button);

	ToolButton *remove_button = memnew(ToolButton);
	remove_button->set_text("Remove");
	remove_button->set_toggle_mode(true);
	remove_button->set_button_group(_tool_button_group);
	remove_button->set_meta("tool_mode", TOOL_MODE_REMOVE);

	remove_button->CONNECT("button_up", this, TerraWorld2DEditor, _on_tool_button_pressed);

	remove_button->set_shortcut(ED_SHORTCUT("voxelman_world_editor/remove_mode", "Remove Mode", KEY_S));
	spatial_editor_hb->add_child(remove_button);

	ToolButton *insert_buton = memnew(ToolButton);
	insert_buton->set_text("Insert");

	insert_buton->CONNECT("button_up", this, TerraWorld2DEditor, _on_insert_block_at_camera_button_pressed);

	insert_buton->set_shortcut(ED_SHORTCUT("voxelman_world_editor/instert_block_at_camera", "Insert at camera", KEY_B));
	spatial_editor_hb->add_child(insert_buton);

	set_custom_minimum_size(Size2(200 * EDSCALE, 0));

	_isolevel_slider = memnew(HSlider);
	_isolevel_slider->set_min(1);
	_isolevel_slider->set_max(255);
	_isolevel_slider->set_value(_current_isolevel);
	_isolevel_slider->set_custom_minimum_size(Size2(50 * EDSCALE, 0));
	_isolevel_slider->set_v_size_flags(SIZE_EXPAND_FILL);
	spatial_editor_hb->add_child(_isolevel_slider);

	_isolevel_slider->CONNECT("value_changed", this, TerraWorld2DEditor, _on_isolevel_slider_value_changed);

	_isolevel_slider->hide();

	ScrollContainer *scs = memnew(ScrollContainer);
	scs->set_h_size_flags(SIZE_EXPAND_FILL);
	scs->set_v_size_flags(SIZE_EXPAND_FILL);
	scs->set_name("Surfaces");
	add_child(scs);

	_surfaces_vbox_container = memnew(VBoxContainer);
	scs->add_child(_surfaces_vbox_container);
	_surfaces_vbox_container->set_h_size_flags(SIZE_EXPAND_FILL);

	_surfaces_button_group.instance();
}
TerraWorld2DEditor::~TerraWorld2DEditor() {
	_world = NULL;

	_surfaces_button_group.unref();
}

void TerraWorld2DEditor::_node_removed(Node *p_node) {
	if (p_node == _world)
		_world = NULL;
}

void TerraWorld2DEditor::_on_surface_button_pressed() {
	BaseButton *button = _surfaces_button_group->get_pressed_button();

	if (button) {
		_selected_type = button->get_meta("index");
	}
}

void TerraWorld2DEditor::_on_tool_button_pressed() {
	BaseButton *button = _tool_button_group->get_pressed_button();

	if (button) {
		_tool_mode = static_cast<TerraWorld2DEditorToolMode>(static_cast<int>(button->get_meta("tool_mode")));
	}
}

void TerraWorld2DEditor::_on_insert_block_at_camera_button_pressed() {
	int selected_voxel = 0;
	int channel = 0;

	channel = _channel_type;

	if (channel == -1)
		return;

	SpatialEditorViewport *vp = SpatialEditor::get_singleton()->get_editor_viewport(0);

	if (!vp)
		return;

	Camera *cam = vp->get_camera();

	if (!cam)
		return;

	Vector3 pos = cam->get_transform().origin;
	selected_voxel = _selected_type + 1;

	if (_channel_isolevel == -1) {
		_world->set_voxel_at_world_position(pos, selected_voxel, channel);
	} else {
		_world->set_voxel_at_world_position(pos, selected_voxel, channel, false);
		_world->set_voxel_at_world_position(pos, _current_isolevel, _channel_isolevel);
	}
}

void TerraWorld2DEditor::_on_isolevel_slider_value_changed(float value) {
	_current_isolevel = value;
}

void TerraWorld2DEditor::_bind_methods() {
	ClassDB::bind_method("_node_removed", &TerraWorld2DEditor::_node_removed);
	ClassDB::bind_method("_on_surface_button_pressed", &TerraWorld2DEditor::_on_surface_button_pressed);
	ClassDB::bind_method("_on_tool_button_pressed", &TerraWorld2DEditor::_on_tool_button_pressed);
	ClassDB::bind_method("_on_insert_block_at_camera_button_pressed", &TerraWorld2DEditor::_on_insert_block_at_camera_button_pressed);
	ClassDB::bind_method("_on_isolevel_slider_value_changed", &TerraWorld2DEditor::_on_isolevel_slider_value_changed);
}

void TerraWorld2DEditorPlugin::_notification(int p_what) {
	if (p_what == EditorSettings::NOTIFICATION_EDITOR_SETTINGS_CHANGED) {
		switch ((int)EditorSettings::get_singleton()->get("editors/voxelman/editor_side")) {
			case 0: { // Left.
				SpatialEditor::get_singleton()->get_palette_split()->move_child(voxel_world_editor, 0);
			} break;
			case 1: { // Right.
				SpatialEditor::get_singleton()->get_palette_split()->move_child(voxel_world_editor, 1);
			} break;
		}
	}
}

void TerraWorld2DEditorPlugin::edit(Object *p_object) {
	voxel_world_editor->edit(Object::cast_to<TerraWorld2D>(p_object));
}

bool TerraWorld2DEditorPlugin::handles(Object *p_object) const {
	if (!p_object->is_class("TerraWorld"))
		return false;

	TerraWorld2D *w = Object::cast_to<TerraWorld2D>(p_object);

	return w->get_editable();
}

void TerraWorld2DEditorPlugin::make_visible(bool p_visible) {
	if (p_visible) {
		voxel_world_editor->show();
		voxel_world_editor->spatial_editor_hb->show();
		voxel_world_editor->set_process(true);
	} else {
		voxel_world_editor->spatial_editor_hb->hide();
		voxel_world_editor->hide();
		voxel_world_editor->edit(NULL);
		voxel_world_editor->set_process(false);
	}
}

TerraWorld2DEditorPlugin::TerraWorld2DEditorPlugin(EditorNode *p_node) {
	editor = p_node;

	EDITOR_DEF("editors/voxelman/editor_side", 1);
	EditorSettings::get_singleton()->add_property_hint(PropertyInfo(Variant::INT, "editors/voxelman/editor_side", PROPERTY_HINT_ENUM, "Left,Right"));

	voxel_world_editor = memnew(TerraWorld2DEditor(editor));
	switch ((int)EditorSettings::get_singleton()->get("editors/voxelman/editor_side")) {
		case 0: { // Left.
			add_control_to_container(CONTAINER_SPATIAL_EDITOR_SIDE_LEFT, voxel_world_editor);
		} break;
		case 1: { // Right.
			add_control_to_container(CONTAINER_SPATIAL_EDITOR_SIDE_RIGHT, voxel_world_editor);
		} break;
	}
	voxel_world_editor->hide();
}

TerraWorld2DEditorPlugin::~TerraWorld2DEditorPlugin() {
}