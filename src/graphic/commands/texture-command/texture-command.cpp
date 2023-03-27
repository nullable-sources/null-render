#include <backend/renderer/renderer.h>
#include <graphic/commands/texture-command/texture-command.h>

namespace null::render::commands {
	void c_texture::handle() {
		backend::renderer->set_texture(texture);
	}
}