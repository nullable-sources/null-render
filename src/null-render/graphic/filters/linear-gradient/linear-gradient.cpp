#include "null-render/backend/renderer/renderer.h"
#include "null-render/backend/shaders/passthrough.h"

#include "linear-gradient.h"

namespace null::render {
	void c_linear_gradient_filter::handle() {
		backend::linear_gradient_shader->set_constants(constants);
		backend::linear_gradient_shader->use();

		child_command->handle();

		backend::passthrough_color_shader->use();
	}
}