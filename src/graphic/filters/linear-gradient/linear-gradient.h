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

	public:
		auto&& set_colors(this auto&& self, const std::array<color_t<int>, 4>& colors) { self.colors = colors; return self; }
		auto&& set_top_left_color(this auto&& self, const color_t<int>& color) { self.colors[0] = color; return self; }
		auto&& set_top_right_color(this auto&& self, const color_t<int>& color) { self.colors[1] = color; return self; }
		auto&& set_bottom_left_color(this auto&& self, const color_t<int>& color) { self.colors[2] = color; return self; }
		auto&& set_bottom_right_color(this auto&& self, const color_t<int>& color) { self.colors[3] = color; return self; }
	};

	struct linear_gradient_t : public i_filter_instancer {
	public:
		radians_t angle{ };
		std::vector<std::pair<color_t<int>, float>> stops{ };

	public:
		auto&& set_angle(this auto&& self, radians_t angle) { self.angle = angle; return self; }
		auto&& set_stops(this auto&& self, const std::vector<std::pair<color_t<int>, float>>& stops) { self.stops = stops; return self; }
		auto&& add_stop(this auto&& self, const color_t<int>& color, const float& uv) { self.stops.push_back({ color, uv }); return self; }

	public:
		std::unique_ptr<i_filter> instance_filter(std::unique_ptr<commands::c_geometry>&& child_command) const override {
			child_command->set_default_uvs();
			return std::make_unique<c_linear_gradient>(std::move(child_command), angle, stops);
		}
	};
}