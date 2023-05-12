#include <shaders/blur/blur.h>

namespace null::render::backend::directx9::shaders {
	void c_blur::use() {
		if(empty()) return;
		i_shader::use();

		vertex_shader->set_constant(0, renderer->get_matrix().linear_array.data(), 4);

		pixel_shader->set_constant(0, &amount);
		pixel_shader->set_constant(1, direction.coordinates.data());
		pixel_shader->set_constant(2, texel_size.coordinates.data());
		pixel_shader->set_constant(3, uv_limits.linear_corners.data());
	}
}