#include <null-render/backend/state-pipeline/state-pipeline.h>
#include "../../wrapper/opengl3.h"
#include "blend-state.h"

namespace ntl::render::opengl3 {
    c_blend_state* c_blend_state::previous_state() {
        return (c_blend_state*)backend::state_pipeline->blends.previous();
    }

    std::uint32_t c_blend_state::to_backend_blend(const backend::e_blend& blend) {
        switch(blend) {
            case backend::e_blend::zero: return 0;
            case backend::e_blend::one: return 1;

            case backend::e_blend::constant_factor: return opengl::e_constant_alpha;
            case backend::e_blend::inv_constant_factor: return opengl::e_one_minus_constant_alpha;

            case backend::e_blend::src_color: return opengl::e_src_color;
            case backend::e_blend::inv_src_color: return opengl::e_one_minus_src_color;
            case backend::e_blend::src_alpha: return opengl::e_src_alpha;
            case backend::e_blend::inv_src_alpha: return opengl::e_one_minus_src_alpha;

            case backend::e_blend::dst_color: return opengl::e_dst_color;
            case backend::e_blend::inv_dst_color: return opengl::e_one_minus_dst_color;
            case backend::e_blend::dst_alpha: return opengl::e_dst_alpha;
            case backend::e_blend::inv_dst_alpha: return opengl::e_one_minus_dst_alpha;
        }
    }

    void c_blend_state::use() {
        update_inheritance();
        if(overridden_hash == 0) return;

        if(!blend_enable.has_inherit()) blend_enable.get() ? opengl::enable(opengl::e_blend) : opengl::disable(opengl::e_blend);
        if(!src_blend.has_inherit() || !dst_blend.has_inherit() || !src_alpha_blend.has_inherit() || !dst_alpha_blend.has_inherit()) {
            backend::e_blend src_blend_value{ };
            opengl::blend_func_separate(
                to_backend_blend(inherit(&c_blend_state::src_blend)), to_backend_blend(inherit(&c_blend_state::dst_blend)),
                to_backend_blend(inherit(&c_blend_state::src_alpha_blend)), to_backend_blend(inherit(&c_blend_state::dst_alpha_blend))
            );
        }
        if(!factor.has_inherit()) opengl::blend_color(0.f, 0.f, 0.f, factor.get());
        if(!disable_color_write.has_inherit()) disable_color_write.get() ? opengl::color_mask(false, false, false, false) : opengl::color_mask(true, true, true, true);
    }
}