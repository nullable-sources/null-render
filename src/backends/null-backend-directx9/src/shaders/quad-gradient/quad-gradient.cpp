#include <shaders/quad-gradient/quad-gradient.h>

namespace null::render::backend::directx9::shaders {
	void c_quad_gradient::use() {
		if(empty()) return;
		i_shader::use();

		vertex_shader->set_constant(0, renderer->get_matrix().linear_array.data(), 4);
		pixel_shader->set_constant(0, colors[0].channels.data(), 1);
		pixel_shader->set_constant(1, colors[1].channels.data(), 1);
		pixel_shader->set_constant(2, colors[2].channels.data(), 1);
		pixel_shader->set_constant(3, colors[3].channels.data(), 1);
	}
}