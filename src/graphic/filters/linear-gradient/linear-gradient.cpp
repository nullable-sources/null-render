#include <backend/renderer/renderer.h>
#include <backend/shaders/linear-gradient/linear-gradient.h>
#include <backend/shaders/passthrough/passthrough.h>

#include <graphic/filters/linear-gradient/linear-gradient.h>

namespace null::render::filters {
	void c_linear_gradient::handle() {
		backend::shaders::linear_gradient->set_angle(angle);
		backend::shaders::linear_gradient->set_stops(stops);
		backend::shaders::linear_gradient->use();

		child_command->handle();

		backend::shaders::passthrough_color->use();
	}
}