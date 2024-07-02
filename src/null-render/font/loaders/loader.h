#pragma once
#include <limits>
#include <vector>
#include <string>

#include <null-sdk.h>

namespace null::render {
    enum class e_font_rasterizer_flags {
        no_hinting = 1 << 0,
        no_auto_hint = 1 << 1,
        force_auto_hint = 1 << 2,
        light_hinting = 1 << 3,
        mono_hinting = 1 << 4,
        bold = 1 << 5,
        oblique = 1 << 6
    }; enum_create_bit_operators(e_font_rasterizer_flags, true);
    enum_create_cast_operator(e_font_rasterizer_flags, -);

    enum class e_font_render_mode { monochrome, sdf, psdf, msdf, mtsdf };
    enum class e_font_coloring_mode { none, simple, ink_trap, by_distance };

    class c_font;
    struct font_config_t {
    public:
        int used_threads{ -1 }; //@note: 0 - disabled, std::numeric_limits<int>::max() - maximum number of available threads
        e_font_rasterizer_flags rasterizer_flags{ };
        e_font_render_mode render_mode{ };
        e_font_coloring_mode coloring_mode{ e_font_coloring_mode::ink_trap };
        float size{ };

        //@note: If the value is too low, glyphs may have artifacts (background)
        //       when drawing a small font, in this case try increasing the range value.
        //       For more understanding, see the explanation in i_sdf_brush::prepare_command
        float pixel_range{ };
        float miter_limit{ 1.f };
        rect_t<float> glyph_padding{ };
        bool x_origin_align{ }, y_origin_align{ };

        c_font* fallback_font{ };

        std::vector<std::uint8_t> font_data{ }, artery_data{ };
        std::vector<std::pair<std::uint32_t, std::uint32_t>> charsets{
            std::pair(0x0020, 0x007f) //@note: ascii
        };

    public:
        font_config_t& use_multithreading(this font_config_t& self, int threads_count = std::numeric_limits<int>::max()) { self.used_threads = threads_count; return self; }
        font_config_t& set_rasterizer_flags(this font_config_t& self, e_font_rasterizer_flags rasterizer_flags) { self.rasterizer_flags = rasterizer_flags; return self; }
        font_config_t& set_render_mode(this font_config_t& self, e_font_render_mode render_mode) { self.render_mode = render_mode; return self; }
        font_config_t& set_coloring_mode(this font_config_t& self, e_font_coloring_mode coloring_mode) { self.coloring_mode = coloring_mode; return self; }
        font_config_t& set_size(this font_config_t& self, float size) { self.size = size; return self; }
        font_config_t& set_pixel_range(this font_config_t& self, float pixel_range) { self.pixel_range = pixel_range; return self; }
        font_config_t& set_miter_limit(this font_config_t& self, float miter_limit) { self.miter_limit = miter_limit; return self; }
        font_config_t& set_font_data(this font_config_t& self, const std::vector<std::uint8_t>& font_data) { self.font_data = font_data; return self; }
        font_config_t& set_artery_data(this font_config_t& self, const std::vector<std::uint8_t>& artery_data) { self.artery_data = artery_data; return self; }
        font_config_t& set_fallback_font(this font_config_t& self, c_font* fallback_font) { self.fallback_font = fallback_font; return self; }
        font_config_t& set_glyph_padding(this font_config_t& self, const rect_t<float>& glyph_padding) { self.glyph_padding = glyph_padding; return self; }
        font_config_t& set_x_origin_align(this font_config_t& self, bool x_origin_align) { self.x_origin_align = x_origin_align; return self; }
        font_config_t& set_y_origin_align(this font_config_t& self, bool y_origin_align) { self.y_origin_align = y_origin_align; return self; }
        font_config_t& set_origin_align(this font_config_t& self, bool x_origin_align, bool y_origin_align) { self.x_origin_align = x_origin_align; self.y_origin_align = y_origin_align; return self; }

    public:
        font_config_t& add_charset_range(this font_config_t& self, std::uint32_t range_start, std::uint32_t range_end) { self.charsets.push_back(std::pair(range_start, range_end)); return self; }
        
        font_config_t& add_cyrillic_charset(this font_config_t& self) { return self.add_charset_range(0x0400, 0x04ff); }
        font_config_t& add_cyrillic_supplement_charset(this font_config_t& self) { return self.add_charset_range(0x0500, 0x052f); }
        font_config_t& add_cyrillic_extended_a(this font_config_t& self) { return self.add_charset_range(0x2de0, 0x2dff); }
        font_config_t& add_cyrillic_extended_b(this font_config_t& self) { return self.add_charset_range(0xa640, 0xa69f); }
        font_config_t& add_cyrillic_extended_c(this font_config_t& self) { return self.add_charset_range(0x1c80, 0x1c8f); }
        font_config_t& add_cyrillic_extended_d(this font_config_t& self) { return self.add_charset_range(0x1e030, 0x1e08f); }

    public:
        font_config_t& load_font_default(this font_config_t& self);
        font_config_t& load_font_from_resource(this font_config_t& self, std::string_view resource_name, std::string_view resource_type);
        font_config_t& load_font_from_file(this font_config_t& self, std::string_view filepath);

        font_config_t& load_artery_from_resource(this font_config_t& self, std::string_view resource_name, std::string_view resource_type);
        font_config_t& load_artery_from_file(this font_config_t& self, std::string_view filepath);
    };

    class c_atlas;
    class i_font_loader {
    public:
        virtual void build(c_atlas* atlas) = 0;
    };
}