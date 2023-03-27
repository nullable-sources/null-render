#pragma once
#include <renderer/shaders/shader.h>

namespace null::render::impl::shaders {
	class i_quad_gradient : public virtual i_shader {
	public:
		virtual void set_colors(const std::array<color_t<int>, 4>& colors) = 0;
	}; inline std::unique_ptr<i_quad_gradient> quad_gradient{ };
}