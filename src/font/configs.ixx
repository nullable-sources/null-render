module;
#include <vector>
#include <utils/fast_operators.h>
export module null.render:font.configs;

import null.sdk;

export namespace null::render {
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

    enum class e_font_render_mode {
        monochrome,
        sdf
    };

    struct glyph_config_t {
    public:
        vec2_t<float> offset{ }, extra_spacing{ };
        const std::uint16_t* ranges{ };
        float min_advance_x, max_advance_x{ std::numeric_limits<float>::max() };
    };

    class c_font;
    struct font_config_t {
    public:
        c_font* font{ };
        glyph_config_t glyph_config{ };

        std::vector<char> data{ };
        int index{ };
        float size_pixels{ };
        vec2_t<float> oversample{ 3, 1 };
        bool pixel_snap_h{ };

        e_font_rasterizer_flags rasterizer_flags{ };
        e_font_render_mode render_mode{ };
    };
}