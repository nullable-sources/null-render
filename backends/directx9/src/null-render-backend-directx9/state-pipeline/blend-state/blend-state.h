#pragma once
#include <d3d9types.h>

#include <null-render/backend/state-pipeline/blend-state.h>

namespace ntl::render::directx9 {
    class c_blend_state : public backend::i_blend_state {
    public:
        static inline D3DBLEND to_backend_blend(const backend::e_blend& blend);

    public:
        virtual void use() override;

    public:
        virtual std::unique_ptr<backend::i_blend_state> clone() const override {
            return std::unique_ptr<c_blend_state>(new c_blend_state(*this));
        }
    };
}