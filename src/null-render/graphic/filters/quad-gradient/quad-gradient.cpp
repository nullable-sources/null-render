#include "../../../backend/state-pipeline/state-pipeline.h"

#include "quad-gradient.h"

namespace null::render {
	void c_quad_gradient_filter::handle() {
		backend::quad_gradient_shader->set_constants(constants);
		backend::state_pipeline->shaders.push(backend::quad_gradient_shader);
		child_command->handle();
		backend::state_pipeline->shaders.pop();
	}
}