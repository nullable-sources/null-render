#include <shaders/linear-gradient/linear-gradient.h>

namespace null::render::backend::directx11::shaders {
	void c_linear_gradient::use() {
		if(empty()) return;
		i_shader::use();

		compiled_objects::passthrough.set_constant({ renderer->get_matrix() });
		compiled_objects::linear_gradient.set_constant(constant);
	}
}