#include <font/font.h>

namespace null::render {
    template <typename char_t>
    void c_font::calc_text_size(const std::basic_string_view<char_t>& str, vec2_t<float>& result, vec2_t<float>& line_size) {
        for(auto iterator{ str.begin() }; iterator != str.end();) {
            std::uint32_t symbol{ (std::uint32_t)*iterator };
            iterator += impl::char_converters::converter<char_t>::convert(symbol, iterator, str.end());
            if(!symbol) break;

            if(symbol == '\r') continue;
            if(symbol == '\n') {
                result.x = std::max(result.x, line_size.x);
                result.y += line_size.y;
                line_size.x = 0.f;
                continue;
            }

            line_size.x += get_char_advance(symbol) * scale;
        }
    }

    template <typename string_view_t>
    vec2_t<float> c_font::calc_text_size(const string_view_t& str, const float& custom_size) {
        vec2_t result{ }, line_size{ 0.f, custom_size <= 0.f ? size : custom_size };

        calc_text_size(std::basic_string_view{ str }, result, line_size);

        result.x = std::max(result.x, line_size.x);
        if(line_size.x > 0.f || result.y == 0.f) result.y += line_size.y;

        return result;
    }

    template <typename string_t>
    vec2_t<float> c_font::calc_text_size(const multicolor_text_t<string_t>& str, const float& custom_size) {
        vec2_t result{ }, line_size{ 0.f, custom_size <= 0.f ? size : custom_size };

        std::ranges::for_each(str.data, [&](const auto& data) { calc_text_size<string_t::value_type>(data.first, result, line_size); });

        result.x = std::max(result.x, line_size.x);
        if(line_size.x > 0.f || result.y == 0.f) result.y += line_size.y;

        return result;
    }
}