#include <future>
#include <stb-decompress.h>

#include <font/loaders/freetype/freetype.h>
#include <font/font.h>

namespace null::render {
    void c_font::build_lookup_table() {
        int max_codepoint{ (int)(*std::ranges::max_element(glyphs, [](const glyph_t& a, const glyph_t& b) { return a.codepoint < b.codepoint; })).codepoint };

        if(glyphs.size() >= std::numeric_limits<std::uint16_t>::max()) {
            utils::logger.log(utils::e_log_type::error, "glyphs.size() >= 0xFFFF.");
            return;
        }
        lookup_table = lookup_table_t{ };
        lookup_table.resize(max_codepoint + 1);

        for(const int& i : std::views::iota((size_t)0, glyphs.size())) {
            int codepoint = glyphs[i].codepoint;
            lookup_table.advances_x[codepoint] = glyphs[i].advance_x;
            lookup_table.indexes[codepoint] = (std::uint16_t)i;
        }

        if(find_glyph((std::uint16_t)' ')) {
            if(glyphs.back().codepoint != '\t')
                glyphs.resize(glyphs.size() + 1);
            glyph_t& tab_glyph = glyphs.back();
            tab_glyph = *find_glyph((std::uint16_t)' ');
            tab_glyph.codepoint = '\t';
            tab_glyph.advance_x *= 4;
            lookup_table.advances_x[(int)tab_glyph.codepoint] = tab_glyph.advance_x;
            lookup_table.indexes[(int)tab_glyph.codepoint] = (std::uint16_t)(glyphs.size() - 1);
        }

        set_glyph_visible((std::uint16_t)' ', false);
        set_glyph_visible((std::uint16_t)'\t', false);

        fallback_glyph = find_glyph(fallback_char, false);
        fallback_advance_x = fallback_glyph ? fallback_glyph->advance_x : 0.0f;
        for(int i : std::views::iota(0, max_codepoint + 1))
            if(lookup_table.advances_x[i] < 0.0f) lookup_table.advances_x[i] = fallback_advance_x;
    }

    c_font::glyph_t* c_font::find_glyph(const std::uint16_t& c, const bool& fallback) {
        if(c >= (size_t)lookup_table.indexes.size()) return fallback ? fallback_glyph : nullptr;
        const std::uint16_t i{ lookup_table.indexes[c] };
        if(i == (std::uint16_t)-1) return fallback ? fallback_glyph : nullptr;
        return &glyphs[i];
    }

    void c_font::add_glyph(config_t* cfg, const std::uint16_t& codepoint, rect_t<float> corners, const rect_t<float>& texture_coordinates, float advance_x) {
        if(cfg) {
            const float advance_x_original{ advance_x };
            advance_x = std::clamp(advance_x, cfg->glyph_config.min_advance_x, cfg->glyph_config.max_advance_x);
            if(advance_x != advance_x_original) {
                corners += vec2_t{ cfg->pixel_snap_h ? floor((advance_x - advance_x_original) * 0.5f) : (advance_x - advance_x_original) * 0.5f, 0.f };
            }

            if(cfg->pixel_snap_h) advance_x = round(advance_x);
            advance_x += cfg->glyph_config.extra_spacing.x;
        }

        glyphs.push_back({ codepoint, corners.min != corners.max, advance_x, corners, texture_coordinates });
        lookup_table.dirty = true;
    }

    void c_atlas::texture_t::get_data_as_rgba32() {
        if(pixels_alpha8.empty()) {
            utils::logger.log(utils::e_log_type::error, "pixels_alpha8 is empty.");
            return;
        }

        if(pixels_rgba32.empty()) {
            pixels_rgba32.resize(size.x * size.y * 4);
            const std::uint8_t* src{ pixels_alpha8.data() };
            std::uint32_t* dst{ pixels_rgba32.data() };
            for(const int& n : std::views::iota(1, size.x * size.y + 1) | std::views::reverse)
                *dst++ = (*src++ << 24) | 0xFFFFFF;
        }
    }
    
    void c_atlas::setup_font(c_font* font, c_font::config_t* config, const float& ascent, const float& descent) {
        font->lookup_table.dirty = true;
        font->size = config->size_pixels;
        font->config = config;
        font->container_atlas = this;
        font->ascent = ascent; font->descent = descent;
    }

    void c_atlas::build_finish() {
        if(texture.pixels_alpha8.empty()) {
            utils::logger.log(utils::e_log_type::error, "texture pixels_alpha8 is empty.");
            return;
        }

        for(std::unique_ptr<c_font>& font : fonts) {
            if(font->lookup_table.dirty) font->build_lookup_table();
        }
    }

    void c_atlas::build() {
        if(!font_loader) font_loader = std::make_unique<c_freetype_loader>();
        font_loader->build(this);
    }

