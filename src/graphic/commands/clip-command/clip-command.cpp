#include <renderer/renderer.h>
#include <graphic/commands/clip-command/clip-command.h>

namespace null::render::commands {
	void c_clip::handle() {
		renderer->set_clip(rect);
	}
}