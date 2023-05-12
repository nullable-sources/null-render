#include <shaders/linear-gradient/linear-gradient.h>

namespace null::render::backend::directx9::shaders {
	void c_linear_gradient::use() {
		if(empty()) return;
		i_shader::use();

		vertex_shader->set_constant(0, renderer->get_matrix().linear_array.data(), 4);
		pixel_shader->set_constant(0, &angle);
		pixel_shader->set_constant(1, &stops_count);
		pixel_shader->set_constant(2, (float*)stops.data(), 16);
		pixel_shader->set_constant(18, (float*)colors.data(), 16);
	}
}