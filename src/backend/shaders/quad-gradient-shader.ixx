module;
#include <array>
#include <memory>
export module null.render:backend.shaders.quad_gradient;

import :backend.shader;

import null.sdk;

export namespace null::render::backend::shaders {
	class i_quad_gradient : public virtual i_shader {
	public:
		virtual void set_colors(const std::array<color_t<int>, 4>& colors) = 0;
	}; std::unique_ptr<i_quad_gradient> quad_gradient{ };
}