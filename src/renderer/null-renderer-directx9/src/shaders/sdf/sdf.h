#pragma once
#include <renderer/shaders/sdf/sdf.h>
#include <shaders/passthrough-texture/passthrough-texture.h>

namespace null::renderer::shaders {
	namespace sources {
		static const std::vector<byte>& sdf() {
			#include <shaders/sdf/compiled/sdf.h>
			static const std::vector<byte> source{ shader_bytes, shader_bytes + sizeof(shader_bytes) };
			return source;
		}
	}

	class c_sdf : public render::impl::shaders::i_sdf, public i_shader {
	public:
		float aa{ }, outline_thickness{ };
		color_t<float> outline_start{ }, outline_end{ };

	public:
		void on_create() override {
			if(!empty()) return;

			pixel_shader = std::make_unique<wrapper::c_pixel_shader>();
			pixel_shader->compile(sources::sdf().data());

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
			vertex_shader->set_constant(0, render::renderer->get_matrix().linear_array.data(), 4);
			pixel_shader->set_constant(0, &aa);
			pixel_shader->set_constant(1, &outline_thickness);
			pixel_shader->set_constant(2, outline_start.channels.data());
			pixel_shader->set_constant(3, outline_end.channels.data());
		}

	public:
		virtual void set_aa(const float& _aa) override { aa = _aa; }
		virtual void set_outline_thickness(const float& thickness) { outline_thickness = thickness; }
		virtual void set_outline_start(const color_t<int>& color) { outline_start = color.cast<float>(); }
		virtual void set_outline_end(const color_t<int>& color) { outline_end = color.cast<float>(); }
	};
}