#include <shaders/passthrough-texture/passthrough-texture.h>

namespace null::render::backend::directx9::shaders {
	void c_passthrough_texture::use() {
		if(empty()) return;
		i_shader::use();

		vertex_shader->set_constant(0, renderer->get_matrix().linear_array.data(), 4);
	}
}