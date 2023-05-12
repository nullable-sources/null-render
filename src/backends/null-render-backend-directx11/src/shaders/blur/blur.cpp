#include <shaders/blur/blur.h>

namespace null::render::backend::directx11::shaders {
	void c_blur::use() {
		if(empty()) return;
		i_shader::use();

		compiled_objects::passthrough.set_constant({ renderer->get_matrix() });
		compiled_objects::blur.set_constant(constant);
	}
}