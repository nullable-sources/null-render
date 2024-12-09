#include "../../wrapper/opengl3.h"
#include "stencil-state.h"

namespace ntl::render::opengl3 {
    c_stencil_state* c_stencil_state::previous_state() {
        return (c_stencil_state*)backend::state_pipeline->stencils.previous();
    }

    std::uint32_t c_stencil_state::to_backend_op(const backend::e_stencil_op& op) {
        switch(op) {
            case backend::e_stencil_op::zero: return 0;
            case backend::e_stencil_op::keep: return opengl::e_keep;
            case backend::e_stencil_op::replace: return opengl::e_replace;
            case backend::e_stencil_op::invert: return opengl::e_invert;
            case backend::e_stencil_op::incr: return opengl::e_incr_wrap;
            case backend::e_stencil_op::incr_sat: return opengl::e_incr;
            case backend::e_stencil_op::decr: return opengl::e_decr_wrap;
            case backend::e_stencil_op::decr_sat: return opengl::e_decr;
        }
    }

    std::uint32_t c_stencil_state::to_backend_comp(const backend::e_stencil_comp& comp) {
        switch(comp) {
            case backend::e_stencil_comp::never: return opengl::e_never;
            case backend::e_stencil_comp::always: return opengl::e_always;
            case backend::e_stencil_comp::greater: return opengl::e_greater;
            case backend::e_stencil_comp::less: return opengl::e_less;
            case backend::e_stencil_comp::equal: return opengl::e_equal;
            case backend::e_stencil_comp::less_equal: return opengl::e_lequal;
            case backend::e_stencil_comp::greater_equal: return opengl::e_gequal;
            case backend::e_stencil_comp::not_equal: return opengl::e_notequal;
        }
    }

    void c_stencil_state::use() {
        update_inheritance();
        if(overridden_hash == 0) return;

        if(!depth_test.has_inherit()) {
            depth_test.get() ? opengl::enable(opengl::e_depth_test) : opengl::disable(opengl::e_depth_test);
            opengl::depth_mask(depth_test.get());
        }
        if(!enable.has_inherit()) enable.get() ? opengl::enable(opengl::e_stencil_test) : opengl::disable(opengl::e_stencil_test);
        if(!fail.has_inherit() || !zfail.has_inherit() || !pass.has_inherit())
            opengl::stencil_op(
                to_backend_op(inherit(&c_stencil_state::fail)),
                to_backend_op(inherit(&c_stencil_state::zfail)),
                to_backend_op(inherit(&c_stencil_state::pass))
            );
        if(!comp.has_inherit() || !ref.has_inherit())
            opengl::stencil_func(to_backend_comp(inherit(&c_stencil_state::comp)), inherit(&c_stencil_state::ref), std::uint32_t(-1));
    }
}