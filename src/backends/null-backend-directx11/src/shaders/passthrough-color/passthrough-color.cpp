#include <shaders/passthrough-color/passthrough-color.h>

namespace null::render::backend::directx11::shaders {
	void c_passthrough_color::use() {
		if(empty()) return;
		i_shader::use();

		compiled_objects::passthrough.set_constant({ renderer->get_matrix() });
	}
}