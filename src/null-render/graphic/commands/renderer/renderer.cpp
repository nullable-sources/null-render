#include "../../../backend/renderer/renderer.h"
#include "../../../backend/state-pipeline/state-pipeline.h"
#include "renderer.h"

namespace null::render {
	void c_setup_state_command::handle() {
		backend::state_pipeline->setup_state();
	}

	void c_texture_command::handle() {
		backend::renderer->set_texture(texture);
	}

	void c_clip_command::handle() {
		backend::renderer->set_clip(rect);
	}

	void c_set_matrix_command::handle() {
		backend::renderer->set_matrix(matrix);
	}

	void c_update_matrix_command::handle() {
		backend::renderer->update_matrix(matrix);
	}

	void c_set_translation_command::handle() {
		backend::renderer->set_translation(translation);
	}

	void c_update_translation_command::handle() {
		backend::renderer->update_translation(translation);
	}
}