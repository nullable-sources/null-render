module;
#include <cmath>
export module null.render:font.glyph;

import null.sdk;

export namespace null::render {
    struct glyph_t {
    public:
        std::uint32_t codepoint{ 31 };
        bool visible{ true };
        float advance_x{ };
        rect_t<float> corners{ }, texture_coordinates{ };

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