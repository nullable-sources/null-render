#pragma once
#include <null-render/backend/state-pipeline/blend-state.h>
#include "../../wrapper/state-machine/blend-state-instances-stack/blend-state-instances-stack.h"

namespace null::render::directx11 {
    class c_blend_state : public i_state_machine<backend::i_blend_state, ID3D11BlendState, dx_blend_factor_desc> {
    private:
        static inline c_blend_state_instances_stack instances_stack{ };

    public:
        static inline D3D11_BLEND to_backend_blend(const backend::e_blend& blend);

    private:
        using state_machine_t = i_state_machine<backend::i_blend_state, ID3D11BlendState, dx_blend_factor_desc>;

    public:
        c_blend_state() : i_state_machine(D3D11_BLEND_DESC{
            .AlphaToCoverageEnable{ false },
            .RenderTarget{ {
                    .BlendOp{ D3D11_BLEND_OP_ADD },
                    .BlendOpAlpha{ D3D11_BLEND_OP_ADD }
            } }
            }) { }


    protected:
        virtual state_instances_stack_t& get_stack() const override { return instances_stack; }
        virtual state_machine_t* get_previous_state() const override;

        virtual void set_state_object(ID3D11BlendState* object, const dx_blend_factor_desc& desc) override {
            std::array<float, 4> blend_factor{ };
            blend_factor.fill(desc.blend_factor);
            shared.context->OMSetBlendState(object, blend_factor.data(), -1);
        }
        virtual void append_state_to_desc(dx_blend_factor_desc& desc) override;

    public:
        virtual std::unique_ptr<backend::i_blend_state> clone() const override {
            return std::unique_ptr<c_blend_state>(new c_blend_state(*this));
        }
    };
}