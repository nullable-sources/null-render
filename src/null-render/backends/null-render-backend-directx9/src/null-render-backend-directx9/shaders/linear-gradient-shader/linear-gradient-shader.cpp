#include "linear-gradient-shader.h"

namespace null::render::directx9 {
	void c_linear_gradient_shader::use() {
		if(empty()) return;
		c_shader::use();

		vertex_shader->set_constant(0, backend::renderer->get_matrix().linear_array.data(), 4);
	}

	void c_linear_gradient_shader::set_constants(const constants_t& constants) {
		std::array<color_t<float>, 16> colors{ };
		std::array<vec4_t<float>, 16> stops{ };

		for(auto [color, stop, pair] : std::views::zip(colors, stops, constants.stops)) {
			color = pair.first.cast<float>();
			stop.x = pair.second;
		}

		const float stops_count{ (float)constants.stops.size() };
		pixel_shader->set_constant(0, (float*)&constants.angle);
		pixel_shader->set_constant(1, &stops_count);
		pixel_shader->set_constant(2, (float*)stops.data(), 16);
		pixel_shader->set_constant(18, (float*)colors.data(), 16);
	}
}