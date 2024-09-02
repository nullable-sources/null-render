#include <null-render/backend/state-pipeline/state-pipeline.h>
#include "blend-state.h"

namespace ntl::render::directx11 {
    D3D11_BLEND c_blend_state::to_backend_blend(const backend::e_blend& blend) {
        switch(blend) {
            case backend::e_blend::zero: return D3D11_BLEND_ZERO;
            case backend::e_blend::one: return D3D11_BLEND_ONE;

            case backend::e_blend::constant_factor: return D3D11_BLEND_BLEND_FACTOR;
            case backend::e_blend::inv_constant_factor: return D3D11_BLEND_INV_BLEND_FACTOR;

            case backend::e_blend::src_color: return D3D11_BLEND_SRC_COLOR;
            case backend::e_blend::inv_src_color: return D3D11_BLEND_INV_SRC_COLOR;
            case backend::e_blend::src_alpha: return D3D11_BLEND_SRC_ALPHA;
            case backend::e_blend::inv_src_alpha: return D3D11_BLEND_INV_SRC_ALPHA;

            case backend::e_blend::dst_color: return D3D11_BLEND_DEST_COLOR;
            case backend::e_blend::inv_dst_color: return D3D11_BLEND_INV_DEST_COLOR;
            case backend::e_blend::dst_alpha: return D3D11_BLEND_DEST_ALPHA;
            case backend::e_blend::inv_dst_alpha: return D3D11_BLEND_INV_DEST_ALPHA;
        }
    }

    c_blend_state::state_machine_t* c_blend_state::get_previous_state() const {
        return (c_blend_state::state_machine_t*)backend::state_pipeline->blends.previous();
    }

    void c_blend_state::append_state_to_desc(dx_blend_factor_desc& desc) {
        if(!blend_enable.has_inherit()) desc.RenderTarget[0].BlendEnable = blend_enable.get();
        if(!src_blend.has_inherit()) desc.RenderTarget[0].SrcBlend = to_backend_blend(src_blend.get());
        if(!dst_blend.has_inherit()) desc.RenderTarget[0].DestBlend = to_backend_blend(dst_blend.get());
        if(!src_alpha_blend.has_inherit()) desc.RenderTarget[0].SrcBlendAlpha = to_backend_blend(src_alpha_blend.get());
        if(!dst_alpha_blend.has_inherit()) desc.RenderTarget[0].DestBlendAlpha = to_backend_blend(dst_alpha_blend.get());
        if(!factor.has_inherit()) desc.blend_factor = factor.get();
        if(!disable_color_write.has_inherit()) desc.RenderTarget[0].RenderTargetWriteMask = disable_color_write.get() ? 0x0 : 0xf;
    }
}