#include <shaders/shaders.h>

namespace null::render::shaders {
    void setup_state() {
        vertex::shader.set_shader();

        {
            float matrix[4][4] = {
                { 2.f / renderer::draw_data_t::screen_size.x,   0.f,                                            0.f,    0.f },
                { 0.f,                                          -(2.f / renderer::draw_data_t::screen_size.y),  0.f,    0.f },
                { 0.f,                                          0.f,                                            0.5f,   0.f },
                { -1.f,                                         1,                                              0.5f,   1.f },
            };

            memcpy(&vertex::constants.matrix, matrix, sizeof(matrix));
            vertex::shader.edit_constant(vertex::constants);
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