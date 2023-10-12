#include "font.h"

namespace null::render {
	template <typename char_t>
	vec2_t<float> c_font::calc_text_size(std::basic_string_view<char_t> text, float custom_size) {
		vec2_t<float> result{ }, line_size{ 0.f, custom_size <= 0.f ? size : custom_size };

		for(auto iterator{ text.begin() }; iterator != text.end();) {
			std::uint32_t symbol{ (std::uint32_t)*iterator };
			iterator += impl::char_converters::converter<char_t>::convert(symbol, iterator, text.end());
			if(!symbol) break;

			if(symbol == '\r') continue;
			if(symbol == '\n') {
				result.x = std::max(result.x, line_size.x);
				result.y += line_size.y;
				line_size.x = 0.f;
				continue;
			}

			line_size.x += get_char_advance(symbol) * (custom_size / size);
		}

		result.x = std::max(result.x, line_size.x);
		if(line_size.x > 0.f || result.y == 0.f) result.y += line_size.y;

		return result;
	}
}