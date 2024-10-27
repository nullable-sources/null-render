#include "../internal/frame-buffer.h"
#include "../post-processing/post-processing.h"
#include "../state-pipeline/state-pipeline.h"

#include "renderer-pipeline.h"

namespace ntl::render::backend {
    void c_renderer_pipeline::begin() {
        renderer_event_dispatcher.begin_render();
        if(store_state_pipeline) state_pipeline->save_state();
        renderer_event_dispatcher.create();
        if(setup_state_pipeline) state_pipeline->setup_state();

        if(use_rendering_buffer && rendering_buffer) {
            state_pipeline->framebuffers.push(rendering_buffer);
            if(clear_rendering_buffer) rendering_buffer->clear();
        }

        if(use_intermediate_buffer && intermediate_buffer) {
            state_pipeline->framebuffers.push(intermediate_buffer);
            if(clear_intermediate_buffer) intermediate_buffer->clear();
        }

        std::ranges::for_each(flushing_draw_lists, std::bind(&c_draw_list::flush, std::placeholders::_1));
    }

    void c_renderer_pipeline::end() {
        if(use_intermediate_buffer && intermediate_buffer) {
            state_pipeline->framebuffers.pop();
            if(blit_intermediate_buffer) post_processing->blit_buffer(intermediate_buffer);
        }

        if(use_rendering_buffer && rendering_buffer) {
            state_pipeline->framebuffers.push(rendering_buffer);
            if(blit_rendering_buffer) post_processing->blit_buffer(rendering_buffer);
        }

        renderer_event_dispatcher.end_render();
        if(store_state_pipeline) state_pipeline->restore_state();
    }
}