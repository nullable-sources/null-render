#pragma once
#include "../color/compiled-object.h"

namespace null::render::opengl3 {
    class c_linear_gradient_shader_object : public c_fragment_shader {
    public:
        memory::resource_t get_source() override {
            return memory::resource_t("null-render:opengl:shaders:linear-gradient.frag", "null-render:opengl:resources").load();
        }
    } inline linear_gradient_shader_object{ };
}