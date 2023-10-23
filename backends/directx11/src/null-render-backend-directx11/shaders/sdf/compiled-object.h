#pragma once
#include "../../wrapper/shaders/pixel.h"

namespace null::render::directx11 {
	namespace sources {
		static const std::vector<byte>& sdf() {
			#include "compiled/sdf.h"
			static const std::vector<byte> source(shader_data, shader_data + sizeof(shader_data));
			return source;
		}
	}

	class c_sdf_shader_object : public c_pixel_shader {
	public:
		c_constant_buffer<backend::i_sdf_shader::constants_t> constant_buffer{ };

	public:
		void set_constant(const backend::i_sdf_shader::constants_t& constant, int slot = 0) {
			constant_buffer.edit_constant(constant);
			set_constant_buffer(constant_buffer.buffer, slot);
		}

	public:
		void on_create() override {
			if(!empty()) return;
			create(sources::sdf());
			constant_buffer.create();
		}

		void on_destroy() override { destroy(); constant_buffer.destroy(); }
	} inline sdf_shader_object{ };
}