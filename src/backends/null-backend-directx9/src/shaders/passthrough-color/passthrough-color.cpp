#include <shaders/passthrough-color/passthrough-color.h>

namespace null::render::backend::directx9::shaders {
	void c_passthrough_color::use() {
		if(empty()) return;
		i_shader::use();

		vertex_shader->set_constant(0, renderer->get_matrix().linear_array.data(), 4);
	}
}