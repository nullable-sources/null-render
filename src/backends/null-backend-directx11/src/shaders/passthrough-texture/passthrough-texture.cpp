#include <shaders/passthrough-texture/passthrough-texture.h>

namespace null::render::backend::directx11::shaders {
	void c_passthrough_texture::use() {
		if(empty()) return;
		i_shader::use();

		compiled_objects::passthrough.set_constant({ renderer->get_matrix() });
	}
}