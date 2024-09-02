#pragma once
#include "../internal/object.h"
#include "state-field.h"

namespace ntl::render::backend {
    class i_rasterizer_state : public i_usable_object {
    protected:
        std::size_t overridden_hash{ };
        bool locked{ };

    public:
        state_field_t<bool> msaa_disable{ };
        state_field_t<bool> scissor_disable{ };

    public:
        i_rasterizer_state() { }
        virtual ~i_rasterizer_state() { }

    public:
        virtual void lock() {
            locked = true;
            msaa_disable.to_hash(overridden_hash);
            scissor_disable.to_hash(overridden_hash);
        }

        virtual void unlock() {
            locked = false;
            overridden_hash = 0;
        }

    public:
        virtual std::unique_ptr<i_rasterizer_state> clone() const = 0;
    }; inline std::unique_ptr<i_rasterizer_state> default_rasterizer_state{ };
}