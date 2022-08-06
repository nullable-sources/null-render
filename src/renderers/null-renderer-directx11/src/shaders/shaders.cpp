#include <shaders/shaders.h>

namespace null::render::shaders {
    void setup_render_state(c_draw_list::draw_data_t* _draw_data) {
        vertex::shader.set_shader();

        {
            float L = _draw_data->window_pos.x;
            float R = _draw_data->window_pos.x + _draw_data->window_size.x;
            float T = _draw_data->window_pos.y;
            float B = _draw_data->window_pos.y + _draw_data->window_size.y;
            float matrix[4][4] =
            {
                { 2.0f / (R - L),    0.0f,              0.0f, 0.0f },
                { 0.0f,              2.0f / (T - B),    0.0f, 0.0f },
                { 0.0f,              0.0f,              0.5f, 0.0f },
                { (R + L) / (L - R), (T + B) / (B - T), 0.5f, 1.0f },
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