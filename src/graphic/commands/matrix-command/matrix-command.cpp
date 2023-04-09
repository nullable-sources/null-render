#include <backend/renderer/renderer.h>
#include <graphic/commands/matrix-command/matrix-command.h>

namespace null::render::commands {
	void c_matrix::handle() {
		backend::renderer->set_matrix(matrix);
	}
}