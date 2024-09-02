#pragma once
#include "../../internal/shader.h"
#include "compiled-object.h"

namespace ntl::render::directx11 {
    class c_color_shader : public backend::i_color_shader, public c_shader {
    public:
        c_color_shader() : c_shader(&color_pixel_shader_object, &color_vertex_shader_object) { }

    public:
        void use() override;
    };
}