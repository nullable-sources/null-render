#pragma once
#define STB_TRUETYPE_IMPLEMENTATION
#define STB_RECT_PACK_IMPLEMENTATION
#define STBRP_STATIC
#define STBTT_STATIC

#include <stb_rect_pack.h>
#include <stb_truetype.h>

#include <null-sdk.h>
#include <compressed-fonts/proggy-clean.h>

#define stb_impl_in2(x) ((i[x] << 8) + i[(x)+1])
#define stb_impl_in3(x) ((i[x] << 16) + stb_impl_in2((x)+1))
#define stb_impl_in4(x) ((i[x] << 24) + stb_impl_in3((x)+1))

namespace null::render {
    template <typename string_t>
    struct multicolor_text_t {
        using data_t = std::vector<std::pair<string_t, color_t>>;
        data_t data{ };

        //@note: returns a string made up of all the strings in the text
        string_t unite() {
            return std::accumulate(data.begin(), data.end(), string_t{ }, [=](string_t result, data_t::value_type str) {
                return result + str.first;
                });
        }
    };

    namespace impl {
        static void decode85(const std::uint8_t* src, std::uint8_t* dst) {
            static auto decode85_byte = [](char c) { return c >= '\\' ? c - 36 : c - 35; };
            while(*src) {
                std::uint32_t tmp = decode85_byte(src[0]) + 85 * (decode85_byte(src[1]) + 85 * (decode85_byte(src[2]) + 85 * (decode85_byte(src[3]) + 85 * decode85_byte(src[4]))));
                dst[0] = ((tmp >> 0) & 0xFF); dst[1] = ((tmp >> 8) & 0xFF); dst[2] = ((tmp >> 16) & 0xFF); dst[3] = ((tmp >> 24) & 0xFF);
                src += 5;
                dst += 4;
            }
        }

        namespace stb {
            inline std::uint8_t* barrier_out_e{ }, *barrier_out_b{ };
            inline const std::uint8_t* barrier_in_b{ };
            inline std::uint8_t* dout{ };

            static std::uint32_t decompress_length(const std::uint8_t* input) { return (input[8] << 24) + (input[9] << 16) + (input[10] << 8) + input[11]; }
            static void match(const std::uint8_t* data, std::uint32_t length) {
                if(dout + length > barrier_out_e) throw std::runtime_error{ "dout + length > barrier_out_e" };
                if(data < barrier_out_b) { dout = barrier_out_e + 1; return; }
                while(length--) *dout++ = *data++;
            }

            static void lit(const std::uint8_t* data, std::uint32_t length) {
                if(dout + length > barrier_out_e) throw std::runtime_error{ "dout + length > barrier_out_e" };
                if(data < barrier_in_b) { dout = barrier_out_e + 1; return; }
                memcpy(dout, data, length);
                dout += length;
            }

            static const std::uint8_t* decompress_token(const std::uint8_t* i) {
                if(*i >= 0x20) {
                    if(*i >= 0x80)       match(dout - i[1] - 1, i[0] - 0x80 + 1), i += 2;
                    else if(*i >= 0x40)  match(dout - (stb_impl_in2(0) - 0x4000 + 1), i[2] + 1), i += 3;
                    else /* *i >= 0x20 */ lit(i + 1, i[0] - 0x20 + 1), i += 1 + (i[0] - 0x20 + 1);
                } else {
                    if(*i >= 0x18)       match(dout - (stb_impl_in3(0) - 0x180000 + 1), i[3] + 1), i += 4;
                    else if(*i >= 0x10)  match(dout - (stb_impl_in3(0) - 0x100000 + 1), stb_impl_in2(3) + 1), i += 5;
                    else if(*i >= 0x08)  lit(i + 2, stb_impl_in2(0) - 0x0800 + 1), i += 2 + (stb_impl_in2(0) - 0x0800 + 1);
                    else if(*i == 0x07)  lit(i + 3, stb_impl_in2(1) + 1), i += 3 + (stb_impl_in2(1) + 1);
                    else if(*i == 0x06)  match(dout - (stb_impl_in3(1) + 1), i[4] + 1), i += 5;
                    else if(*i == 0x04)  match(dout - (stb_impl_in3(1) + 1), stb_impl_in2(4) + 1), i += 6;
                }
                return i;
            }

