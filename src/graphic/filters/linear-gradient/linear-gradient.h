#pragma once
#include <graphic/filters/filters.h>

namespace null::render::filters {
	class c_linear_gradient : public i_filter {
	public:
		radians_t angle{ };
		std::vector<std::pair<color_t<int>, float>> stops{ };

	public:
		c_linear_gradient(std::unique_ptr<commands::i_command> _child_command, const radians_t& _angle, const std::vector<std::pair<color_t<int>, float>>& _stops)
			: i_filter{ std::move(_child_command) }, angle { _angle }, stops{ _stops } { }

	public:
		virtual void handle() override;
	};
}