#pragma once
#include <null-render/backend/state-pipeline/stencil-state.h>
#include "../../wrapper/state-machine/stencil-state-instances-stack/stencil-state-instances-stack.h"

namespace ntl::render::directx11 {
    class c_stencil_state : public i_state_machine<backend::i_stencil_state, ID3D11DepthStencilState, dx_stencil_desc> {
    private:
        static inline c_stencil_state_instances_stack instances_stack{ };

    public:
        static inline D3D11_STENCIL_OP to_backend_op(const backend::e_stencil_op& op);
        static inline D3D11_COMPARISON_FUNC to_backend_comp(const backend::e_stencil_comp& comp);

    private:
        using state_machine_t = i_state_machine<backend::i_stencil_state, ID3D11DepthStencilState, dx_stencil_desc>;

    public:
        c_stencil_state() : i_state_machine(D3D11_DEPTH_STENCIL_DESC{
            .DepthEnable{ false },
            .DepthWriteMask{ D3D11_DEPTH_WRITE_MASK_ALL },
            .DepthFunc{ D3D11_COMPARISON_LESS },
            .StencilReadMask{ D3D11_DEFAULT_STENCIL_READ_MASK },
            .StencilWriteMask{ D3D11_DEFAULT_STENCIL_WRITE_MASK }
            }) { }


    protected:
        virtual state_instances_stack_t& get_stack() const override { return instances_stack; }
        virtual state_machine_t* get_previous_state() const override;

        virtual void set_state_object(ID3D11DepthStencilState* object, const dx_stencil_desc& desc) override {
            shared.context->OMSetDepthStencilState(object, desc.ref);
        }
        virtual void append_state_to_desc(dx_stencil_desc& desc) override;

    public:
        virtual std::unique_ptr<backend::i_stencil_state> clone() const override {
            return std::unique_ptr<c_stencil_state>(new c_stencil_state(*this));
        }
    };
}