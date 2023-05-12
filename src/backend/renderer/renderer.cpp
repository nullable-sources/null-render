#include <backend/renderer/renderer.h>
#include <backend/internal/mesh.h>
#include <backend/internal/frame-buffer.h>
#include <backend/shaders/passthrough/passthrough.h>

#include <null-render.h>

namespace null::render::backend {
    void i_renderer::create_objects() {
        if(empty()) return;
        shaders::event_dispatcher.create();

        mesh->create();

        msaa_buffer->create();
        rendering_buffer->create();

        rtt_buffer->create();
        for(std::unique_ptr<i_frame_buffer>& postprocessing_buffer : postprocessing_buffers) {
            postprocessing_buffer->create();
        }

        create_atlases();
        create_internal_objects();
    }

    void i_renderer::destroy_objects() {
        if(empty()) return;
        shaders::event_dispatcher.destroy();

        mesh->destroy();

        msaa_buffer->destroy();
        rendering_buffer->destroy();

        rtt_buffer->destroy();
        for(std::unique_ptr<i_frame_buffer>& postprocessing_buffer : postprocessing_buffers) {
            postprocessing_buffer->destroy();
        }

        destroy_atlases();
        destroy_internal_objects();
    }

    void i_renderer::begin_render() {
        save_state();

        mesh->compile();

        setup_state();

        shaders::passthrough_color->use();

        if(render::shared::msaa_quality != 0) {
            msaa_buffer->set();
            msaa_buffer->clear();
            msaa_buffer->copy_from(rendering_buffer);
        }

        background.handle();
        background.clear();
    }

    void i_renderer::end_render() {
        foreground.handle();
        foreground.clear();

        if(render::shared::msaa_quality != 0) {
            rendering_buffer->set();
            rendering_buffer->copy_from(msaa_buffer);
        }

        mesh->clear_geometry();

        restore_state();
    }

    void i_renderer::create_atlases() {
        if(!atlases_handler.changed) return;

        for(c_atlas* atlas : atlases_handler.atlases) {
            if(atlas->texture.data) destroy_texture(atlas->texture.data);

            if(atlas->texture.pixels_alpha8.empty()) {
                if(atlas->configs.empty()) atlas->add_font_default();
                atlas->build();
            }

            atlas->texture.get_data_as_rgba32();
            atlas->texture.data = create_texture(atlas->texture.size, atlas->texture.pixels_rgba32.data());
        }

        atlases_handler.changed = false;
    }

    void i_renderer::destroy_atlases() {
        for(c_atlas* atlas : atlases_handler.atlases)
            destroy_texture(atlas->texture.data);
        atlases_handler.changed = true;
    }
}