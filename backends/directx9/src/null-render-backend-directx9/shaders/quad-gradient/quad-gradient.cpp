#include "quad-gradient.h"

namespace null::render::directx9 {
	void c_quad_gradient_shader::use() {
		if(empty()) return;
		c_shader::use();

		vertex_shader->set_constant(0, backend::renderer->get_matrix().linear_array.data(), 4);
	}

	void c_quad_gradient_shader::set_constants(const constants_t& constants) {
		for(auto [i, color] : constants.colors | std::views::enumerate) {
			pixel_shader->set_constant(i, color.cast<float>().channels.data(), 1);
		}
	}
}