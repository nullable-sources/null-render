#include <renderer/renderer.h>
#include <graphic/draw-list/draw-list.h>

namespace null::render {
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