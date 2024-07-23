#pragma once
#include <null-render/backend/state-pipeline/blend-state.h>

namespace null::render::opengl3 {
    class c_blend_state : public backend::i_blend_state {
    public:
        static inline std::uint32_t to_backend_blend(const backend::e_blend& blend);

    public:
        virtual void use() override;

    public:
        virtual std::unique_ptr<backend::i_blend_state> clone() const override {
            return std::unique_ptr<c_blend_state>(new c_blend_state(*this));
        }
    };
}