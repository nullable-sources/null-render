#pragma once
#include "null-render/backend/shaders/linear-gradient.h"
#include "../filters.h"

namespace null::render {
	class c_linear_gradient_filter : public i_filter {
	public:
		backend::i_linear_gradient_shader::constants_t constants{ };

	public:
		c_linear_gradient_filter(std::unique_ptr<i_command> _child_command, const backend::i_linear_gradient_shader::constants_t& _constants)
			: i_filter(std::move(_child_command)), constants(_constants) { }

	public:
		virtual void handle() override;
	};

	struct linear_gradient_filter_t : public i_filter_instancer {
	public:
		backend::i_linear_gradient_shader::constants_t constants{ };

	public:
		auto&& set_angle(this auto&& self, radians_t angle) { self.constants.angle = angle; return self; }
		auto&& set_stops(this auto&& self, const std::vector<std::pair<color_t<int>, float>>& stops) { self.constants.stops = stops; return self; }
		auto&& add_stop(this auto&& self, const color_t<int>& color, float uv) { self.constants.stops.push_back({ color, uv }); return self; }

	public:
		std::unique_ptr<i_filter> instance_filter(std::unique_ptr<c_geometry_command>&& child_command) const override {
			child_command->set_default_uvs();
			return std::make_unique<c_linear_gradient_filter>(std::move(child_command), constants);
		}
	};
}