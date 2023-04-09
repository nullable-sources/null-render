#include <backend/renderer/renderer.h>
#include <backend/shaders/quad-gradient/quad-gradient.h>
#include <backend/shaders/passthrough/passthrough.h>

#include <graphic/filters/quad-gradient/quad-gradient.h>

namespace null::render::filters {
	void c_quad_gradient::handle() {
		backend::shaders::quad_gradient->set_colors(colors);
		backend::shaders::quad_gradient->use();

		child_command->handle();

		backend::shaders::passthrough_color->use();
	}
}