#pragma once
#include <null-render/backend/state-pipeline/stencil-state.h>
#include "../../wrapper/recursively-inherited-state.h"

namespace ntl::render::opengl3 {
    class c_stencil_state : public backend::i_stencil_state, private c_recursively_inherited_state<c_stencil_state> {
    public:
        friend class c_recursively_inherited_state<c_stencil_state>;

    protected:
        static c_stencil_state* previous_state();

    public:
        static inline std::uint32_t to_backend_op(const backend::e_stencil_op& op);
        static inline std::uint32_t to_backend_comp(const backend::e_stencil_comp& comp);

    public:
        virtual void use() override;

    public:
        virtual std::unique_ptr<backend::i_stencil_state> clone() const override {
            return std::unique_ptr<c_stencil_state>(new c_stencil_state(*this));
        }
    };
}