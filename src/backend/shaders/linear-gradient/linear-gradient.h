#pragma once
#include <backend/shaders/shader.h>

namespace null::render::backend::shaders {
	class i_linear_gradient : public virtual i_shader {
	public:
		virtual void set_angle(const radians_t& angle) = 0;
		virtual void set_stops(const std::vector<std::pair<color_t<int>, float>>& stops) = 0;
	}; inline std::unique_ptr<i_linear_gradient> linear_gradient{ };
}