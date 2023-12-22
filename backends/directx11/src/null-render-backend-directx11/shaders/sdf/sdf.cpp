#include "sdf.h"

namespace null::render::directx11 {
	void c_sdf_shader::use() {
		if(empty()) return;
		c_shader::use();

		passthrough_shader_object.set_constant({ backend::renderer->get_matrix(), backend::renderer->get_translation() });
	}

	void c_sdf_shader::set_constants(const constants_t& constants) {
		sdf_shader_object.set_constant(constants);
	}
}