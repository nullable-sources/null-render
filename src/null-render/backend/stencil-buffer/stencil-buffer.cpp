#include "../backend.h"

#include "stencil-buffer.h"

namespace ntl::render::backend {
    i_stencil_buffer::i_stencil_buffer() {
        renderer_event_dispatcher.attach_listener(e_renderer_event_type::viewport_resize_begin, this);
        renderer_event_dispatcher.attach_listener(e_renderer_event_type::viewport_resize_end, this);
        renderer_event_dispatcher.attach_listener(e_renderer_event_type::create, this);
        renderer_event_dispatcher.attach_listener(e_renderer_event_type::destroy, this);

        disable_write_blend = backend::factory->instance_blend_state();
        disable_write_blend->unlock();
        disable_write_blend->disable_color_write.set(true);
        disable_write_blend->lock();

        stencil_test_state = backend::factory->instance_stencil_state();
        stencil_test_state->unlock();
        stencil_test_state->enable.set(true);
        stencil_test_state->fail.set(backend::e_stencil_op::keep);
        stencil_test_state->zfail.set(backend::e_stencil_op::keep);
        stencil_test_state->pass.set(backend::e_stencil_op::keep);
        stencil_test_state->comp.set(backend::e_stencil_comp::equal);
        stencil_test_state->lock();

        stencil_set_write_state = backend::factory->instance_stencil_state();
        stencil_set_write_state->unlock();
        stencil_set_write_state->enable.set(true);
        stencil_set_write_state->fail.set(backend::e_stencil_op::keep);
        stencil_set_write_state->zfail.set(backend::e_stencil_op::keep);
        stencil_set_write_state->pass.set(backend::e_stencil_op::replace);
        stencil_set_write_state->comp.set(backend::e_stencil_comp::always);
        stencil_set_write_state->ref.set(1);
        stencil_set_write_state->lock();

        stencil_intersect_write_state = stencil_set_write_state->clone();
        stencil_intersect_write_state->unlock();
        stencil_intersect_write_state->pass.set(backend::e_stencil_op::incr_sat);
        stencil_intersect_write_state->lock();
    }

    void i_stencil_buffer::set_test(bool test) {
        if(test == already_testing) return;
        if(test) backend::state_pipeline->stencils.push(stencil_test_state);
        else backend::state_pipeline->stencils.pop();
        already_testing = test;
    }

    void i_stencil_buffer::set_operation(backend::e_stencil_operation operation) {
        if(operation == backend::e_stencil_operation::end_write) {
            backend::state_pipeline->blends.append_last();

            stencil_test_state->unlock();
            stencil_test_state->ref.set(stencil_ref);
            stencil_test_state->lock();

            if(already_testing) stencil_test_state->use();
            else backend::state_pipeline->stencils.append_last();
            return;
        }

        disable_write_blend->use();

        switch(operation) {
            case backend::e_stencil_operation::set:
            case backend::e_stencil_operation::set_inverse: {
                stencil_set_write_state->use();
                stencil_ref = operation == backend::e_stencil_operation::set ? 1 : 0;
            } break;

            case backend::e_stencil_operation::intersect: {
                stencil_intersect_write_state->use();
                stencil_ref++;
            } break;
        }
    }
}