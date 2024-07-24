#include "../../renderer/renderer.h"
#include "blend-state.h"

namespace null::render::directx9 {
    D3DBLEND c_blend_state::to_backend_blend(const backend::e_blend& blend) {
        switch(blend) {
            case backend::e_blend::zero: return D3DBLEND_ZERO;
            case backend::e_blend::one: return D3DBLEND_ONE;

            case backend::e_blend::constant_factor: return D3DBLEND_BLENDFACTOR;
            case backend::e_blend::inv_constant_factor: return D3DBLEND_INVBLENDFACTOR;

            case backend::e_blend::src_color: return D3DBLEND_SRCCOLOR;
            case backend::e_blend::inv_src_color: return D3DBLEND_INVSRCCOLOR;
            case backend::e_blend::src_alpha: return D3DBLEND_SRCALPHA;
            case backend::e_blend::inv_src_alpha: return D3DBLEND_INVSRCALPHA;

            case backend::e_blend::dst_color: return D3DBLEND_DESTCOLOR;
            case backend::e_blend::inv_dst_color: return D3DBLEND_INVDESTCOLOR;
            case backend::e_blend::dst_alpha: return D3DBLEND_DESTALPHA;
            case backend::e_blend::inv_dst_alpha: return D3DBLEND_INVDESTALPHA;
        }
    }

    void c_blend_state::use() {
        if(overridden_hash == 0) return;
        if(!blend_enable.has_inherit()) shared.device->SetRenderState(D3DRS_ALPHABLENDENABLE, blend_enable.get());
        if(!src_blend.has_inherit()) shared.device->SetRenderState(D3DRS_SRCBLEND, to_backend_blend(src_blend.get()));
        if(!dst_blend.has_inherit()) shared.device->SetRenderState(D3DRS_DESTBLEND, to_backend_blend(dst_blend.get()));
        if(!src_alpha_blend.has_inherit()) shared.device->SetRenderState(D3DRS_SRCBLENDALPHA, to_backend_blend(src_alpha_blend.get()));
        if(!dst_alpha_blend.has_inherit()) shared.device->SetRenderState(D3DRS_DESTBLENDALPHA, to_backend_blend(dst_alpha_blend.get()));
        if(!dst_alpha_blend.has_inherit()) shared.device->SetRenderState(D3DRS_DESTBLENDALPHA, to_backend_blend(dst_alpha_blend.get()));
        if(!factor.has_inherit()) shared.device->SetRenderState(D3DRS_BLENDFACTOR, D3DCOLOR_COLORVALUE(factor.get(), factor.get(), factor.get(), factor.get()));
        if(!disable_color_write.has_inherit()) shared.device->SetRenderState(D3DRS_COLORWRITEENABLE, disable_color_write.get() ? 0 : 0xf);
    }
}