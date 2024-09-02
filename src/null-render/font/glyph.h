#pragma once
#include <null-sdk.h>

namespace ntl::render {
    struct glyph_t {
    public:
        bool visible{ true };
        std::uint32_t codepoint{ 31 };
        float advance{ };
        rect_t<float> rectangle{ }, non_aligned_rectangle{ };
        vec2_t<float> size{ };
        vec2_t<float> bearing{ };

        //@note: the same rect_t<float> texture_uvs, but with pre-calculated upper right and lower left corners
        rect_t<float> upper_texture_uvs{ }; //@note: min - top left corner, max - top right corner
        rect_t<float> lower_texture_uvs{ }; //@note: min - bottom left corner, max - bottom right corner

    public:
        static const std::uint16_t* ranges_default() {
            static const std::uint16_t ranges[] = {
                0x0020, 0x00FF,
                0,
            }; return &ranges[0];
        }

        static const std::uint16_t* ranges_cyrillic() {
            static const std::uint16_t ranges[] = {
                0x0020, 0x00FF,
                0x0400, 0x052F,
                0x2DE0, 0x2DFF,
                0xA640, 0xA69F,
                0,
            };
            return &ranges[0];
        }
    };
}