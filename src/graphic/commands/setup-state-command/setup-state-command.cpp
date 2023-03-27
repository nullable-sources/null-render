#include <renderer/renderer.h>
#include <graphic/commands/setup-state-command/setup-state-command.h>

namespace null::render::commands {
	void c_setup_state::handle() {
		renderer->setup_state();
	}
}