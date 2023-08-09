#pragma once
#include <backend/shaders/shader.h>

namespace null::render::backend {
	class i_linear_gradient_shader : public virtual i_shader {
	public:
		struct constants_t {
		public:
			radians_t angle{ };
			std::vector<std::pair<color_t<int>, float>> stops{ };
		};

	public:
		virtual void set_constants(const constants_t& constants) = 0;
	}; inline std::unique_ptr<i_linear_gradient_shader> linear_gradient_shader{ };
}