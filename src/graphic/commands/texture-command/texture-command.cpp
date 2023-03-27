#include <renderer/renderer.h>
#include <graphic/commands/texture-command/texture-command.h>

namespace null::render::commands {
	void c_texture::handle() {
		renderer->set_texture(texture);
	}
}