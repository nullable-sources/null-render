#pragma once
#include <graphic/draw-list/brushes/brush.h>

namespace null::render {
	struct linear_gradient_brush_t : public brush_t {
	public:
		radians_t angle{ };
		std::vector<std::pair<color_t<int>, float>> stops{ };

	public:
		template <typename self_t> auto&& set_angle(this self_t&& self, const radians_t& angle) { self.angle = angle; return self; }
		template <typename self_t> auto&& add_stop(this self_t&& self, const color_t<int>& color, const float& uv) { self.stops.push_back({ color, uv }); return self; }

	public:
		virtual std::unique_ptr<commands::i_command> prepare_command(std::unique_ptr<commands::c_geometry>& command) const override;
	};
}