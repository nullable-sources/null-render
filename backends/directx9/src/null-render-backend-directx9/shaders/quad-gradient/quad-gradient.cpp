#include "quad-gradient.h"

namespace ntl::render::directx9 {
    void c_quad_gradient_shader::set_constants(const constants_t& constants) {
        for(auto [i, color] : constants.colors | std::views::enumerate) {
            pixel_shader->set_constant(i, color.cast<float>().channels.data(), 1);
        }
    }
}