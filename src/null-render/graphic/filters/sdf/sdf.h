#pragma once
#include "../../../backend/shaders/sdf.h"
#include "../texture/texture.h"

namespace null::render {
    template <typename shader_constants_t = backend::i_sdf_shader::constants_t>
    class i_sdf_filter : public c_texture_filter {
    public:
        shader_constants_t constants{ };

    public:
        virtual ~i_sdf_filter() { }

    public:
        void set_px_range(float px_range) { constants.px_range = px_range; }
        void set_weight(float threshold, float out_bias) {
            constants.threshold = threshold;
            constants.out_bias = out_bias;
        }

        void set_outline(float absolute_thickness, float relative_thickness, float blur, const color_t<int>& color) {
            constants.outline_width_absolute = absolute_thickness;
            constants.outline_width_relative = relative_thickness;
            constants.outline_blur = blur;
            constants.outline_color = color;
        }
    };

    class c_sdf_filter : public i_sdf_filter<backend::i_sdf_shader::constants_t> {
    public:
        static std::shared_ptr<c_sdf_filter> instance() { return std::make_shared<c_sdf_filter>(); }

    public:
        virtual void handle() override;

    public:
        virtual std::shared_ptr<i_filter> clone() const override {
            return std::shared_ptr<c_sdf_filter>(new c_sdf_filter(*this));
        }
    };

    class c_msdf_filter : public i_sdf_filter<backend::i_msdf_shader::constants_t> {
    public:
        static std::shared_ptr<c_msdf_filter> instance() { return std::make_shared<c_msdf_filter>(); }

    public:
        virtual void handle() override;

    public:
        virtual std::shared_ptr<i_filter> clone() const override {
            return std::shared_ptr<c_msdf_filter>(new c_msdf_filter(*this));
        }
    };

    class c_mtsdf_filter : public i_sdf_filter<backend::i_mtsdf_shader::constants_t> {
    public:
        static std::shared_ptr<c_mtsdf_filter> instance() { return std::make_shared<c_mtsdf_filter>(); }

    public:
        void set_rounds(float glyph_rounding, float outline_rounding) {
            constants.glyph_rounding = glyph_rounding;
            constants.outline_rounding = outline_rounding;
        }

    public:
        virtual void handle() override;

    public:
        virtual std::shared_ptr<i_filter> clone() const override {
            return std::shared_ptr<c_mtsdf_filter>(new c_mtsdf_filter(*this));
        }
    };
}
