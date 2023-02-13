#pragma once
#define STB_RECT_PACK_IMPLEMENTATION
#define STBRP_STATIC
#include <stb_rect_pack.h>

#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftmodapi.h>
#include <freetype/ftglyph.h>
#include <freetype/ftsynth.h>

#include <null-sdk.h>

namespace null::render {
    template <typename string_t>
    struct multicolor_text_t {
    public:
        using data_t = std::vector<std::pair<string_t, color_t<int>>>;
        data_t data{ };

    public:
        multicolor_text_t() { }
        multicolor_text_t(const string_t& str, const color_t<int>& color) : data{ { str, color } } { }
        multicolor_text_t(const data_t& _data) : data{ _data } { }

    public:
        void transform(const std::function<void(string_t&, color_t<int>&)> func) {
            for(auto& [string, color] : data) { func(string, color); }
        }

        //@note: returns a string made up of all the strings in the text
        string_t unite() {
            return std::accumulate(data.begin(), data.end(), string_t{ }, [=](string_t result, data_t::value_type str) {
                return result + str.first;
                });
        }
    };

    namespace impl {
        static int get_char_from_utf8(std::uint32_t* out_char, const std::string_view::const_iterator& iterator, const std::string_view::const_iterator& end) {
            static constexpr std::array<char, 32> lengths{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 3, 3, 4, 0 };
            static constexpr std::array<int, 5> masks{ 0x00, 0x7f, 0x1f, 0x0f, 0x07 };
            static constexpr std::array<std::uint32_t, 5> mins{ 0x400000, 0, 0x80, 0x800, 0x10000 };
            static constexpr std::array<int, 5> shiftc{ 0, 18, 12, 6, 0 };
            static constexpr std::array<int, 5> shifte{ 0, 6, 4, 2, 0 };
            int len{ lengths[*(const unsigned char*)iterator._Unwrapped() >> 3] };
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

        namespace char_converters {
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

    enum class e_rasterizer_flags {
        no_hinting = 1 << 0,
        no_auto_hint = 1 << 1,
        force_auto_hint = 1 << 2,
        light_hinting = 1 << 3,
        mono_hinting = 1 << 4,
        bold = 1 << 5,
        oblique = 1 << 6,
        monochrome = 1 << 7
    }; enum_create_bit_operators(e_rasterizer_flags, true);
    enum_create_cast_operator(e_rasterizer_flags, -);

    class c_atlas;
    class c_font {
    public:
        struct glyph_t {
        public:
            struct config_t {
                vec2_t<float> offset{ }, extra_spacing{ };
                const std::uint16_t* ranges{ };
                float min_advance_x, max_advance_x{ std::numeric_limits<float>::max() };
            };

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

    public:
        struct config_t {
        public:
            c_font* font{ };
            glyph_t::config_t glyph_config{ };

            std::vector<char> data{ };
            bool owned_by_atlas{ true };
            int index{ };
            float size_pixels{ };
            vec2_t<float> oversample{ 3, 1 };
            bool pixel_snap_h{ };
            bool merge_mode{ };

            e_rasterizer_flags rasterizer_flags{ };
            float rasterizer_multiply{ 1.f };

            std::uint16_t ellipsis_char{ (std::uint16_t)-1 };
        };

        struct lookup_table_t {
        public:
            std::vector<float> advances_x{ };
            std::vector<std::uint16_t> indexes{ };
            bool dirty{ };

        public:
            void resize(const int& new_size) {
                if(advances_x.size() != indexes.size()) throw std::runtime_error{ "advances_x.size() != indexes.size()" };
                if(new_size <= indexes.size()) return;
                advances_x.resize(new_size, -1.0f);
                indexes.resize(new_size, (std::uint16_t)-1);
            }
        };

    public:
        static inline c_font* current_font{ }; //@note: set only from set_current_font
        static inline std::vector<c_font*> fonts{ };
        static inline float font_global_scale{ };

    public:
        static c_font* get_current_font();
        static void set_current_font(c_font* font);
        static void push_font(c_font* font);
        static void pop_font(c_font* font);

    public:
        std::uint16_t fallback_char{ (std::uint16_t)'?' }, ellipsis_char{ (std::uint16_t)-1 };

        lookup_table_t lookup_table{ };
        std::vector<glyph_t> glyphs{ };
        glyph_t* fallback_glyph{ };
        float fallback_advance_x{ };

        c_atlas* container_atlas{ };
        config_t* config{ };
        short config_count{ };

        float size{ }, scale{ 1.f };
        float ascent{ }, descent{ };

        std::array<std::uint8_t, 2> used_4k_pages_map{ };

    public:
        void build_lookup_table();

        glyph_t* find_glyph(const std::uint16_t& c, const bool& fallback = true);
        void add_glyph(config_t* src_config, const std::uint16_t& c, rect_t<float> corners, const rect_t<float>& texture_coordinates, float advance_x);
        void add_remap_char(const std::uint16_t& dst, const std::uint16_t& src, const bool& overwrite_dst = true);
        bool is_glyph_range_unused(const std::uint32_t& c_begin, const std::uint32_t& c_last);

        void set_fallback_char(const std::uint16_t& c) { fallback_char = c; build_lookup_table(); }
        void set_glyph_visible(const std::uint16_t& c, const bool& visible) { if(glyph_t* glyph{ find_glyph(c) }) glyph->visible = visible; }

        bool is_loaded() const { return container_atlas; }
        float get_char_advance(const std::uint16_t& c) const { return (c < lookup_table.advances_x.size()) ? lookup_table.advances_x[c] : fallback_advance_x; }

        template <typename char_t> void calc_text_size(const std::basic_string_view<char_t>& str, vec2_t<float>& result, vec2_t<float>& line_size);
        template <typename string_view_t> vec2_t<float> calc_text_size(const string_view_t& str, const float& custom_size = 0.f);
        template <typename string_t> vec2_t<float> calc_text_size(const multicolor_text_t<string_t>& str, const float& custom_size = 0.f);
    };

    class c_atlas {
    private:
        struct build_data_t {
        public:
            int glyphs_highest{ }, glyphs_count{ };
            std::vector<std::uint32_t> glyphs_set{ };
        };

        struct src_glyph_t {
        public:
            c_font::glyph_t glyph{ };
            std::vector<std::uint8_t> bitmap{ };
        };

        struct build_src_t : build_data_t {
        public:
            struct freetype_t {
            public:
                struct info_t {
                public:
                    std::uint32_t pixel_height{ };
                    float ascender{ }, descender{ }, line_spacing{ }, line_gap{ }, max_advance_width{ };
                };

            public:
                info_t info{ };
                FT_Face face{ };
                e_rasterizer_flags rasterizer_flags{ };
                FT_Int32 flags{ };
                FT_Render_Mode render_mode{ };
            };

        public:
            freetype_t freetype{ };
            stbrp_rect* rects{ };

            const std::uint16_t* src_ranges{ };
            int dst_index{ };
            std::vector<src_glyph_t> glyphs_list{ };
        };

        struct custom_rect_t {
        public:
            rect_t<float> size{ vec2_t{ std::numeric_limits<std::uint16_t>::max() }, vec2_t{ 0.f } };
            std::uint32_t glyph_id{ };
            float glyph_advance_x{ };
            vec2_t<float> glyph_offset{ };
            c_font* font{ };

        public:
            bool is_packed() const { return size.min.x != std::numeric_limits<std::uint16_t>::max(); }
        };

    public:
        struct texture_t {
        public:
            void* data{ };
            int desired_width{ 0 }, glyph_padding{ 1 };

            std::vector<std::uint8_t> pixels_alpha8{ };
            std::vector<std::uint32_t> pixels_rgba32{ };

            vec2_t<float> size{ };
            vec2_t<float> uv_scale{ }, uv_white_pixel{ };
            std::array<rect_t<float>, 64> uv_lines{ };

        public:
            void clear() { pixels_alpha8.clear(); pixels_rgba32.clear(); }
            bool is_built() const { return !pixels_alpha8.empty() || !pixels_rgba32.empty(); }
            void get_data_as_rgba32();
        };

    public:
        texture_t texture{ };

        bool locked{ };
        std::vector<c_font> fonts{ };
        std::vector<custom_rect_t> custom_rects{ };
        std::vector<c_font::config_t> configs{ };

        int pack_id_cursors{ -1 }, pack_id_lines{ -1 };

    public:
        ~c_atlas() { clear(); }

    public:
        void setup_font(c_font* font, c_font::config_t* config, const float& ascent, const float& descent);

        void build_initialize();
        void build_finish();

        bool build();

        void render_lines_texture_data();
        void render_default_texture_data();

        void pack_custom_rects(stbrp_context* stbrp_context_opaque);
        void multiply_calc_lookup_table(std::array<std::uint8_t, 256>& out_table, const float& in_multiply_factor);

        c_font* add_font(c_font::config_t* config);
        c_font* add_font_default(c_font::config_t* config = nullptr);
        c_font* add_font_from_file_ttf(const std::string_view& filename, const float& size_pixels, c_font::config_t* config = nullptr, const std::uint16_t* glyph_ranges = c_font::glyph_t::ranges_default());
        c_font* add_font_from_memory_ttf(const std::vector<char>& font_data, float size_pixels, c_font::config_t* config = nullptr, const std::uint16_t* glyph_ranges = c_font::glyph_t::ranges_default());
        c_font* add_font_from_memory_compressed_ttf(const std::vector<std::uint8_t>& compressed_ttf, const float& size_pixels, c_font::config_t* config = nullptr, const std::uint16_t* glyph_ranges = c_font::glyph_t::ranges_default());

        void clear_input_data();
        void clear() { clear_input_data(); texture.clear(); fonts.clear(); }
    } inline atlas{ };
}

#include <font/font.inl>