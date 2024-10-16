#include "../renderer/renderer.h"
#include "state-pipeline.h"

namespace ntl::render::backend {
    void i_state_pipeline::c_framebuffers_pipeline::append() { stack.back()->use(); }
    void i_state_pipeline::c_framebuffers_pipeline::set_default() { state_pipeline->restore_framebuffer(); }

    void i_state_pipeline::c_shaders_pipeline::append() { stack.back()->use(); }
    void i_state_pipeline::c_shaders_pipeline::set_default() { state_pipeline->restore_shader(); }

    void i_state_pipeline::c_meshes_pipeline::append() { stack.back()->use(); }
    void i_state_pipeline::c_meshes_pipeline::set_default() { state_pipeline->restore_mesh(); }

    void i_state_pipeline::c_textures_pipeline::append() { renderer->set_texture(stack.back()); }
    void i_state_pipeline::c_textures_pipeline::set_default() { state_pipeline->restore_texture(); }

    void i_state_pipeline::c_rasterizers_pipeline::append() { stack.back()->use(); }
    void i_state_pipeline::c_rasterizers_pipeline::set_default() { state_pipeline->restore_rasterizer(); }

    void i_state_pipeline::c_blends_pipeline::append() { stack.back()->use(); }
    void i_state_pipeline::c_blends_pipeline::set_default() { state_pipeline->restore_blend(); }

    void i_state_pipeline::setup_state() {
        rasterizers.push(default_rasterizer_state);
        blends.push(default_blend_state);
        backend::renderer->set_clip({ { 0 }, shared::viewport });
        backend::renderer->set_matrix(backend::renderer->get_projection_matrix());
        backend::renderer->update_translation(0.f);
        backend::renderer->set_translation(0.f);
        backend::renderer_event_dispatcher.setup_state();
    }

    void i_state_pipeline::restore_state() {
        blends.pop();
        rasterizers.pop();
    }
}