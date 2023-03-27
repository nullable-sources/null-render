#pragma once
#include <shaders/shaders.h>
#include <renderer/shaders/sdf/sdf.h>

namespace null::renderer::shaders {
	class c_sdf : public render::impl::shaders::i_sdf, public i_shader {
	public:
		renderer::wrapper::c_uniform<float> aa{ }, outline_thickness{ };
		renderer::wrapper::c_uniform<vec4_t<float>> outline_start{ }, outline_end{ };

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
			fragment->compile(memory::resource_t{ "null-renderer:opengl:shaders:sdf.frag", "null-renderer:opengl:resources" }.load().locked_data);
			program->attach_shader(fragment.get());

			vertex = std::make_unique<renderer::wrapper::c_vertex_shader>();
			vertex->create();
			vertex->compile(memory::resource_t{ "null-renderer:opengl:shaders:passthrough-texture.vert", "null-renderer:opengl:resources" }.load().locked_data);
			program->attach_shader(vertex.get());

			program->link();

			matrix.get_location(program.get(), "matrix");

			aa.get_location(program.get(), "aa");
			outline_thickness.get_location(program.get(), "outline_thickness");
			outline_start.get_location(program.get(), "outline_start");
			outline_end.get_location(program.get(), "outline_end");

			position.get_location(program.get(), "position");
			uv.get_location(program.get(), "uv");
			color.get_location(program.get(), "color");
		}

		void on_destroy() override {
			program->detach_shader(fragment.get());
			program->detach_shader(vertex.get());

			fragment->destroy();
			vertex->destroy();

			program->destroy();
		}

		void use() override {
			if(empty()) return;
			program->use();

			matrix.set(render::renderer->get_matrix());

			position.set();
			uv.set();
			aa.set();
			outline_thickness.set();
			outline_start.set();
			outline_end.set();
			color.set();
		}

	public:
		void set_aa(const float& _aa) override { aa.value() = _aa; }
		void set_outline_thickness(const float& thickness) override { outline_thickness.value() = thickness; }
		void set_outline_start(const color_t<int>& color) override { outline_start.value() = color.cast<float>(); }
		void set_outline_end(const color_t<int>& color) override { outline_end.value() = color.cast<float>(); }
	};
}