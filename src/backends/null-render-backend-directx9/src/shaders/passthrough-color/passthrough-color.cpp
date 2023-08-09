#include "shaders/passthrough-color/passthrough-color.h"

namespace null::render::directx9 {
	void c_passthrough_color_shader::use() {
		if(empty()) return;
		c_shader::use();

		vertex_shader->set_constant(0, backend::renderer->get_matrix().linear_array.data(), 4);
	}
}