#include <null-render/backend/state-pipeline/state-pipeline.h>
#include "stencil-state.h"

namespace ntl::render::directx11 {
    D3D11_STENCIL_OP c_stencil_state::to_backend_op(const backend::e_stencil_op& op) {
        switch(op) {
            case backend::e_stencil_op::zero: return D3D11_STENCIL_OP_ZERO;
            case backend::e_stencil_op::keep: return D3D11_STENCIL_OP_KEEP;
            case backend::e_stencil_op::replace: return D3D11_STENCIL_OP_REPLACE;
            case backend::e_stencil_op::invert: return D3D11_STENCIL_OP_INVERT;
            case backend::e_stencil_op::incr: return D3D11_STENCIL_OP_INCR;
            case backend::e_stencil_op::incr_sat: return D3D11_STENCIL_OP_INCR_SAT;
            case backend::e_stencil_op::decr: return D3D11_STENCIL_OP_DECR;
            case backend::e_stencil_op::decr_sat: return D3D11_STENCIL_OP_DECR_SAT;
        }
    }

    D3D11_COMPARISON_FUNC c_stencil_state::to_backend_comp(const backend::e_stencil_comp& comp) {
        switch(comp) {
            case backend::e_stencil_comp::never: return D3D11_COMPARISON_NEVER;
            case backend::e_stencil_comp::always: return D3D11_COMPARISON_ALWAYS;
            case backend::e_stencil_comp::greater: return D3D11_COMPARISON_GREATER;
            case backend::e_stencil_comp::less: return D3D11_COMPARISON_LESS;
            case backend::e_stencil_comp::equal: return D3D11_COMPARISON_EQUAL;
            case backend::e_stencil_comp::less_equal: return D3D11_COMPARISON_LESS_EQUAL;
            case backend::e_stencil_comp::greater_equal: return D3D11_COMPARISON_GREATER_EQUAL;
            case backend::e_stencil_comp::not_equal: return D3D11_COMPARISON_NOT_EQUAL;
        }
    }

    c_stencil_state::state_machine_t* c_stencil_state::get_previous_state() const {
        return (c_stencil_state::state_machine_t*)backend::state_pipeline->stencils.previous();
    }

    void c_stencil_state::append_state_to_desc(dx_stencil_desc& desc) {
        if(!enable.has_inherit()) desc.StencilEnable = enable.get();
        if(!fail.has_inherit()) desc.BackFace.StencilFailOp = desc.FrontFace.StencilFailOp = to_backend_op(fail.get());
        if(!zfail.has_inherit()) desc.BackFace.StencilDepthFailOp = desc.FrontFace.StencilDepthFailOp = to_backend_op(zfail.get());
        if(!pass.has_inherit()) desc.BackFace.StencilPassOp = desc.FrontFace.StencilPassOp = to_backend_op(pass.get());
        if(!comp.has_inherit()) desc.BackFace.StencilFunc = desc.FrontFace.StencilFunc = to_backend_comp(comp.get());
        if(!ref.has_inherit()) desc.ref = ref.get();
    }
}