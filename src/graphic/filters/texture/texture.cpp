#include <renderer/renderer.h>
#include <renderer/shaders/passthrough/passthrough.h>

#include <graphic/filters/texture/texture.h>

namespace null::render::filters {
	void c_texture::handle() {
		renderer->set_texture(texture);
		impl::shaders::passthrough_texture->use();

		child_command->handle();

		impl::shaders::passthrough_color->use();
		renderer->set_texture(nullptr);
	}
}