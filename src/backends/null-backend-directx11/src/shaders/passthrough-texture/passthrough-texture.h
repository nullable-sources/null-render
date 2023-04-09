#pragma once
#include <backend/shaders/passthrough/passthrough.h>
#include <shaders/shader.h>

namespace null::render::backend::directx11::shaders {
	namespace sources {
		static const std::vector<byte>& passthrough_texture_pixel() {
			#include <shaders/passthrough-texture/compiled/passthrough-texture-pixel.h>
			static const std::vector<byte> source{ shader_bytes, shader_bytes + sizeof(shader_bytes) };
			return source;
		}

		static const std::vector<byte>& passthrough_texture_vertex() {
			#include <shaders/passthrough-texture/compiled/passthrough-texture-vertex.h>
			static const std::vector<byte> source{ shader_bytes, shader_bytes + sizeof(shader_bytes) };
			return source;
		}
	}

	class c_passthrough_texture : public backend::shaders::i_passthrough_texture, public i_shader {
	public:
		struct constant_buffer_t { matrix4x4_t matrix{ }; };
		wrapper::c_constant_buffer<constant_buffer_t> constant_buffer{ };

	public:
		void on_create() override {
			if(!empty()) return;

			pixel_shader = std::make_unique<wrapper::c_pixel_shader>();
			pixel_shader->create(sources::passthrough_texture_pixel());

			vertex_shader = std::make_unique<wrapper::c_vertex_shader>();
			vertex_shader->create(sources::passthrough_texture_vertex());

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
			constant_buffer.edit_constant({ renderer->get_matrix() });
		}
	};
}