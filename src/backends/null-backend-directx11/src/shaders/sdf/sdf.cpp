#include <shaders/sdf/sdf.h>

namespace null::render::backend::directx11::shaders {
	void c_sdf::use() {
		if(empty()) return;
		i_shader::use();

		compiled_objects::passthrough.set_constant({ renderer->get_matrix() });
		compiled_objects::sdf.set_constant(constant);
	}
}