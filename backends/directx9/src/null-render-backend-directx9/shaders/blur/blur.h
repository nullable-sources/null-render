#pragma once
#include "../../internal/shader.h"
#include "compiled-object.h"

namespace null::render::directx9 {
    class c_blur_shader : public backend::i_blur_shader, public c_shader {
    public:
        c_blur_shader() : c_shader(&blur_shader_object, &passthrough_vertex_shader_object) { }

    public:
        void set_constants(const constants_t& constants) override;
        void set_direction(const vec2_t<float>& direction) override;
    };
}