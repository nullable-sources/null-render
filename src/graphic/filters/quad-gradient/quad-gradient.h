#pragma once
#include <graphic/filters/filters.h>

namespace null::render::filters {
	class c_quad_gradient : public i_filter {
	public:
		std::array<color_t<int>, 4> colors{ };

	public:
		c_quad_gradient(const std::array<color_t<int>, 4>& _colors) : colors{ _colors } { }

	public:
		virtual void handle() override;
	};
}