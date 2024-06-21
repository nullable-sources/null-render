#include "../../../backend/state-pipeline/state-pipeline.h"

#include "radial-gradient.h"

namespace null::render {
    void c_radial_gradient_filter::handle() {
        backend::radial_gradient_shader->set_constants(constants);
        backend::state_pipeline->shaders.push(backend::radial_gradient_shader);
        child_command->handle();
        backend::state_pipeline->shaders.pop();
    }
}