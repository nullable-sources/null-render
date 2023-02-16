#pragma once
#include <shaders/shaders.h>

namespace null::render::shaders {
	class c_general_color_shader : public i_shader {
	public:
		c_matrix4x4 matrix{ };
		c_vertex_element<renderer::c_opengl3::vertex_t, 2, opengl::e_float, false, offsetof(renderer::c_opengl3::vertex_t, pos)> position{ };
		c_vertex_element<renderer::c_opengl3::vertex_t, 4, opengl::e_unsigned_byte, true, offsetof(renderer::c_opengl3::vertex_t, color)> color{ };

	public:
		void create() override {
			program = std::make_unique<c_program>();
			program->create();

			fragment = std::make_unique<c_fragment_shader_wrapper>();
			fragment->create();
			fragment->compile(memory::resource_t{ "null-renderer:opengl:shaders:fragment-color", "null-renderer:opengl:resources" }.load().locked_data);
			program->attach_shader(fragment.get());

			vertex = std::make_unique<c_vertex_shader_wrapper>();
			vertex->create();
			vertex->compile(memory::resource_t{ "null-renderer:opengl:shaders:vertex-color", "null-renderer:opengl:resources" }.load().locked_data);
			program->attach_shader(vertex.get());

			program->link();

			matrix.get_location(program.get(), "proj_mtx");
			position.get_location(program.get(), "position");
			color.get_location(program.get(), "color");
		}

		void destroy() override {
			program->detach_shader(fragment.get());
			program->detach_shader(vertex.get());

			fragment->destroy();
			vertex->destroy();

			program->destroy();
		}

		void setup_state() override {
			matrix.setup(matrix4x4_t::project_ortho(0.f, render::shared::viewport.x, render::shared::viewport.y, 0.f, -10000.f, 10000.f));
		}

		void use() override {
			program->use();

			matrix.set();

			position.set();
			color.set();
		}
	} inline general_color{ };
}