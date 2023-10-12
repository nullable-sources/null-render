#include "quad-gradient-shader.h"

namespace null::render::directx11 {
	void c_quad_gradient_shader::use() {
		if(empty()) return;
		c_shader::use();

		passthrough_shader_object.set_constant({ backend::renderer->get_matrix() });
	}

	void c_quad_gradient_shader::set_constants(const constants_t& constants) {
		quad_gradient_shader_object.set_constant(constants);
	}
}