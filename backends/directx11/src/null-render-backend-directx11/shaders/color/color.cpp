#include "color.h"

namespace ntl::render::directx11 {
    void c_color_shader::use() {
        if(empty()) return;
        c_shader::use();

        color_vertex_shader_object.set_constant({ backend::renderer->get_matrix(), backend::renderer->get_translation() });
    }
}