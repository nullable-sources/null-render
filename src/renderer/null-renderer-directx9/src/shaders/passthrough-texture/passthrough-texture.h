#pragma once
#include <renderer/shaders/passthrough/passthrough.h>
#include <shaders/shader.h>

namespace null::renderer::shaders {
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

	class c_passthrough_texture : public render::impl::shaders::i_passthrough_texture, public i_shader {
	public:
		void on_create() override {
			if(!empty()) return;

			pixel_shader = std::make_unique<wrapper::c_pixel_shader>();
			pixel_shader->compile(sources::passthrough_texture_pixel().data());

			vertex_shader = std::make_unique<wrapper::c_vertex_shader>();
			vertex_shader->compile(sources::passthrough_texture_vertex().data());
		}

		void on_destroy() override {
			if(empty()) return;
			pixel_shader->destroy();
			vertex_shader->destroy();
		}

		void use() override {
			i_shader::use();

			if(empty()) return;
			vertex_shader->set_constant(0, render::renderer->get_matrix().linear_array.data(), 4);
		}
	};
}