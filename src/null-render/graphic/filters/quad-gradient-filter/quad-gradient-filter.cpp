#include "null-render/backend/renderer/renderer.h"
#include "null-render/backend/shaders/passthrough-shader.h"

#include "quad-gradient-filter.h"

namespace null::render {
	void c_quad_gradient_filter::handle() {
		backend::quad_gradient_shader->set_constants(constants);
		backend::quad_gradient_shader->use();

		child_command->handle();

		backend::passthrough_color_shader->use();
	}
}