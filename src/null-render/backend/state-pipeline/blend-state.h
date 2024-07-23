#pragma once
#include "../internal/object.h"
#include "state-field.h"

namespace null::render::backend {
    enum class e_blend {
        zero, one,
        src_color, inv_src_color, src_alpha, inv_src_alpha,
        dst_color, inv_dst_color, dst_alpha, inv_dst_alpha
    };

    class i_blend_state : public i_usable_object {
    protected:
        std::size_t overridden_hash{ };
        bool locked{ };

    public:
        state_field_t<bool> blend_enable{ };
        state_field_t<e_blend> src_blend{ }, dst_blend{ };
        state_field_t<e_blend> src_alpha_blend{ }, dst_alpha_blend{ };
        state_field_t<bool> disable_color_write{ };

    public:
        i_blend_state() { }
        virtual ~i_blend_state() { }

    public:
        virtual void lock() {
            locked = true;
            blend_enable.to_hash(overridden_hash);
            src_blend.to_hash(overridden_hash);
            dst_blend.to_hash(overridden_hash);
            src_alpha_blend.to_hash(overridden_hash);
            dst_alpha_blend.to_hash(overridden_hash);
            disable_color_write.to_hash(overridden_hash);
        }

        virtual void unlock() {
            locked = false;
            overridden_hash = 0;
        }

    public:
        virtual std::unique_ptr<i_blend_state> clone() const = 0;
    }; inline std::unique_ptr<i_blend_state> default_blend_state{ };
}