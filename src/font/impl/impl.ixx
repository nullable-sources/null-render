module;
#define STB_RECT_PACK_IMPLEMENTATION
#define STBRP_STATIC
#include <stb_rect_pack.h>

#include <string_view>
#include <array>
#include <cmath>
export module null.render:font.impl;

export namespace null::render::font::impl {
    int get_char_from_utf8(std::uint32_t* out_char, const std::string_view::const_iterator& iterator, const std::string_view::const_iterator& end) {
        static constexpr std::array<char, 32> lengths{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 3, 3, 4, 0 };
        static constexpr std::array<int, 5> masks{ 0x00, 0x7f, 0x1f, 0x0f, 0x07 };
        static constexpr std::array<std::uint32_t, 5> mins{ 0x400000, 0, 0x80, 0x800, 0x10000 };
        static constexpr std::array<int, 5> shiftc{ 0, 18, 12, 6, 0 };
        static constexpr std::array<int, 5> shifte{ 0, 6, 4, 2, 0 };
        const int len{ lengths[*(const unsigned char*)iterator._Unwrapped() >> 3] };
        int wanted{ len + !len };

        std::array<std::uint8_t, 4> s{ };
        for(int i{ 0 }; std::next(iterator, i) != end && i < s.size(); i++) {
            s[i] = *std::next(iterator, i);
        }

        *out_char = (std::uint32_t)(s[0] & masks[len]) << 18;
        *out_char |= (std::uint32_t)(s[1] & 0x3f) << 12;
        *out_char |= (std::uint32_t)(s[2] & 0x3f) << 6;
        *out_char |= (std::uint32_t)(s[3] & 0x3f) << 0;
        *out_char >>= shiftc[len];

        int e{ (*out_char < mins[len]) << 6 };
        e |= ((*out_char >> 11) == 0x1b) << 7;
        e |= (*out_char > 0xFFFF) << 8;
        e |= (s[1] & 0xc0) >> 2;
        e |= (s[2] & 0xc0) >> 4;
        e |= (s[3]) >> 6;
        e ^= 0x2a;
        e >>= shifte[len];

        if(e) {
            wanted = std::min(wanted, !!s[0] + !!s[1] + !!s[2] + !!s[3]);
            *out_char = 0xFFFD;
        }

        return wanted;
    }

    template <typename string_t> struct char_t { using type = string_t::value_type; };
    template <> struct char_t<const char*> { using type = char; };
    template <> struct char_t<const wchar_t*> { using type = char; };

    template <class string_t>
    struct char_converter {
        template <typename iterator_t>
        static int convert(std::uint32_t& output_char, const iterator_t& iterator, const iterator_t& end) {
            return 1;
        }
    };

    template <>
    struct char_converter<char> {
        static int convert(std::uint32_t& output_char, const std::string_view::const_iterator& iterator, const std::string_view::const_iterator& end) {
            return output_char < 0x80 ? 1 : get_char_from_utf8(&output_char, iterator, end);
        }
    };
}