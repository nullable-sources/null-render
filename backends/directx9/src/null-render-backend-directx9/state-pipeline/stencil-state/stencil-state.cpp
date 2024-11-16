#include "../../renderer/renderer.h"
#include "stencil-state.h"

namespace ntl::render::directx9 {
    D3DSTENCILOP c_stencil_state::to_backend_op(const backend::e_stencil_op& op) {
        switch(op) {
            case backend::e_stencil_op::zero: return D3DSTENCILOP_ZERO;
            case backend::e_stencil_op::keep: return D3DSTENCILOP_KEEP;
            case backend::e_stencil_op::replace: return D3DSTENCILOP_REPLACE;
            case backend::e_stencil_op::invert: return D3DSTENCILOP_INVERT;
            case backend::e_stencil_op::incr: return D3DSTENCILOP_INCR;
            case backend::e_stencil_op::incr_sat: return D3DSTENCILOP_INCRSAT;
            case backend::e_stencil_op::decr: return D3DSTENCILOP_DECR;
            case backend::e_stencil_op::decr_sat: return D3DSTENCILOP_DECRSAT;
        }
    }

    D3DCMPFUNC c_stencil_state::to_backend_comp(const backend::e_stencil_comp& comp) {
        switch(comp) {
            case backend::e_stencil_comp::never: return D3DCMP_NEVER;
            case backend::e_stencil_comp::always: return D3DCMP_ALWAYS;
            case backend::e_stencil_comp::greater: return D3DCMP_GREATER;
            case backend::e_stencil_comp::less: return D3DCMP_LESS;
            case backend::e_stencil_comp::equal: return D3DCMP_EQUAL;
            case backend::e_stencil_comp::less_equal: return D3DCMP_LESSEQUAL;
            case backend::e_stencil_comp::greater_equal: return D3DCMP_GREATEREQUAL;
            case backend::e_stencil_comp::not_equal: return D3DCMP_NOTEQUAL;
        }
    }

    void c_stencil_state::use() {
        if(overridden_hash == 0) return;

        if(!enable.has_inherit()) shared.device->SetRenderState(D3DRS_STENCILENABLE, enable.get());
        if(!fail.has_inherit()) shared.device->SetRenderState(D3DRS_STENCILFAIL, to_backend_op(fail.get()));
        if(!zfail.has_inherit()) shared.device->SetRenderState(D3DRS_STENCILZFAIL, to_backend_op(zfail.get()));
        if(!pass.has_inherit()) shared.device->SetRenderState(D3DRS_STENCILPASS, to_backend_op(pass.get()));
        if(!comp.has_inherit()) shared.device->SetRenderState(D3DRS_STENCILFUNC, to_backend_comp(comp.get()));
        if(!ref.has_inherit()) shared.device->SetRenderState(D3DRS_STENCILREF, ref.get());
    }
}