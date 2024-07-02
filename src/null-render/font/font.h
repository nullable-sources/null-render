#pragma once
#include "loaders/loader.h"
#include "glyph.h"

namespace null::render {
    namespace impl {
        static int get_char_from_utf8(std::uint32_t* out_char, std::string_view::const_iterator iterator, std::string_view::const_iterator end) {
            static constexpr std::array<char, 32> lengths{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 3, 3, 4, 0 };
            static constexpr std::array<int, 5> masks{ 0x00, 0x7f, 0x1f, 0x0f, 0x07 };
            static constexpr std::array<std::uint32_t, 5> mins{ 0x400000, 0, 0x80, 0x800, 0x10000 };
            static constexpr std::array<int, 5> shiftc{ 0, 18, 12, 6, 0 };
            static constexpr std::array<int, 5> shifte{ 0, 6, 4, 2, 0 };
            int len = lengths[*(const unsigned char*)iterator._Unwrapped() >> 3];
            int wanted = len + !len;

            std::array<std::uint8_t, 4> s{ };
            for(int i = 0; std::next(iterator, i) != end && i < s.size(); i++) {
                s[i] = *std::next(iterator, i);
            }

            *out_char = (std::uint32_t)(s[0] & masks[len]) << 18;
            *out_char |= (std::uint32_t)(s[1] & 0x3f) << 12;
            *out_char |= (std::uint32_t)(s[2] & 0x3f) << 6;
            *out_char |= (std::uint32_t)(s[3] & 0x3f) << 0;
            *out_char >>= shiftc[len];

            int e = (*out_char < mins[len]) << 6;
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

        namespace char_converters {
            template <typename string_t> struct char_t { using type = string_t::value_type; };
            template <> struct char_t<const char*> { using type = char; };
            template <> struct char_t<const wchar_t*> { using type = char; };

            template <class string_t>
            struct converter {
                template <typename iterator_t>
                static int convert(std::uint32_t& output_char, const iterator_t& iterator, const iterator_t& end) {
                    return 1;
                }
            };

            template <>
            struct converter<char> {
                static int convert(std::uint32_t& output_char, const std::string_view::const_iterator& iterator, const std::string_view::const_iterator& end) {
                    return output_char < 0x80 ? 1 : get_char_from_utf8(&output_char, iterator, end);
                }
            };
        }
    }

    class c_atlas;
    class c_font {
    public:
        friend class c_atlas;

    public:
        struct metrics_t {
        public:
            friend class c_font;

        public:
            float size{ };
            float ascender{ }, descender{ };
            float line_height{ }; //@note: distance between two baselines

            float px_range{ };
        };

    protected:
        c_atlas* parent_atlas{ };
        std::vector<glyph_t> glyphs{ };

        std::uint32_t fallback_codepoint{ (std::uint32_t)'?' };

        std::unordered_map<std::uint64_t, float> kerning{ };
        std::vector<size_t> lookup_table{ }; //@note: the table for getting glyph by codepoint.

    public:
        metrics_t metrics{ };

    public:
        void build_lookups();

        size_t lookup_codepoint(std::uint32_t codepoint) const { return lookup_table.at(codepoint); }
        float& lookup_kerning(std::uint32_t prev_codepoint, std::uint32_t curr_codepoint) {
            std::uint64_t key = ((std::uint64_t)prev_codepoint << 32) | (std::uint64_t)curr_codepoint;
            return kerning[key];
        }

        bool is_loaded() const { return parent_atlas; }
        const c_atlas* get_parent_atlas() const { return parent_atlas; }

        glyph_t* find_glyph(std::uint32_t codepoint);
        void add_glyph(const glyph_t& glyph) { glyphs.push_back(glyph); }

        void set_fallback_codepoint(std::uint32_t codepoint) { fallback_codepoint = codepoint; build_lookups(); }
        void set_glyph_visible(std::uint32_t codepoint, bool visible) { if(glyph_t* glyph = find_glyph(codepoint)) glyph->visible = visible; }

        template <typename string_t> vec2_t<float> calc_text_size(const string_t& text, float custom_size = -1.f,float letter_spacing = 1.f, float line_spacing = 1.f) { return calc_text_size(std::basic_string_view(text), custom_size, letter_spacing, line_spacing); }
        template <typename char_t> vec2_t<float> calc_text_size(std::basic_string_view<char_t> text, float custom_size = -1.f, float letter_spacing = 1.f, float line_spacing = 1.f);
    };

    class c_atlas {
    public:
        struct texture_t {
        public:
            void* data{ };
            std::vector<std::uint32_t> pixels_rgba32{ };

            vec2_t<float> size{ };

        public:
            void resize(const vec2_t<float>& new_size) {
                size = new_size;
                pixels_rgba32.resize(size.x * size.y);
            }

            void write_rgba(size_t x, size_t y, const color_t<int>& rgba) { write_rgba(size.x * y + x, rgba); }
            void write_rgba(size_t offset, const color_t<int>& rgba) {
                read_rgba(offset) = (std::uint32_t)(rgba.r << 0 | rgba.g << 8 | rgba.b << 16 | rgba.a << 24);
            }

            std::uint32_t& read_rgba(size_t x, size_t y) { return pixels_rgba32[size.x * y + x]; }
            std::uint32_t& read_rgba(size_t offset) { return pixels_rgba32[offset]; }

        public:
            void clear() { pixels_rgba32.clear(); }
            bool is_built() const { return !pixels_rgba32.empty(); }
        };

    public:
        texture_t texture{ };

        bool locked{ };
        std::vector<std::unique_ptr<c_font>> fonts{ };
        std::vector<font_config_t> configs{ };

        std::unique_ptr<i_font_loader> font_loader{ };

    public:
        ~c_atlas() { clear(); }

    public:
        void build_finish();

        void build();

        c_font* add_font(const font_config_t& config);

        void clear_input_data();
        void clear() { clear_input_data(); texture.clear(); fonts.clear(); }
    } inline atlas{ };

    struct atlases_handler_t {
    public:
        bool changed{ true };
        std::vector<c_atlas*> atlases{ &atlas };

    public:
        void add(c_atlas* atlas) {
            atlases.push_back(atlas);
            changed = true;
        }
    } inline atlases_handler{ };

    c_font* get_default_font();
    void set_default_font(c_font* font);
}

#include "font.inl"