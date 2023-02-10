#include <shaders/shaders.h>

namespace null::render::shaders {
    void setup_state() {
        vertex::shader.set_shader();

        {
            vertex::constant vertex_const{ matrix4x4_t::project_ortho(0.f, renderer::draw_data_t::screen_size.x, renderer::draw_data_t::screen_size.y, 0.f, -10000.f, 10000.f) };

            vertex::shader.edit_constant(vertex_const);
        }

        vertex::shader.set_constant();
        pixel::shader.set_shader();
    }

    void create_shaders() {
        vertex::create_shader_control();
        pixel::create_shader_control();
    }

    void release_shaders() {
        vertex::release_shader_control();
        pixel::release_shader_control();
    }

    void win_proc() {
        release_shaders();
    }
}