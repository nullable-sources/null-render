#pragma once
#include <null-render/backend/state-pipeline/blend-state.h>
#include "../../wrapper/recursively-inherited-state.h"

namespace ntl::render::opengl3 {
    class c_blend_state : public backend::i_blend_state, private c_recursively_inherited_state<c_blend_state> {
    public:
        friend class c_recursively_inherited_state<c_blend_state>;

    protected:
        static c_blend_state* previous_state();

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