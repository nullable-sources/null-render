#include "../internal/frame-buffer.h"
#include "../post-processing/post-processing.h"
#include "../state-pipeline/state-pipeline.h"
#include "../../graphic/draw-list/draw-list.h"

#include "renderer-pipeline.h"

namespace ntl::render::backend {
    void c_renderer_pipeline::begin() {
        renderer_event_dispatcher.begin_render();
        if(store_state_pipeline) state_pipeline->save_state();
        renderer_event_dispatcher.create();
        if(setup_state_pipeline) state_pipeline->setup_state();

        if(use_intermediate_buffer && intermediate_buffer) {
            state_pipeline->framebuffers.push(intermediate_buffer);
            if(clear_intermediate_buffer) intermediate_buffer->clear();
        }

        if(flush_default_draw_list) draw_list->flush();
    }

    void c_renderer_pipeline::end() {
        if(use_intermediate_buffer && intermediate_buffer) {
            state_pipeline->framebuffers.pop();
            if(blit_intermediate_buffer) post_processing->blit_buffer(msaa_buffer.get());
        }

        renderer_event_dispatcher.end_render();
        if(store_state_pipeline) state_pipeline->restore_state();
    }
}