            static std::uint32_t adler32(std::uint32_t adler32, std::uint8_t* buffer, std::uint32_t buflen) {
                constexpr std::uint64_t ADLER_MOD = 65521;
                std::uint64_t s1{ adler32 & 0xffff }, s2{ adler32 >> 16 };
                std::uint64_t blocklen{ buflen % 5552 };

                std::uint64_t i{ };
                while(buflen) {
                    for(i = 0; i + 7 < blocklen; i += 8) {
                        s1 += buffer[0], s2 += s1;
                        s1 += buffer[1], s2 += s1;
                        s1 += buffer[2], s2 += s1;
                        s1 += buffer[3], s2 += s1;
                        s1 += buffer[4], s2 += s1;
                        s1 += buffer[5], s2 += s1;
                        s1 += buffer[6], s2 += s1;
                        s1 += buffer[7], s2 += s1;

                        buffer += 8;
                    }

                    for(; i < blocklen; ++i)
                        s1 += *buffer++, s2 += s1;

                    s1 %= ADLER_MOD, s2 %= ADLER_MOD;
                    buflen -= blocklen;
                    blocklen = 5552;
                }
                return (std::uint32_t)(s2 << 16) + (std::uint32_t)s1;
            }

            static std::uint32_t decompress(std::uint8_t* output, const std::uint8_t* i) {
                if(stb_impl_in4(0) != 0x57bC0000) return 0;
                if(stb_impl_in4(4) != 0) return 0;
                const std::uint32_t olen{ decompress_length(i) };
                barrier_in_b = i;
                barrier_out_e = output + olen;
                barrier_out_b = output;
                i += 16;

                dout = output;
                for(;;) {
                    const std::uint8_t* old_i{ i };
                    i = decompress_token(i);
                    if(i == old_i) {
                        if(*i == 0x05 && i[1] == 0xfa) {
                            assert(dout == output + olen);
                            if(dout != output + olen) throw std::runtime_error{ "dout != output + olen" };
                            if(adler32(1, output, olen) != (std::uint32_t)stb_impl_in4(2))
                                return 0;
                            return olen;
                        } else throw std::runtime_error{ "decompress error" };
                    }

                    if(dout > output + olen) throw std::runtime_error{ "dout > output + olen" };
                }
            }
        }

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

    class c_font {
    public:
        struct glyph_t {
            struct config_t {
                vec2_t offset{ }, extra_spacing{ };
                const std::uint16_t* ranges{ };
                float min_advance_x, max_advance_x{ std::numeric_limits<float>::max() };
            };

            std::uint32_t codepoint{ 31 };
            bool visible{ true };
            float advance_x{ };
            rect_t corners{ }, texture_coordinates{ };

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

    private:
        struct config_t {
            c_font* font{ };
            glyph_t::config_t glyph_config{ };

            std::vector<char> data{ };
            bool owned_by_atlas{ true };
            int index{ };
            float size_pixels{ };
            vec2_t oversample{ 3, 1 };
            bool pixel_snap_h{ };
            bool merge_mode{ };

            std::uint32_t rasterizer_flags{ };
            float rasterizer_multiply{ 1.f };

            std::uint16_t ellipsis_char{ (std::uint16_t)-1 };
        };

    public:
        class c_atlas {
        private:
            struct build_data_t {
                int glyphs_highest{ }, glyphs_count{ };
                std::vector<std::uint32_t> glyphs_set{ };
            };

            struct build_src_t : build_data_t {
                stbtt_fontinfo font_info{ };
                stbtt_pack_range pack_range{ };
                stbrp_rect* rects{ };
                stbtt_packedchar* packed_chars{ };

                const std::uint16_t* src_ranges{ };
                int dst_index{ };
                std::vector<int> glyphs_list{ };
            };

            struct build_dst_t : build_data_t {
                int src_count{ };
            };

            struct custom_rect_t {
                rect_t size{ vec2_t{ std::numeric_limits<std::uint16_t>::max() }, vec2_t{ 0.f } };
                std::uint32_t glyph_id{ };
                float glyph_advance_x{ };
                vec2_t glyph_offset{ };
                c_font* font{ };

                bool is_packed() { return size.min.x != std::numeric_limits<std::uint16_t>::max(); }
            };

        public:
            struct texture_t {
                void* id{ };
                int desired_width{ 0 }, glyph_padding{ 1 };

                std::vector<std::uint8_t> pixels_alpha8{ };
                std::vector<std::uint32_t> pixels_rgba32{ };

                vec2_t size{ };
                vec2_t uv_scale{ }, uv_white_pixel{ };
                std::array<rect_t, 64> uv_lines{ };
                
                void clear() { pixels_alpha8.clear(); pixels_rgba32.clear(); }
                bool is_built() { return !pixels_alpha8.empty() || !pixels_rgba32.empty(); }
                void get_data_as_rgba32();
            } texture;

            bool locked{ };
            std::vector<c_font> fonts{ };
            std::vector<custom_rect_t> custom_rects{ };
            std::vector<config_t> configs{ };

            int pack_id_cursors{ -1 }, pack_id_lines{ -1 };

