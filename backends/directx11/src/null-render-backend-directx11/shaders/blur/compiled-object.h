#pragma once
#include "../../wrapper/shaders/pixel.h"

namespace null::render::directx11 {
	namespace sources {
		static const std::vector<byte>& blur() {
			#include "compiled/blur.h"
			static const std::vector<byte> source(shader_data, shader_data + sizeof(shader_data));
			return source;
		}
	}

	class c_blur_shader_object : public c_pixel_shader {
	public:
		struct constant_buffer_t {
		public:
			std::array<vec4_t<float>, 24> weights{ };
			std::array<vec4_t<float>, 24> offsets{ };
			rect_t<float> uv_limits{ };
			vec2_t<float> texel_size{ };
			vec2_t<float> direction{ };
			int iterations{ };
		} buff{ };
		c_constant_buffer<constant_buffer_t> constant_buffer{ };

	public:
		void set_constant(const constant_buffer_t& constant, int slot = 0) {
			buff = constant;
			constant_buffer.edit_constant(buff);
			set_constant_buffer(constant_buffer.buffer, slot);
		}

		void edit_direction(const vec2_t<float>& direction) {
			buff.direction = direction;
			constant_buffer.edit_constant(buff);
			set_constant_buffer(constant_buffer.buffer, 0);
		}

	public:
		void create() override {
			if(!empty()) return;
			compile(sources::blur());
			constant_buffer.create();
		}

		void destroy() override { c_pixel_shader::destroy(); constant_buffer.destroy(); }
	} inline blur_shader_object{ };
}