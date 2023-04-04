#pragma once
#include <renderer/shaders/passthrough/passthrough.h>
#include <shaders/shader.h>

namespace null::renderer::shaders {
	namespace sources {
		static const std::vector<byte>& passthrough_color_pixel() {
#include <shaders/passthrough-color/compiled/passthrough-color-pixel.h>
			static const std::vector<byte> source{ shader_bytes, shader_bytes + sizeof(shader_bytes) };
			return source;
		}

		static const std::vector<byte>& passthrough_color_vertex() {
#include <shaders/passthrough-color/compiled/passthrough-color-vertex.h>
			static const std::vector<byte> source{ shader_bytes, shader_bytes + sizeof(shader_bytes) };
			return source;
		}
	}

	class c_passthrough_color : public render::impl::shaders::i_passthrough_color, public i_shader {
	public:
		struct constant_buffer_t { matrix4x4_t matrix{ }; };
		wrapper::c_constant_buffer<constant_buffer_t> constant_buffer{ };

	public:
		void on_create() override {
			if(!empty()) return;

			pixel_shader = std::make_unique<wrapper::c_pixel_shader>();
			pixel_shader->create(sources::passthrough_color_pixel());

			vertex_shader = std::make_unique<wrapper::c_vertex_shader>();
			vertex_shader->create(sources::passthrough_color_vertex());

			constant_buffer.create();
		}

		void on_destroy() override {
			if(empty()) return;
			pixel_shader->destroy();
			vertex_shader->destroy();
			constant_buffer.destroy();
		}

		void use() override {
			i_shader::use();

			if(empty()) return;
			constant_buffer.edit_constant({ render::renderer->get_matrix() });
			vertex_shader->set_constant(constant_buffer);
		}
	};
}