#pragma once
#include "../../internal/shader/shader.h"
#include "compiled-object.h"

namespace null::render::opengl3 {
    template <typename implement_sdf_type_t>
    class i_sdf_shader : public implement_sdf_type_t, public c_default_shader {
    public:
        void* texture{ };
        c_uniform<float> px_range{ };
        c_uniform<float> outline_blur{ }, outline_width_relative{ }, outline_width_absolute{ };
        c_uniform<float> threshold{ }, out_bias{ };
        c_uniform<vec4_t<float>> outline_color{ };

    public:
        i_sdf_shader(c_sdf_shader_object& _sdf_shader_object) : c_default_shader(&_sdf_shader_object, &color_vertex_shader_object) { }

    public:
        virtual void create() override {
            if(!empty()) return;
            c_default_shader::create();

            px_range.get_location(program.get(), "px_range");
            threshold.get_location(program.get(), "threshold");
            out_bias.get_location(program.get(), "out_bias");
            outline_width_absolute.get_location(program.get(), "outline_width_absolute");
            outline_width_relative.get_location(program.get(), "outline_width_relative");
            outline_blur.get_location(program.get(), "outline_blur");
            outline_color.get_location(program.get(), "outline_color");
        }

        virtual void use() override {
            if(empty()) return;
            c_default_shader::use();

            px_range.set();
            threshold.set();
            out_bias.set();
            outline_width_absolute.set();
            outline_width_relative.set();
            outline_blur.set();
            outline_color.set();
        }

    public:
        virtual void set_constants(const implement_sdf_type_t::constants_t& constants) override {
            px_range.value() = constants.px_range;
            threshold.value() = constants.threshold;
            out_bias.value() = constants.out_bias;
            outline_width_absolute.value() = constants.outline_width_absolute;
            outline_width_relative.value() = constants.outline_width_relative;
            outline_blur.value() = constants.outline_blur;
            outline_color.value() = constants.outline_color.cast<float>();
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
        c_uniform<float> glyph_rounding{ }, outline_rounding{ };

    public:
        c_mtsdf_shader() : i_sdf_shader(mtsdf_shader_object) { }

    public:
        void create() override {
            if(!empty()) return;
            i_sdf_shader::create();
            glyph_rounding.get_location(program.get(), "glyph_rounding");
            outline_rounding.get_location(program.get(), "outline_rounding");
        }

        virtual void use() override {
            if(empty()) return;
            i_sdf_shader::use();
            glyph_rounding.set();
            outline_rounding.set();
        }

    public:
        virtual void set_constants(const constants_t& constants) override {
            i_sdf_shader::set_constants(constants);
            glyph_rounding.value() = constants.glyph_rounding;
            outline_rounding.value() = constants.outline_rounding;
        }
    };
}