#pragma once
#include "../internal/shader.h"

namespace null::render::backend {
	class i_blur_shader : public virtual i_shader {
	public:
		struct constants_t {
		public:
			vec2_t<float> texel_size{ };
			rect_t<float> uv_limits{ };
			std::array<float, 96> offsets{ }, weights{ };
			int iterations{ };
		};

	public:
		virtual void set_constants(const constants_t& constants) = 0;
		virtual void set_direction(const vec2_t<float>& direction) = 0;
	}; inline std::unique_ptr<i_blur_shader> blur_shader{ };
}