#pragma once
#include <d3d9types.h>

#include <null-render/backend/state-pipeline/stencil-state.h>

namespace ntl::render::directx9 {
    class c_stencil_state : public backend::i_stencil_state {
    public:
        static inline D3DSTENCILOP to_backend_op(const backend::e_stencil_op& op);
        static inline D3DCMPFUNC to_backend_comp(const backend::e_stencil_comp& comp);

    public:
        virtual void use() override;

    public:
        virtual std::unique_ptr<backend::i_stencil_state> clone() const override {
            return std::unique_ptr<c_stencil_state>(new c_stencil_state(*this));
        }
    };
}