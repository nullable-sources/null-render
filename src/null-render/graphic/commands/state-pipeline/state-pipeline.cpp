#include "state-pipeline.h"

namespace null::render {
    void c_setup_state_command::handle() {
        backend::state_pipeline->setup_state();
    }

    void c_rasterizer_push_command::handle() {
        backend::state_pipeline->rasterizers.push(rasterizer_state);
    }

    void c_rasterizer_pop_command::handle() {
        backend::state_pipeline->rasterizers.pop();
    }

    void c_blend_push_command::handle() {
        backend::state_pipeline->blends.push(blend_state);
    }

    void c_blend_pop_command::handle() {
        backend::state_pipeline->blends.pop();
    }
}