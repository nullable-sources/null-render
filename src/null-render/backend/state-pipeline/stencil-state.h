#pragma once
#include "../internal/object.h"
#include "state-field.h"

namespace ntl::render::backend {
    enum class e_stencil_op {
        zero, keep, replace, invert,
        incr, incr_sat, decr, decr_sat
    };

    enum class e_stencil_comp {
        never, always,
        greater,
        less, equal,
        less_equal, greater_equal,
        not_equal
    };

    class i_stencil_state : public i_usable_object {
    protected:
        std::size_t overridden_hash{ };
        bool locked{ };

    public:
        state_field_t<bool> enable{ }, depth_test{ };
        state_field_t<e_stencil_op> fail{ }, zfail{ }, pass{ };
        state_field_t<e_stencil_comp> comp{ };
        state_field_t<int> ref{ };

    public:
        i_stencil_state() { }
        virtual ~i_stencil_state() { }

    public:
        virtual void lock() {
            locked = true;
            enable.to_hash(overridden_hash);
            depth_test.to_hash(overridden_hash);
            fail.to_hash(overridden_hash);
            zfail.to_hash(overridden_hash);
            pass.to_hash(overridden_hash);
            comp.to_hash(overridden_hash);
            ref.to_hash(overridden_hash);
        }

        virtual void unlock() {
            locked = false;
            overridden_hash = 0;
        }

    public:
        virtual std::unique_ptr<i_stencil_state> clone() const = 0;
    }; inline std::unique_ptr<i_stencil_state> default_stencil_state{ };
}