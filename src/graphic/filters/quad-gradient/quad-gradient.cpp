#include <renderer/renderer.h>
#include <renderer/shaders/quad-gradient/quad-gradient.h>
#include <renderer/shaders/passthrough/passthrough.h>

#include <graphic/filters/quad-gradient/quad-gradient.h>

namespace null::render::filters {
	void c_quad_gradient::handle() {
		impl::shaders::quad_gradient->set_colors(colors);
		impl::shaders::quad_gradient->use();

		child_command->handle();

		impl::shaders::passthrough_color->use();
	}
}