#include "../wrapper/opengl3.h"
#include "state-pipeline.h"

namespace null::render::opengl3 {
	void c_state_pipeline::setup_state() {
		opengl::enable(opengl::e_blend);
		opengl::blend_equation(opengl::e_func_add);
		opengl::blend_func(1, opengl::e_one_minus_src_alpha);
		//opengl::blend_func_separate(opengl::e_src_alpha, opengl::e_one_minus_src_alpha, 1, opengl::e_one_minus_src_alpha);
		opengl::disable(opengl::e_cull_face);
		opengl::disable(opengl::e_depth_test);
		opengl::disable(opengl::e_stencil_test);
		opengl::enable(opengl::e_scissor_test);
		opengl::disable(opengl::e_primitive_restart);

		//opengl::viewport(0.f, shared::viewport.y, shared::viewport.x, -shared::viewport.y);

		backend::renderer->set_clip({ { 0 }, shared::viewport });
		backend::renderer->set_matrix(backend::renderer->get_projection_matrix());
		backend::renderer_event_dispatcher.setup_state();
	}

	void c_state_pipeline::save_state() {
		opengl::get_integerv(opengl::e_draw_framebuffer_binding, (int*)&saved_state.draw_fbo);
		opengl::get_integerv(opengl::e_read_framebuffer_binding, (int*)&saved_state.read_fbo);
		opengl::get_integerv(opengl::e_renderbuffer_binding, (int*)&saved_state.rbo);
		opengl::get_integerv(opengl::e_active_texture, (int*)&saved_state.active_texture);
		opengl::active_texture(opengl::e_texture0);
		opengl::get_integerv(opengl::e_current_program, (int*)&saved_state.program);
		opengl::get_integerv(opengl::e_texture_binding_2d, (int*)&saved_state.texture);
		opengl::get_integerv(opengl::e_sampler_binding, (int*)&saved_state.sampler);
		opengl::get_integerv(opengl::e_array_buffer_binding, (int*)&saved_state.array_buffer);
		opengl::get_integerv(opengl::e_element_array_buffer, (int*)&saved_state.element_array_buffer);
		opengl::get_integerv(opengl::e_vertex_array_binding, (int*)&saved_state.vertex_array_object);

		opengl::get_integerv(opengl::e_viewport, saved_state.viewport);
		opengl::get_integerv(opengl::e_scissor_box, saved_state.scissor_box);
		opengl::get_integerv(opengl::e_blend_src_rgb, (int*)&saved_state.blend_src_rgb);
		opengl::get_integerv(opengl::e_blend_dst_rgb, (int*)&saved_state.blend_dst_rgb);
		opengl::get_integerv(opengl::e_blend_src_alpha, (int*)&saved_state.blend_src_alpha);
		opengl::get_integerv(opengl::e_blend_dst_alpha, (int*)&saved_state.blend_dst_alpha);
		opengl::get_integerv(opengl::e_blend_equation_rgb, (int*)&saved_state.blend_equation_rgb);
		opengl::get_integerv(opengl::e_blend_equation_alpha, (int*)&saved_state.blend_equation_alpha);
		saved_state.enable_blend = opengl::is_enabled(opengl::e_blend);
		saved_state.enable_cull_face = opengl::is_enabled(opengl::e_cull_face);
		saved_state.enable_depth_test = opengl::is_enabled(opengl::e_depth_test);
		saved_state.enable_stencil_test = opengl::is_enabled(opengl::e_stencil_test);
		saved_state.enable_scissor_test = opengl::is_enabled(opengl::e_scissor_test);
		saved_state.enable_primitive_restart = opengl::is_enabled(opengl::e_primitive_restart);
	}

	void c_state_pipeline::restore_state() {
		restore_framebuffer();
		restore_shader();
		restore_mesh();
		restore_texture();
		opengl::blend_equation_separate(saved_state.blend_equation_rgb, saved_state.blend_equation_alpha);
		opengl::blend_func_separate(saved_state.blend_src_rgb, saved_state.blend_dst_rgb, saved_state.blend_src_alpha, saved_state.blend_dst_alpha);
		if(saved_state.enable_blend) opengl::enable(opengl::e_blend); else opengl::disable(opengl::e_blend);
		if(saved_state.enable_cull_face) opengl::enable(opengl::e_cull_face); else opengl::disable(opengl::e_cull_face);
		if(saved_state.enable_depth_test) opengl::enable(opengl::e_depth_test); else opengl::disable(opengl::e_depth_test);
		if(saved_state.enable_stencil_test) opengl::enable(opengl::e_stencil_test); else opengl::disable(opengl::e_stencil_test);
		if(saved_state.enable_scissor_test) opengl::enable(opengl::e_scissor_test); else opengl::disable(opengl::e_scissor_test);
		if(saved_state.enable_primitive_restart) opengl::enable(opengl::e_primitive_restart); else opengl::disable(opengl::e_primitive_restart);
		opengl::viewport(saved_state.viewport[0], saved_state.viewport[1], saved_state.viewport[2], saved_state.viewport[3]);
		opengl::scissor(saved_state.scissor_box[0], saved_state.scissor_box[1], saved_state.scissor_box[2], saved_state.scissor_box[3]);
	}

	void c_state_pipeline::restore_framebuffer() {
		opengl::bind_framebuffer(opengl::e_read_framebuffer, saved_state.read_fbo);
		opengl::bind_framebuffer(opengl::e_draw_framebuffer, saved_state.draw_fbo);
		opengl::bind_renderbuffer(opengl::e_renderbuffer, saved_state.rbo);
	}

	void c_state_pipeline::restore_mesh() {
		opengl::bind_vertex_array(saved_state.vertex_array_object);
		opengl::bind_buffer(opengl::e_array_buffer, saved_state.array_buffer);
		opengl::bind_buffer(opengl::e_element_array_buffer, saved_state.element_array_buffer);
	}

	void c_state_pipeline::restore_shader() {
		opengl::use_program(saved_state.program);
	}

	void c_state_pipeline::restore_texture() {
		opengl::bind_texture(opengl::e_texture_2d, saved_state.texture);
		opengl::bind_sampler(0, saved_state.sampler);
		opengl::active_texture(saved_state.active_texture);
	}
}