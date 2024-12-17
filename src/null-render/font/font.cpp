#include <algorithm>

#include "loaders/freetype/freetype.h"
#include "font.h"

namespace ntl::render {
    static inline c_font* default_font{ };

    glyph_t* c_font::find_glyph(std::uint32_t codepoint) {
        if(codepoint >= lookup_table.size()) return nullptr;
        const size_t i = lookup_codepoint(codepoint);
        return &glyphs[i];
    }

    void c_font::build_lookups() {
        if(glyphs.size() >= std::numeric_limits<std::uint16_t>::max()) {
            sdk::logger(sdk::e_log_type::error, "glyphs.size() >= 0xFFFF.");
            return;
        }
        lookup_table.clear();

        if(auto finded = std::ranges::find(glyphs, fallback_codepoint, &glyph_t::codepoint); finded != glyphs.end()) {
            std::uint32_t max_codepoint = std::ranges::max(glyphs, { }, &glyph_t::codepoint).codepoint;
            lookup_table.resize(max_codepoint + 1);
            std::ranges::fill(lookup_table, std::distance(glyphs.begin(), finded));
        }

        //@note: fill all created glyphs to lookup_table
        for(size_t i = 0; i < glyphs.size(); i++)
            lookup_table[glyphs[i].codepoint] = i;

        if(const glyph_t* whitespace = find_glyph(' ')) {
            if(glyphs.back().codepoint != '\t')
                glyphs.push_back({ });

            glyph_t& tab_glyph = glyphs.back();
            tab_glyph = *whitespace;
            tab_glyph.codepoint = '\t';
            tab_glyph.advance *= 4;
            lookup_table[tab_glyph.codepoint] = glyphs.size() - 1;
        }
    }

    void c_atlas::build_finish() {
        if(texture.pixels_rgba32.empty()) {
            sdk::logger(sdk::e_log_type::error, "atlas texture pixels_rgba32 is empty.");
            return;
        }
    }

    void c_atlas::build() {
        if(!font_loader) font_loader = std::make_unique<c_freetype_loader>();
        font_loader->build(this);
    }

    c_font* c_atlas::add_font(const font_config_t& config) {
        if(locked) { sdk::logger(sdk::e_log_type::error, "cannot modify a locked atlas between begin_render() and end_render/render()."); return nullptr; }
        c_font* new_font = fonts.emplace_back(std::make_unique<c_font>()).get();
        new_font->parent_atlas = this;

        font_config_t& new_config = configs.emplace_back(config);
        new_config.set_fallback_font(new_font);

        return new_font;
    }

    void c_atlas::clear_input_data() {
        if(locked) { sdk::logger(sdk::e_log_type::error, "cannot modify a locked atlas between begin_render() and end_render/render()."); return; }

        configs.clear();
    }

    c_font* get_default_font() {
        return default_font ? default_font : atlas.fonts.front().get();
    }

    void set_default_font(c_font* font) {
        if(!font || !font->is_loaded()) {
            sdk::logger(sdk::e_log_type::warning, "The default font cannot be set, it is empty or not loaded.");
            return;
        }

        default_font = font;
    }
}