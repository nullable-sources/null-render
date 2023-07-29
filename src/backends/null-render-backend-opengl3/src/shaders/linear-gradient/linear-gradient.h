#pragma once
#include <shaders/shader.h>
#include <backend/renderer/renderer.h>
#include <backend/shaders/linear-gradient/linear-gradient.h>

#include <shaders/compiled-objects/linear-gradient/linear-gradient.h>
#include <shaders/compiled-objects/passthrough/passthrough.h>

namespace null::render::backend::opengl3::shaders {
	class c_linear_gradient : public backend::shaders::i_linear_gradient, public i_shader {
	public:
		wrapper::c_uniform<float> angle{ };
		wrapper::c_uniform<int> stops_count{ };
		wrapper::c_uniform<std::array<vec4_t<float>, 16>> colors{ };
		wrapper::c_uniform<std::array<float, 16>> stops{ };

		wrapper::c_uniform<matrix4x4_t> matrix{ };

	public:
		void on_create() override;
		void on_destroy() override;

		void use() override;

	public:
		void set_angle(radians_t _angle) override { angle.value() = _angle; }
		void set_stops(const std::vector<std::pair<color_t<int>, float>>& _stops) {
			for(const int& i : std::views::iota(0u, _stops.size())) {
				colors.value()[i] = _stops[i].first.cast<float>();
				stops.value()[i] = _stops[i].second;
			}

			stops_count.value() = _stops.size();
		}
	};
}