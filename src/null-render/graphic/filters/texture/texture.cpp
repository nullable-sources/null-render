#include "../../../backend/state-pipeline/state-pipeline.h"
#include "../../../backend/shaders/passthrough.h"

#include "texture.h"

namespace null::render {
	void c_texture_filter::handle() {
		backend::state_pipeline->shaders.push(backend::passthrough_texture_shader);
		backend::state_pipeline->textures.push(texture);
		child_command->handle();
		backend::state_pipeline->textures.pop();
		backend::state_pipeline->shaders.pop();
	}
}