#include "../../wrapper/opengl3.h"
#include "rasterizer-state.h"

namespace null::render::opengl3 {
    void c_rasterizer_state::use() {
        if(overridden_hash == 0) return;
        if(!msaa_disable.has_inherit()) msaa_disable.get() ? opengl::disable(opengl::e_multisample) : opengl::enable(opengl::e_multisample);
        if(!scissor_disable.has_inherit()) scissor_disable.get() ? opengl::disable(opengl::e_scissor_test) : opengl::enable(opengl::e_scissor_test);
    }
}