#include "blur.h"

namespace null::render::directx11 {
    void c_blur_shader::set_direction(const vec2_t<float>& direction) {
        blur_shader_object.edit_direction(direction);
    }

    void c_blur_shader::set_constants(const constants_t& constants) {
        c_blur_shader_object::constant_buffer_t constant_buffer{ };

        for(auto [chunk_index, chunk] : std::views::zip(constants.weights, constants.offsets) | std::views::take(constants.iterations) | std::views::chunk(4) | std::views::enumerate) {
            for(auto [element_index, chunk_element] : chunk | std::views::enumerate) {
                constant_buffer.weights[chunk_index][element_index] = std::get<0>(chunk_element);
                constant_buffer.offsets[chunk_index][element_index] = std::get<1>(chunk_element);
            }
        }

        constant_buffer.iterations = constants.iterations;
        constant_buffer.texel_size = constants.texel_size;
        constant_buffer.uv_limits = constants.uv_limits;
        blur_shader_object.set_constant(constant_buffer);
    }
}