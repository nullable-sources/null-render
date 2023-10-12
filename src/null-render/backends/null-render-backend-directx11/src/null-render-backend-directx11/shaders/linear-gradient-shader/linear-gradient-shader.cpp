#include "linear-gradient-shader.h"

namespace null::render::directx11 {
	void c_linear_gradient_shader::use() {
		if(empty()) return;
		c_shader::use();

		passthrough_shader_object.set_constant({ backend::renderer->get_matrix() });
	}

	void c_linear_gradient_shader::set_constants(const constants_t& constants) {
		c_linear_gradient_shader_object::constant_buffer_t constant_buffer{ };

		for(auto [color, stop, pair] : std::views::zip(constant_buffer.colors, constant_buffer.stops, constants.stops)) {
			color = pair.first;
			stop.x = pair.second;
		}

		constant_buffer.stops_count = constants.stops.size();
		constant_buffer.angle = constants.angle;

		linear_gradient_shader_object.set_constant(constant_buffer);
	}
}