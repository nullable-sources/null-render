#include "font.h"

namespace ntl::render {
    template <typename char_t>
    vec2_t<float> c_font::calc_text_size(std::basic_string_view<char_t> text, float custom_size, float letter_spacing, float line_spacing) {
        float scale = 1.f;
        if(custom_size != -1) scale = custom_size / metrics.size;

        const float scaled_letter_spacing = letter_spacing * scale;
        const float scaled_line_spacing = line_spacing * scale;

        vec2_t<float> result{ }, line_size(0.f, metrics.line_height * scale + scaled_line_spacing);

        std::uint32_t previous_symbol{ };
        for(auto iterator = text.begin(); iterator != text.end();) {
            std::uint32_t symbol = (std::uint32_t)*iterator;
            iterator += impl::char_converters::converter<char_t>::convert(symbol, iterator, text.end());
            if(!symbol) break;

            if(symbol == '\r') continue;
            if(symbol == '\n') {
                result.x = std::max(result.x, line_size.x);
                result.y += line_size.y;
                line_size.x = 0.f;
                continue;
            }

            const glyph_t* glyph = find_glyph(symbol);
            if(!glyph) continue;

            line_size.x += glyph->advance * scale + lookup_kerning(previous_symbol, symbol) * scale + scaled_letter_spacing;
            previous_symbol = symbol;
        }

        result.x = std::max(result.x, line_size.x);
        if(line_size.x > 0.f || result.y == 0.f) result.y += line_size.y;

        return result;
    }
}