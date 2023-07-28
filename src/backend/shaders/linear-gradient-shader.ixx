module;
#include <vector>
#include <memory>
export module null.render:backend.shaders.linear_gradient;

import null.sdk;

import :backend.shader;

export namespace null::render::backend::shaders {
	class i_linear_gradient : public virtual i_shader {
	public:
		virtual void set_angle(radians_t angle) = 0;
		virtual void set_stops(const std::vector<std::pair<color_t<int>, float>>& stops) = 0;
	}; std::unique_ptr<i_linear_gradient> linear_gradient{ };
}