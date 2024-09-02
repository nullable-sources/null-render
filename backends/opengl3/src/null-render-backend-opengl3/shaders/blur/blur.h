#pragma once
#include "../../internal/shader/shader.h"
#include "compiled-object.h"

namespace ntl::render::opengl3 {
    class c_blur_shader : public backend::i_blur_shader, public c_shader {
    public:
        c_uniform<vec2_t<float>> texel_size{ };
        c_uniform<vec2_t<float>> direction{ };
        c_uniform<vec4_t<float>> uv_limits{ };
        c_uniform<std::array<float, 96>> offsets{ };
        c_uniform<std::array<float, 96>> weights{ };
        c_uniform<int> iterations{ };

    public:
        c_blur_shader() : c_shader(&blur_fragment_shader_object, &passthrough_vertex_shader_object) { }

    public:
        void create() override;

        void use() override;

    public:
        void set_constants(const constants_t& constants) override;
        void set_direction(const vec2_t<float>& _direction) override;
    };
}