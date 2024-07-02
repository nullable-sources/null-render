#pragma once
#include "../../filters/sdf/sdf.h"
#include "../text/text.h"

namespace null::render {
    template <typename sdf_filter_t>
    class i_sdf_brush : public i_text_brush {
    private:
        float outline_absolute{ 0.f }, outline_relative{ 0.f }, outline_blur{ 0.f };
        color_t<int> outline_color{ };
        float out_bias{ 0.25f }, threshold{ 0.5f };

    public:
        i_sdf_brush() { }
        virtual ~i_sdf_brush() { }

    public:
        //@note: value between 0.25 ... 0.75
        void set_threshold(float _threshold) { threshold = std::clamp(_threshold, 0.25f, 0.75f); }

        //@note: value between -0.5 ... 0.5
        void set_out_bias(float _out_bias) { out_bias = std::clamp(_out_bias, -0.5f, 0.5f); }

        //@note: value between 0 ... 1
        void set_outline_blur(float blur) { outline_blur = std::clamp(blur, 0.f, 1.f); }

        //@note: relative outline - thickness depends on the font size
        //       absolute outline - thickness measured in pixels will be the same for text of any size
        void set_outline_width(float thickness, bool relative = false) { 
            if(relative) outline_relative = std::min(thickness, 0.5f);
            else outline_absolute = thickness;
        }
        void set_outline_color(const color_t<int> color) { outline_color = color; }

    protected:
        virtual std::shared_ptr<sdf_filter_t> instance_sdf_filter() const = 0;

    public:
        virtual std::shared_ptr<i_command> prepare_command(std::shared_ptr<c_geometry_command>&& command) const {
            std::shared_ptr<sdf_filter_t> filter = instance_sdf_filter();
            filter->set_texture(font ? font->get_parent_atlas()->texture.data : get_default_font()->get_parent_atlas()->texture.data);
            filter->set_outline(outline_absolute, outline_relative, outline_blur, outline_color);
            filter->set_weight(threshold, out_bias);

            c_font::metrics_t* metrics{ };
            if(!font) metrics = &get_default_font()->metrics;
            else metrics = &font->metrics;

            float size_scale = 1.f;
            if(size != -1.f) size_scale = size / metrics->size;
            //@note: https://github.com/Chlumsky/msdfgen#:~:text=screenPxRange()%20must%20never%20be%20lower%20than%201.%20If%20it%20is%20lower%20than%202
            //       px_range after scaling must be more than one,
            //       otherwise the glyphs will have artifacts in the form of a background.
            filter->set_px_range(metrics->px_range * size_scale);

            filter->set_child_command(command);
            return filter;
        };
    };

    class c_sdf_brush : public i_sdf_brush<c_sdf_filter> {
    public:
        static std::shared_ptr<c_sdf_brush> instance() { return std::make_shared<c_sdf_brush>(); }

    protected:
        virtual std::shared_ptr<c_sdf_filter> instance_sdf_filter() const override;
    };

    class c_msdf_brush : public i_sdf_brush<c_msdf_filter> {
    public:
        static std::shared_ptr<c_msdf_brush> instance() { return std::make_shared<c_msdf_brush>(); }

    protected:
        virtual std::shared_ptr<c_msdf_filter> instance_sdf_filter() const override;
    };

    class c_mtsdf_brush : public i_sdf_brush<c_mtsdf_filter> {
    public:
        static std::shared_ptr<c_mtsdf_brush> instance() { return std::make_shared<c_mtsdf_brush>(); }

    public:
        float glyph_rounding{ }, outline_rounding{ };

    public:
        void set_glyph_rounding(float _glyph_rounding) { glyph_rounding = _glyph_rounding; }
        void set_outline_rounding(float _outline_rounding) { outline_rounding = _outline_rounding; }
        void set_rounds(float _glyph_rounding, float _outline_rounding) {
            set_glyph_rounding(_glyph_rounding);
            set_outline_rounding(_outline_rounding);
        }

    protected:
        virtual std::shared_ptr<c_mtsdf_filter> instance_sdf_filter() const override;
    };
}