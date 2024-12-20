#include "../internal/frame-buffer.h"
#include "../state-pipeline/state-pipeline.h"
#include "../post-processing/post-processing.h"

#include "../../graphic/draw-list/draw-list.h"

#include "renderer.h"

namespace ntl::render::backend {
    void i_renderer::update_uniforms() {
        if(state_pipeline->shaders.empty()) return;
        state_pipeline->shaders.append_last();
    }

    void i_renderer::create_objects() {
        if(empty()) return;

        create_atlases();
        create_internal_objects();
    }

    void i_renderer::destroy_objects() {
        if(empty()) return;

        renderer_event_dispatcher.destroy();

        destroy_atlases();
        destroy_internal_objects();
    }

    void i_renderer::begin_resize_viewport(const vec2_t<int>& new_viewport) {
        shared::viewport = new_viewport;
        renderer_event_dispatcher.viewport_resize_begin();
    }

    void i_renderer::end_resize_viewport() {
        renderer_event_dispatcher.viewport_resize_end();
    }

    std::unique_ptr<std::uint8_t[]> i_renderer::premultiply_texture_color(const vec2_t<float>& size, std::uint8_t* data) {
        const size_t data_size = size.x * size.y * 4;
        std::unique_ptr<std::uint8_t[]> premultiplied(new std::uint8_t[data_size]);
        if(!data) return std::move(premultiplied);

        for(size_t i = 0; i < data_size; i += 4) {
            const std::uint8_t alpha = data[i + 3];
            for(size_t j = 0; j < 3; j++)
                premultiplied[i + j] = int(data[i + j]) * int(alpha) / 255;
            premultiplied[i + 3] = alpha;
        }

        return std::move(premultiplied);
    }

    void i_renderer::create_atlases() {
        if(!atlases_handler.changed) return;

        for(c_atlas* atlas : atlases_handler.atlases) {
            c_atlas::texture_t& texture = atlas->texture;
            if(texture.data) destroy_texture(texture.data);

            if(!texture.is_built()) {
                if(atlas->configs.empty()) {
                    font_config_t default_config{ };
                    default_config.load_font_default();
                    atlas->add_font(default_config);
                }
                atlas->build();
            }

            if(!texture.is_built()) {
                sdk::logger(sdk::e_log_type::warning, "the font atlas texture could not be created because the pixel array is empty.");
                continue;
            }

            texture.data = create_texture(texture.size, texture.pixels_rgba32.data());
        }

        atlases_handler.changed = false;
    }

    void i_renderer::destroy_atlases() {
        for(c_atlas* atlas : atlases_handler.atlases) {
            destroy_texture(atlas->texture.data);
            atlas->texture.data = nullptr;
        }
        atlases_handler.changed = true;
    }
}