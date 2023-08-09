#include "backend/renderer/renderer.h"
#include "backend/shaders/passthrough-shader.h"

#include "graphic/filters/texture-filter/texture-filter.h"

namespace null::render {
	void c_texture_filter::handle() {
		backend::renderer->set_texture(texture);
		backend::passthrough_texture_shader->use();

		child_command->handle();

		backend::passthrough_color_shader->use();
		backend::renderer->set_texture(nullptr);
	}
}