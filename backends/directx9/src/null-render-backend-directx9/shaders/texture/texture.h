#pragma once
#include "../../internal/shader.h"
#include "compiled-object.h"

namespace null::render::directx9 {
    class c_texture_shader : public backend::i_texture_shader, public c_default_shader {
    public:
        c_texture_shader() : c_default_shader(&texture_shader_object, &color_vertex_shader_object) { }
    };
}