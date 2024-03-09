#include "../renderer/renderer.h"
#include "state-pipeline.h"

namespace null::render::backend {
	void i_state_pipeline::c_framebuffers_pipeline::append() { stack.back()->use(); }
	void i_state_pipeline::c_framebuffers_pipeline::set_default() { state_pipeline->restore_framebuffer(); }

	void i_state_pipeline::c_meshes_pipeline::append() { stack.back()->use(); }
	void i_state_pipeline::c_meshes_pipeline::set_default() { state_pipeline->restore_mesh(); }

	void i_state_pipeline::c_shaders_pipeline::append() { stack.back()->use(); }
	void i_state_pipeline::c_shaders_pipeline::set_default() { state_pipeline->restore_shader(); }

	void i_state_pipeline::c_textures_pipeline::append() { renderer->set_texture(stack.back()); }
	void i_state_pipeline::c_textures_pipeline::set_default() { state_pipeline->restore_texture(); }

	void i_state_pipeline::setup_state() {
		backend::renderer->set_clip({ { 0 }, shared::viewport });
		backend::renderer->set_matrix(backend::renderer->get_projection_matrix());
		backend::renderer->set_translation(0.f);
		backend::renderer_event_dispatcher.setup_state();
	}
}