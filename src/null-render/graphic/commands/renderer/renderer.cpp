#include "null-render/backend/renderer/renderer.h"
#include "renderer.h"

namespace null::render {
	void c_setup_state_command::handle() {
		backend::renderer->setup_state();
	}

	void c_texture_command::handle() {
		backend::renderer->set_texture(texture);
	}

	void c_matrix_command::handle() {
		backend::renderer->set_matrix(matrix);
	}

	void c_clip_command::handle() {
		backend::renderer->set_clip(rect);
	}
}