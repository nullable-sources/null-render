#pragma once
#include "../../internal/shader.h"
#include "compiled-object.h"

namespace ntl::render::directx9 {
    template <typename implement_sdf_type_t>
    class i_sdf_shader : public implement_sdf_type_t, public c_default_shader {
    public:
        i_sdf_shader(c_pixel_shader& _sdf_shader_object) : c_default_shader(&_sdf_shader_object, &color_vertex_shader_object) { }

    public:
        virtual void set_constants(const implement_sdf_type_t::constants_t& constants) override {
            pixel_shader->set_constant(0, &constants.px_range);
            pixel_shader->set_constant(1, &constants.threshold);
            pixel_shader->set_constant(2, &constants.out_bias);
            pixel_shader->set_constant(3, &constants.outline_width_absolute);
            pixel_shader->set_constant(4, &constants.outline_width_relative);
            pixel_shader->set_constant(5, &constants.outline_blur);
            pixel_shader->set_constant(6, constants.outline_color.cast<float>().channels.data());
        }
    };

    class c_sdf_shader : public i_sdf_shader<backend::i_sdf_shader> {
    public:
        c_sdf_shader() : i_sdf_shader(sdf_shader_object) { }
    };

    class c_msdf_shader : public i_sdf_shader<backend::i_msdf_shader> {
    public:
        c_msdf_shader() : i_sdf_shader(msdf_shader_object) { }
    };

    class c_mtsdf_shader : public i_sdf_shader<backend::i_mtsdf_shader> {
    public:
        c_mtsdf_shader() : i_sdf_shader(mtsdf_shader_object) { }

    public:
        void set_constants(const backend::i_mtsdf_shader::constants_t& constants) override {
            i_sdf_shader::set_constants(constants);
            pixel_shader->set_constant(7, &constants.glyph_rounding);
            pixel_shader->set_constant(8, &constants.outline_rounding);
        }
    };
}