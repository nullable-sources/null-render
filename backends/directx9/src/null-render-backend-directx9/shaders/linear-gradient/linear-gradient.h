#pragma once
#include "../../internal/shader.h"
#include "compiled-object.h"

namespace ntl::render::directx9 {
    class c_linear_gradient_shader : public backend::i_linear_gradient_shader, public c_default_shader {
    public:
        c_linear_gradient_shader() : c_default_shader(&linear_gradient_shader_object, &color_vertex_shader_object) { }

    public:
        void set_constants(const constants_t& constants) override;
    };
}