#pragma once
#include "../../internal/shader.h"
#include "compiled-object.h"

namespace ntl::render::directx11 {
    template <typename sdf_shader_t>
    class i_sdf_shader : public sdf_shader_t, public c_shader {
    private:
        i_sdf_shader_object<sdf_shader_t>* shader_object{ };

    public:
        i_sdf_shader(i_sdf_shader_object<sdf_shader_t>* _sdf_shader_object) : c_shader(_sdf_shader_object, &color_vertex_shader_object), shader_object(_sdf_shader_object) { }

    public:
        void use() override {
            if(empty()) return;
            c_shader::use();

            color_vertex_shader_object.set_constant({ backend::renderer->get_matrix(), backend::renderer->get_translation() });
        }

    public:
        void set_constants(const sdf_shader_t::constants_t& constants) override {
            shader_object->set_constant(constants);
        }
    };

    class c_sdf_shader : public i_sdf_shader<backend::i_sdf_shader> {
    public:
        c_sdf_shader() : i_sdf_shader(&sdf_shader_object) { };
    };

    class c_msdf_shader : public i_sdf_shader<backend::i_msdf_shader> {
    public:
        c_msdf_shader() : i_sdf_shader(&msdf_shader_object) { };
    };

    class c_mtsdf_shader : public i_sdf_shader<backend::i_mtsdf_shader> {
    public:
        c_mtsdf_shader() : i_sdf_shader(&mtsdf_shader_object) { };
    };
}