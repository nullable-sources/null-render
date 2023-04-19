#pragma once
#include <backend/shaders/linear-gradient/linear-gradient.h>
#include <shaders/shader.h>

#include <shaders/compiled-objects/linear-gradient/linear-gradient.h>
#include <shaders/compiled-objects/passthrough/passthrough.h>

namespace null::render::backend::directx9::shaders {
	class c_linear_gradient : public backend::shaders::i_linear_gradient, public i_shader {
	public:
		float angle{ };
		float stops_count{ };
		std::array<color_t<float>, 16> colors{ };
		std::array<vec4_t<float>, 16> stops{ };

	public:
		c_linear_gradient() : i_shader{ &compiled_objects::linear_gradient, &compiled_objects::passthrough } { }

	public:
		void use() override;

	public:
		void set_angle(const radians_t& _angle) { angle = _angle; }
		void set_stops(const std::vector<std::pair<color_t<int>, float>>& _stops) override {
			for(const int& i : std::views::iota(0u, _stops.size())) {
				colors[i] = _stops[i].first.cast<float>();
				stops[i].x = _stops[i].second;
			}

			stops_count = _stops.size();
		}
	};
}