        public:
            void setup_font(c_font* font, config_t* config, float ascent, float descent);

            void build_initialize();
            void build_finish();

            bool build_with_stb_truetype();

            void render_lines_texture_data();
            void render_default_texture_data();

            void pack_custom_rects(void* stbrp_context_opaque);
            void multiply_calc_lookup_table(std::array<std::uint8_t, 256>& out_table, float in_multiply_factor);
            void multiply_rect_alpha8(const std::array<std::uint8_t, 256>& table, std::vector<std::uint8_t>& pixels, const rect_t& size, int stride);

            c_font* add_font(config_t* config);
            c_font* add_font_default(config_t* config = nullptr);
            c_font* add_font_from_file_ttf(const char* filename, float size_pixels, config_t* config = nullptr, const std::uint16_t* glyph_ranges = glyph_t::ranges_default());
            c_font* add_font_from_memory_ttf(const std::vector<char>& font_data, float size_pixels, config_t* config = nullptr, const std::uint16_t* glyph_ranges = glyph_t::ranges_default());
            c_font* add_font_from_memory_compressed_ttf(const std::vector<char>& compressed_ttf, float size_pixels, config_t* config = nullptr, const std::uint16_t* glyph_ranges = glyph_t::ranges_default());
            c_font* add_font_from_memory_compressed_base_85_ttf(const char* compressed_font_data_base85, float size_pixels, config_t* config = nullptr, const std::uint16_t* glyph_ranges = glyph_t::ranges_default());

            void clear_input_data();
            void clear_fonts();
            void clear() { clear_input_data(); texture.clear(); clear_fonts(); }
        };

        struct lookup_table_t {
            std::vector<float> advances_x{ };
            std::vector<std::uint16_t> indexes{ };
            bool dirty{ };

            void resize(int new_size) {
                if(advances_x.size() != indexes.size()) throw std::runtime_error{ "advances_x.size() != indexes.size()" };
                if(new_size <= indexes.size()) return;
                advances_x.resize(new_size, -1.0f);
                indexes.resize(new_size, (std::uint16_t)-1);
            }
        } lookup_table;

    private:
        std::uint16_t fallback_char{ (std::uint16_t)'?' }, ellipsis_char{ (std::uint16_t)-1 };

    public:
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

        glyph_t* find_glyph(std::uint16_t c, bool fallback = true);
        void add_glyph(config_t* src_config, std::uint16_t c, rect_t corners, rect_t texture_coordinates, float advance_x);
        void add_remap_char(std::uint16_t dst, std::uint16_t src, bool overwrite_dst = true);
        bool is_glyph_range_unused(std::uint32_t c_begin, std::uint32_t c_last);

        void set_fallback_char(std::uint16_t c) { fallback_char = c; build_lookup_table(); }
        void set_glyph_visible(std::uint16_t c, bool visible) { if(glyph_t* glyph = find_glyph(c)) glyph->visible = visible; }

        bool is_loaded() { return container_atlas; }
        float get_char_advance(std::uint16_t c) const { return (c < lookup_table.advances_x.size()) ? lookup_table.advances_x[c] : fallback_advance_x; }

        template <typename char_t>
        void calc_text_size(std::basic_string_view<char_t> str, vec2_t& result, vec2_t& line_size) {
            for(auto iterator = str.begin(); iterator != str.end();) {
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
        vec2_t calc_text_size(string_view_t str, float custom_size = 0.f) {
            vec2_t result{ }, line_size{ 0.f, custom_size <= 0.f ? size : custom_size };

            calc_text_size(std::basic_string_view{ str }, result, line_size);

            result.x = std::max(result.x, line_size.x);
            if(line_size.x > 0.f || result.y == 0.f) result.y += line_size.y;

            return result;
        }

        template <typename string_t>
        vec2_t calc_text_size(const multicolor_text_t<string_t>& str, float custom_size = 0.f) {
            vec2_t result{ }, line_size{ 0.f, custom_size <= 0.f ? size : custom_size };

            std::ranges::for_each(str.data, [&](const auto& data) { calc_text_size<string_t::value_type>(data.first, result, line_size); });

            result.x = std::max(result.x, line_size.x);
            if(line_size.x > 0.f || result.y == 0.f) result.y += line_size.y;

            return result;
        }
    };

    inline bool atlas_owned_by_initialize{ };
    inline c_font::c_atlas global_atlas{ };

    inline c_font* current_font{ }; //@note: set only from set_current_font
    inline std::vector<c_font*> fonts{ };
    inline float font_global_scale{ };

    static c_font* get_current_font() { return current_font ? current_font : &global_atlas.fonts.front(); }
    void set_current_font(c_font* font);
    void push_font(c_font* font);
    void pop_font(c_font* font);
}