    c_font* c_atlas::add_font(c_font::config_t* config) {
        if(locked) { utils::logger.log(utils::e_log_type::error, "cannot modify a locked atlas between begin_render() and end_render/render()."); return nullptr; }
        if(config->font) { utils::logger.log(utils::e_log_type::error, "config font pointer is empty."); return nullptr; }
        if(config->data.empty()) { utils::logger.log(utils::e_log_type::error, "cofig font data is empty."); return nullptr; }
        if(config->size_pixels <= 0.f) { utils::logger.log(utils::e_log_type::error, "config size_pixels <= 0.f."); return nullptr; }

        fonts.push_back(std::make_unique<c_font>());

        configs.push_back(*config);
        c_font::config_t& cfg{ configs.back() };
        
        if(!cfg.font) cfg.font = fonts.back().get();
        if(cfg.data.empty()) cfg.data = config->data;

        texture.clear();
        return cfg.font;
    }

    c_font* c_atlas::add_font_default(c_font::config_t* config) {
        c_font::config_t cfg{ config ? *config : c_font::config_t{ } };
        if(!config) {
            cfg.oversample = 1;
            cfg.pixel_snap_h = true;
        }
        if(cfg.size_pixels <= 0.0f) cfg.size_pixels = 13.0f;
        cfg.glyph_config.offset.y = 1.0f * std::floor(cfg.size_pixels / 13.0f);

        return add_font_from_file_ttf(std::format("{}\\fonts\\Tahoma.ttf", std::getenv("windir")), cfg.size_pixels, &cfg, cfg.glyph_config.ranges ? cfg.glyph_config.ranges : c_font::glyph_t::ranges_default());
    }

    c_font* c_atlas::add_font_from_file_ttf(const std::string_view& filename, const float& size_pixels, c_font::config_t* config, const std::uint16_t* glyph_ranges) {
        if(locked) { utils::logger.log(utils::e_log_type::error, "cannot modify a locked atlas between begin_render() and end_render/render()."); return nullptr; }
        
        std::ifstream file{ filename.data(), std::ios::in | std::ios::binary | std::ios::ate };
        if(!file.is_open()) { utils::logger.log(utils::e_log_type::error, "cannot open font file."); return nullptr; }
        
        std::vector<char> font_file((std::size_t)file.tellg());
        file.seekg(0, std::ios::beg);
        file.read(font_file.data(), font_file.size());
        file.close();

        return add_font_from_memory_ttf(font_file, size_pixels, config, glyph_ranges);
    }

    c_font* c_atlas::add_font_from_memory_ttf(const std::vector<char>& font_file, float size_pixels, c_font::config_t* config, const std::uint16_t* glyph_ranges) {
        if(locked) { utils::logger.log(utils::e_log_type::error, "cannot modify a locked atlas between begin_render() and end_render/render()."); return nullptr; }

        c_font::config_t cfg{ config ? *config : c_font::config_t{ } };
        if(!cfg.data.empty()) utils::logger.log(utils::e_log_type::warning, "config font data is not empty.");
        
        cfg.data = font_file;
        cfg.size_pixels = size_pixels;
        if(glyph_ranges) cfg.glyph_config.ranges = glyph_ranges;
        
        return add_font(&cfg);
    }

    c_font* c_atlas::add_font_from_memory_compressed_ttf(const std::vector<std::uint8_t>& compressed_ttf, const float& size_pixels, c_font::config_t* config, const std::uint16_t* glyph_ranges) {
        std::vector<char> buf_decompressed_data(stb::decompress_length((std::uint8_t*)compressed_ttf.data()));
        stb::decompress((std::uint8_t*)buf_decompressed_data.data(), compressed_ttf.data());

        c_font::config_t cfg{ config ? *config : c_font::config_t{ } };
        if(!cfg.data.empty()) utils::logger.log(utils::e_log_type::warning, "config font data is not empty.");

        return add_font_from_memory_ttf(buf_decompressed_data, size_pixels, &cfg, glyph_ranges);
    }

    void c_atlas::clear_input_data() {
        if(locked) { utils::logger.log(utils::e_log_type::error, "cannot modify a locked atlas between begin_render() and end_render/render()."); return; }

        for(c_font::config_t& config : configs) {
            config.font = nullptr;
        }

        configs.clear();
    }

    c_font* get_default_font() {
        return c_font::default_font ? c_font::default_font : atlas.fonts.front().get();
    }

    void set_default_font(c_font* font) {
        if(!font || !font->is_loaded()) {
            utils::logger.log(utils::e_log_type::warning, "The default font cannot be set, it is empty or not loaded.");
            return;
        }

        c_font::default_font = font;
    }
}