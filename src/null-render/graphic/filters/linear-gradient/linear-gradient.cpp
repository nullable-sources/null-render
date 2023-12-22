#include "../../../backend/state-pipeline/state-pipeline.h"

#include "linear-gradient.h"

namespace null::render {
	void c_linear_gradient_filter::handle() {
		backend::linear_gradient_shader->set_constants(constants);
		backend::state_pipeline->shaders.push(backend::linear_gradient_shader);
		child_command->handle();
		backend::state_pipeline->shaders.pop();
	}
}