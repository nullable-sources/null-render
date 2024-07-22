#include "../../renderer/renderer.h"
#include "rasterizer-state.h"

namespace null::render::directx9 {
    void c_rasterizer_state::use() {
        if(overridden_hash == 0) return;
        if(!msaa_disable.has_inherit()) shared.device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, !msaa_disable.get());
        if(!scissor_disable.has_inherit()) shared.device->SetRenderState(D3DRS_SCISSORTESTENABLE, !scissor_disable.get());
    }
}