#pragma once
#include "../color/compiled-object.h"

namespace null::render::directx11 {
	namespace sources {
		static const std::vector<byte>& radial_gradient() {
			#include "compiled/radial-gradient.h"
			static const std::vector<byte> source(shader_data, shader_data + sizeof(shader_data));
			return source;
		}
	}

	class c_radial_gradient_shader_object : public c_pixel_shader {
	public:
		struct constant_buffer_t {
		public:
			std::array<color_t<int>, 16> colors{ };
			std::array<vec4_t<float>, 16> stops{ };
			vec2_t<float> origin{ };
			vec2_t<float> radius{ };
			int stops_count{ };
		};
		c_constant_buffer<constant_buffer_t> constant_buffer{ };

	public:
		void set_constant(const constant_buffer_t& constant, int slot = 0) {
			constant_buffer.edit_constant(constant);
			set_constant_buffer(constant_buffer.buffer, slot);
		}

	public:
		void create() override {
			if(!empty()) return;
			compile(sources::radial_gradient());
			constant_buffer.create();
		}

		void destroy() override { c_pixel_shader::destroy(); constant_buffer.destroy(); }
	} inline radial_gradient_shader_object{ };
}