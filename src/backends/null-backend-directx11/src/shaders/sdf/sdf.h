#pragma once
#include <backend/shaders/sdf/sdf.h>
#include <shaders/passthrough-texture/passthrough-texture.h>

namespace null::render::backend::directx11::shaders {
	namespace sources {
		static const std::vector<byte>& sdf() {
			#include <shaders/sdf/compiled/sdf.h>
			static const std::vector<byte> source{ shader_bytes, shader_bytes + sizeof(shader_bytes) };
			return source;
		}
	}

	class c_sdf : public backend::shaders::i_sdf, public i_shader {
	public:
		struct vs_constant_buffer_t { matrix4x4_t matrix{ }; };
		wrapper::c_constant_buffer<vs_constant_buffer_t> vs_constant_buffer{ };

		struct ps_constant_buffer_t {
			color_t<int> outline_start{ }, outline_end{ };
			float aa{ }, outline_thickness{ };
		} ps_constant{ };

		wrapper::c_constant_buffer<ps_constant_buffer_t> ps_constant_buffer{ };

	public:
		void on_create() override {
			if(!empty()) return;

			pixel_shader = std::make_unique<wrapper::c_pixel_shader>();
			pixel_shader->create(sources::sdf());

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
			vs_constant_buffer.edit_constant({ renderer->get_matrix() });
			vertex_shader->set_constant(vs_constant_buffer);

			ps_constant_buffer.edit_constant(ps_constant);
			pixel_shader->set_constant(ps_constant_buffer);
		}

	public:
		virtual void set_aa(const float& aa) override { ps_constant.aa = aa; }
		virtual void set_outline_thickness(const float& thickness) { ps_constant.outline_thickness = thickness; }
		virtual void set_outline_start(const color_t<int>& color) { ps_constant.outline_start = color; }
		virtual void set_outline_end(const color_t<int>& color) { ps_constant.outline_end = color; }
	};
}