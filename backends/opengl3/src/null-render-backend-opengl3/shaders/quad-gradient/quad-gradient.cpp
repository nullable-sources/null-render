#include "quad-gradient.h"

namespace ntl::render::opengl3 {
    void c_quad_gradient_shader::create() {
        if(!empty()) return;
        c_default_shader::create();

        top_left_color.get_location(program.get(), "color_tl");
        top_right_color.get_location(program.get(), "color_tr");
        bottom_right_color.get_location(program.get(), "color_br");
        bottom_left_color.get_location(program.get(), "color_bl");
    }

    void c_quad_gradient_shader::use() {
        if(empty()) return;
        c_default_shader::use();

        top_left_color.set();
        top_right_color.set();
        bottom_right_color.set();
        bottom_left_color.set();
    }

    void c_quad_gradient_shader::set_constants(const constants_t& constants) {
        top_left_color.value() = constants.colors[0].cast<float>();
        top_right_color.value() = constants.colors[1].cast<float>();
        bottom_left_color.value() = constants.colors[2].cast<float>();
        bottom_right_color.value() = constants.colors[3].cast<float>();
    }
}