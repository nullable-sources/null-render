#pragma once
#include <null-render.h>
#include <shaders/shaders.h>
#include <renderer/shaders/quad-gradient/quad-gradient.h>

namespace null::renderer::shaders {
	class c_quad_gradient : public render::impl::shaders::i_quad_gradient, public i_shader {
	public:
		renderer::wrapper::c_uniform<vec4_t<float>> top_left_color{ }, top_right_color{ }, bottom_right_color{ }, bottom_left_color{ };
		renderer::wrapper::c_uniform<vec4_t<float>> hui{ };

		renderer::wrapper::c_uniform<matrix4x4_t> matrix{ };
		renderer::wrapper::c_vertex_element<render::impl::vertex_t, 2, opengl::e_float, false, offsetof(render::impl::vertex_t, pos)> position{ };
		renderer::wrapper::c_vertex_element<render::impl::vertex_t, 2, opengl::e_float, false, offsetof(render::impl::vertex_t, uv)> uv{ };
		renderer::wrapper::c_vertex_element<render::impl::vertex_t, 4, opengl::e_int, false, offsetof(render::impl::vertex_t, color)> color{ };

	public:
		void on_create() override {
			if(!empty()) return;
			program = std::make_unique<renderer::wrapper::c_program>();
			program->create();

			fragment = std::make_unique<renderer::wrapper::c_fragment_shader>();
			fragment->create();
			fragment->compile(memory::resource_t{ "null-renderer:opengl:shaders:quad-gradient.frag", "null-renderer:opengl:resources" }.load().locked_data);
			program->attach_shader(fragment.get());

			vertex = std::make_unique<renderer::wrapper::c_vertex_shader>();
			vertex->create();
			vertex->compile(memory::resource_t{ "null-renderer:opengl:shaders:passthrough-texture.vert", "null-renderer:opengl:resources" }.load().locked_data);
			program->attach_shader(vertex.get());

			program->link();

			matrix.get_location(program.get(), "matrix");
			position.get_location(program.get(), "position");
			uv.get_location(program.get(), "uv");
			color.get_location(program.get(), "color");

			top_left_color.get_location(program.get(), "color_tl");
			top_right_color.get_location(program.get(), "color_tr");
			bottom_right_color.get_location(program.get(), "color_br");
			bottom_left_color.get_location(program.get(), "color_bl");
		}

		void on_destroy() override {
			program->detach_shader(fragment.get());
			program->detach_shader(vertex.get());

			fragment->destroy();
			vertex->destroy();

			program->destroy();
		}

		void use() override {
			program->use();

			matrix.set(render::renderer->get_matrix());

			position.set();
			uv.set();
			color.set();

			top_left_color.set();
			top_right_color.set();
			bottom_right_color.set();
			bottom_left_color.set();
		}

	public:
		void set_colors(const std::array<color_t<int>, 4>& colors) override {
			top_left_color.value() = colors[0].cast<float>();
			top_right_color.value() = colors[1].cast<float>();
			bottom_left_color.value() = colors[2].cast<float>();
			bottom_right_color.value() = colors[3].cast<float>();
		}
	};
}