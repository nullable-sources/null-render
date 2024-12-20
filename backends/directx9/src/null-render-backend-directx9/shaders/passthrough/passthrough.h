#pragma once
#include "../../internal/shader.h"
#include "compiled-object.h"

namespace ntl::render::directx9 {
    class c_passthrough_shader : public backend::i_passthrough_shader, public c_default_shader {
    public:
        c_passthrough_shader() : c_default_shader(&passthrough_pixel_shader_object, &passthrough_vertex_shader_object) { }
    };
}