#include "shaders/passthrough-texture/passthrough-texture.h"

namespace null::render::directx11 {
	void c_passthrough_texture_shader::use() {
		if(empty()) return;
		c_shader::use();

		passthrough_shader_object.set_constant({ backend::renderer->get_matrix() });
	}
}