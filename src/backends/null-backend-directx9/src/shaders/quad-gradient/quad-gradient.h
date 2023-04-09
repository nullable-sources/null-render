#pragma once
#include <backend/shaders/quad-gradient/quad-gradient.h>
#include <shaders/passthrough-texture/passthrough-texture.h>

namespace null::render::backend::directx9::shaders {
	namespace sources {
		static const std::vector<byte>& quad_gradient() {
			#include <shaders/quad-gradient/compiled/quad-gradient.h>
			static const std::vector<byte> source{ shader_bytes, shader_bytes + sizeof(shader_bytes) };
			return source;
		}
	}

	class c_quad_gradient : public backend::shaders::i_quad_gradient, public i_shader {
	public:
		std::array<color_t<float>, 4> colors{ };

	public:
		void on_create() override {
			if(!empty()) return;

			pixel_shader = std::make_unique<wrapper::c_pixel_shader>();
			pixel_shader->compile(sources::quad_gradient().data());

			vertex_shader = std::make_unique<wrapper::c_vertex_shader>();
			vertex_shader->compile(sources::passthrough_texture_vertex().data());
		}

		void on_destroy() override {
			if(empty()) return;
			pixel_shader->destroy();
			vertex_shader->destroy();
		}

		virtual void use() override {
			i_shader::use();

			if(empty()) return;
			vertex_shader->set_constant(0, renderer->get_matrix().linear_array.data(), 4);
			pixel_shader->set_constant(0, colors[0].channels.data(), 1);
			pixel_shader->set_constant(1, colors[1].channels.data(), 1);
			pixel_shader->set_constant(2, colors[2].channels.data(), 1);
			pixel_shader->set_constant(3, colors[3].channels.data(), 1);
		}

	public:
		virtual void set_colors(const std::array<color_t<int>, 4>& _colors) {
			for(std::tuple<color_t<float>&, const color_t<int>&> tuple : std::views::zip(colors, _colors)) {
				std::get<color_t<float>&>(tuple) = std::get<const color_t<int>&>(tuple).cast<float>();
			}
		}
	};
}