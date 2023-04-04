#pragma once
#include <renderer/shaders/quad-gradient/quad-gradient.h>
#include <shaders/passthrough-texture/passthrough-texture.h>

namespace null::renderer::shaders {
	namespace sources {
		static const std::vector<byte>& quad_gradient() {
			#include <shaders/quad-gradient/compiled/quad-gradient.h>
			static const std::vector<byte> source{ shader_bytes, shader_bytes + sizeof(shader_bytes) };
			return source;
		}
	}

	class c_quad_gradient : public render::impl::shaders::i_quad_gradient, public i_shader {
	public:
		struct vs_constant_buffer_t { matrix4x4_t matrix{ }; };
		wrapper::c_constant_buffer<vs_constant_buffer_t> vs_constant_buffer{ };

		struct ps_constant_buffer_t { std::array<color_t<int>, 4> colors{ }; };
		wrapper::c_constant_buffer<ps_constant_buffer_t> ps_constant_buffer{ };

	public:
		void on_create() override {
			if(!empty()) return;

			pixel_shader = std::make_unique<wrapper::c_pixel_shader>();
			pixel_shader->create(sources::quad_gradient());

			vertex_shader = std::make_unique<wrapper::c_vertex_shader>();
			vertex_shader->create(sources::passthrough_texture_vertex());

			vs_constant_buffer.create();
			ps_constant_buffer.create();
		}

		void on_destroy() override {
			if(empty()) return;
			pixel_shader->destroy();
			vertex_shader->destroy();
			vs_constant_buffer.destroy();
			ps_constant_buffer.destroy();
		}

		void use() override {
			i_shader::use();

			if(empty()) return;
			vs_constant_buffer.edit_constant({ render::renderer->get_matrix() });
			vertex_shader->set_constant(vs_constant_buffer);

			pixel_shader->set_constant(ps_constant_buffer);
		}

	public:
		void set_colors(const std::array<color_t<int>, 4>& colors) override {
			ps_constant_buffer.edit_constant({ colors });
		}
	};
}