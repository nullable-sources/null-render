#include <shaders/sdf/sdf.h>

namespace null::render::backend::directx9::shaders {
	void c_sdf::use() {
		if(empty()) return;
		i_shader::use();

		vertex_shader->set_constant(0, renderer->get_matrix().linear_array.data(), 4);

		pixel_shader->set_constant(0, &aa);
		pixel_shader->set_constant(1, &outline_thickness);
		pixel_shader->set_constant(2, outline_start.channels.data());
		pixel_shader->set_constant(3, outline_end.channels.data());
	}
}