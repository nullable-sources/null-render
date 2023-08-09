#include <backend/renderer/renderer.h>
#include <backend/shaders/passthrough/passthrough.h>

#include <graphic/filters/linear-gradient/linear-gradient.h>

namespace null::render {
	void c_linear_gradient_filter::handle() {
		backend::linear_gradient_shader->set_constants(constants);
		backend::linear_gradient_shader->use();

		child_command->handle();

		backend::passthrough_color_shader->use();
	}
}