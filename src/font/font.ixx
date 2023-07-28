module;
#include <vector>
#include <string_view>
#include <cmath>
#include <algorithm>
#include <ranges>
export module null.render:font;

import null.sdk;

import :font.configs;
import :font.impl;
import :font.lookup_table;
import :font.glyph;

export namespace null::render {
    class c_atlas;
    class c_font {
    public:
        static inline c_font* default_font{ }; //@note: set only from set_current_font

    public:
        std::uint16_t fallback_char{ (std::uint16_t)'?' };

        font::impl::lookup_table_t lookup_table{ };
        std::vector<glyph_t> glyphs{ };
        glyph_t* fallback_glyph{ };
        float fallback_advance_x{ };

        c_atlas* container_atlas{ };
        font_config_t* config{ };

        float size{ };
        float ascent{ }, descent{ };

    public:
        bool is_loaded() const { return container_atlas; }
        void set_fallback_char(const std::uint16_t& c) { fallback_char = c; build_lookup_table(); }
        void set_glyph_visible(const std::uint16_t& c, const bool& visible) { if(glyph_t * glyph{ find_glyph(c) }) glyph->visible = visible; }
        float get_char_advance(const std::uint16_t& c) const { return (c < lookup_table.advances_x.size()) ? lookup_table.advances_x[c] : fallback_advance_x; }

    public:
    	void build_lookup_table() {
            const size_t max_codepoint{ std::ranges::max_element(glyphs, [](const glyph_t& a, const glyph_t& b) { return a.codepoint < b.codepoint; })->codepoint };

            if(glyphs.size() >= std::numeric_limits<std::uint16_t>::max()) {
                utils::logger(utils::e_log_type::error, "glyphs.size() >= 0xFFFF.");
                return;
            }

            lookup_table = font::impl::lookup_table_t{ };
            lookup_table.resize(max_codepoint + 1u);

            for(const auto [i, glyph] : glyphs | std::views::enumerate) {
                const size_t codepoint{ glyph.codepoint };
                lookup_table.advances_x[codepoint] = glyph.advance_x;
                lookup_table.indexes[codepoint] = (std::uint16_t)i;
            }

            if(glyph_t* glyph{ find_glyph((std::uint16_t)' ') }) {
                if(glyphs.back().codepoint != '\t')
                    glyphs.resize(glyphs.size() + 1);
                glyph_t& tab_glyph{ glyphs.back() };
                tab_glyph = *glyph;
                tab_glyph.codepoint = '\t';
                tab_glyph.advance_x *= 4;
                lookup_table.advances_x[tab_glyph.codepoint] = tab_glyph.advance_x;
                lookup_table.indexes[tab_glyph.codepoint] = (std::uint16_t)(glyphs.size() - 1);
            }

            set_glyph_visible((std::uint16_t)' ', false);
            set_glyph_visible((std::uint16_t)'\t', false);

            fallback_glyph = find_glyph(fallback_char, false);
            fallback_advance_x = fallback_glyph ? fallback_glyph->advance_x : 0.0f;

            for(int i : std::views::iota(0u, max_codepoint + 1))
                if(lookup_table.advances_x[i] < 0.0f) lookup_table.advances_x[i] = fallback_advance_x;
    	}

        glyph_t* find_glyph(const std::uint16_t& c, const bool& fallback = true) {
            if(c >= (size_t)lookup_table.indexes.size())
                return fallback ? fallback_glyph : nullptr;

            const std::uint16_t i{ lookup_table.indexes[c] };
            if(i == (std::uint16_t)-1)
                return fallback ? fallback_glyph : nullptr;

            return &glyphs[i];
        }

        void add_glyph(font_config_t* src_config, const std::uint16_t& codepoint, rect_t<float> corners, const rect_t<float>& texture_coordinates, float advance_x) {
            if(src_config) {
                const float advance_x_original{ advance_x };
                advance_x = std::clamp(advance_x, src_config->glyph_config.min_advance_x, src_config->glyph_config.max_advance_x);
                if(advance_x != advance_x_original) {
                    corners += rect_t<float>{ vec2_t<float>{ src_config->pixel_snap_h ? std::floor((advance_x - advance_x_original) * 0.5f) : (advance_x - advance_x_original) * 0.5f, 0.f } };
                }

                if(src_config->pixel_snap_h) advance_x = round(advance_x);
                advance_x += src_config->glyph_config.extra_spacing.x;
            }

            glyphs.push_back({ codepoint, corners.min != corners.max, advance_x, corners, texture_coordinates });
            lookup_table.dirty = true;
    	}

        template <typename string_t> vec2_t<float> calc_text_size(const string_t& text, const float& custom_size = 0.f) { return calc_text_size(std::basic_string_view{ text }, custom_size); }
        template <typename char_t> vec2_t<float> calc_text_size(const std::basic_string_view<char_t>& text, const float& custom_size = 0.f) {
            vec2_t<float> result{ }, line_size{ 0.f, custom_size <= 0.f ? size : custom_size };

            for(auto iterator{ text.begin() }; iterator != text.end();) {
                std::uint32_t symbol{ (std::uint32_t)*iterator };
                iterator += font::impl::char_converter<char_t>::convert(symbol, iterator, text.end());
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
    };
}