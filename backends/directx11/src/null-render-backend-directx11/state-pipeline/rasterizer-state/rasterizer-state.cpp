#include <null-render/backend/state-pipeline/state-pipeline.h>
#include "rasterizer-state.h"

namespace ntl::render::directx11 {
    c_rasterizer_state::state_machine_t* c_rasterizer_state::get_previous_state() const {
        return (c_rasterizer_state::state_machine_t*)backend::state_pipeline->rasterizers.previous();
    }

    void c_rasterizer_state::append_state_to_desc(dx_rasterizer_state_desc_t& desc) {
        if(!msaa_disable.has_inherit()) {
            desc.MultisampleEnable = !msaa_disable.get();
#ifdef NULL_RENDER_DX11_ALLOW_FEATURE_11_1
            desc.ForcedSampleCount = msaa_disable.get() ? 1 : 0;
#endif
        }
        if(!scissor_disable.has_inherit()) desc.ScissorEnable = !scissor_disable.get();
    }
}