#include <backend/renderer/renderer.h>
#include <backend/shaders/passthrough/passthrough.h>

#include <graphic/filters/texture/texture.h>

namespace null::render::filters {
	void c_texture::handle() {
		backend::renderer->set_texture(texture);
		backend::shaders::passthrough_texture->use();

		child_command->handle();

		backend::shaders::passthrough_color->use();
		backend::renderer->set_texture(nullptr);
	}
}