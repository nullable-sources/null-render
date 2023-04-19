#include <shaders/quad-gradient/quad-gradient.h>

namespace null::render::backend::directx11::shaders {
	void c_quad_gradient::use() {
		if(empty()) return;
		i_shader::use();

		compiled_objects::passthrough.set_constant({ renderer->get_matrix() });
	}

	void c_quad_gradient::set_colors(const std::array<color_t<int>, 4>& colors) {
		compiled_objects::quad_gradient.set_constant({ colors });
	}
}