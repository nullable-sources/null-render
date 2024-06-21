#pragma once
#include "../../internal/shader/shader.h"
#include "compiled-object.h"

namespace null::render::opengl3 {
    class c_sdf_shader : public backend::i_sdf_shader, public c_default_shader {
    public:
        void* texture{ };
        c_uniform<float> aa{ }, outline_thickness{ };
        c_uniform<vec4_t<float>> outline_start{ }, outline_end{ };

    public:
        c_sdf_shader() : c_default_shader(&sdf_shader_object, &color_vertex_shader_object) { }

    public:
        void create() override;

        void use() override;

    public:
        void set_constants(const constants_t& constants) override;
    };
}