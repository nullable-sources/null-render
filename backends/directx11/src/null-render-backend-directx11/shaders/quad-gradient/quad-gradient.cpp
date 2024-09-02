#include "quad-gradient.h"

namespace ntl::render::directx11 {
    void c_quad_gradient_shader::use() {
        if(empty()) return;
        c_shader::use();

        color_vertex_shader_object.set_constant({ backend::renderer->get_matrix(), backend::renderer->get_translation() });
    }

    void c_quad_gradient_shader::set_constants(const constants_t& constants) {
        quad_gradient_shader_object.set_constant(constants);
    }
}