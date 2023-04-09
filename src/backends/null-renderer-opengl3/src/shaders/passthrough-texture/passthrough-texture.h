#pragma once
#include <shaders/shaders.h>
#include <renderer/shaders/passthrough/passthrough.h>

namespace null::renderer::shaders {
	class c_passthrough_texture : public render::impl::shaders::i_passthrough_texture, public i_shader {
	public:
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
			fragment->compile(memory::resource_t{ "null-renderer:opengl:shaders:passthrough-texture.frag", "null-renderer:opengl:resources" }.load().locked_data);
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
		}
	};
}