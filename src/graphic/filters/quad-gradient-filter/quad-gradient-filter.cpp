#include "backend/renderer/renderer.h"
#include "backend/shaders/passthrough-shader.h"

#include "graphic/filters/quad-gradient-filter/quad-gradient-filter.h"

namespace null::render {
	void c_quad_gradient_filter::handle() {
		backend::quad_gradient_shader->set_constants(constants);
		backend::quad_gradient_shader->use();

		child_command->handle();

		backend::passthrough_color_shader->use();
	}
}