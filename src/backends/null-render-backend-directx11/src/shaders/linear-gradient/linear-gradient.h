#pragma once
#include <shaders/shader.h>
#include <backend/shaders/linear-gradient/linear-gradient.h>

#include <shaders/compiled-objects/linear-gradient/linear-gradient.h>
#include <shaders/compiled-objects/passthrough/passthrough.h>

namespace null::render::backend::directx11::shaders {
	class c_linear_gradient : public backend::shaders::i_linear_gradient, public i_shader {
	public:
		compiled_objects::c_linear_gradient::constant_buffer_t constant{ };

	public:
		c_linear_gradient() : i_shader{ &compiled_objects::linear_gradient, &compiled_objects::passthrough } { }

	public:
		void use() override;

	public:
		virtual void set_angle(radians_t angle) override { constant.angle = angle; }
		virtual void set_stops(const std::vector<std::pair<color_t<int>, float>>& stops) {
			for(const int& i : std::views::iota(0u, stops.size())) {
				constant.colors[i] = stops[i].first;
				constant.stops[i].x = stops[i].second;
			}

			constant.stops_count = stops.size();
		}
	};
}