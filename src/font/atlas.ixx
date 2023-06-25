module;
#include <cmath>
#include <limits>
#include <vector>
#include <string_view>
#include <memory>
#include <fstream>
#include <format>
#include <ranges>

#include <stb-decompress.h>
export module null.render:font.atlas;

import null.sdk;
import :font.lookup_table;
import :font;
import :font.configs;
import :font.glyph;
import :font.loader;

export namespace null::render {
    class c_atlas {
    public:
        struct custom_rect_t {
        public:
            rect_t<float> size{ vec2_t<float>{ std::numeric_limits<std::uint16_t>::max() }, vec2_t<float>{ 0.f } };
            std::uint32_t glyph_id{ };
            float glyph_advance_x{ };
            vec2_t<float> glyph_offset{ };
            c_font* font{ };

        public:
            bool is_packed() const { return size.min.x != std::numeric_limits<std::uint16_t>::max(); }
        };

        struct texture_t {
        public:
            void* data{ };
            int desired_width{ 0 }, glyph_padding{ 1 };

            std::vector<std::uint8_t> pixels_alpha8{ };
            std::vector<std::uint32_t> pixels_rgba32{ };

            vec2_t<float> size{ };

        public:
            void clear() { pixels_alpha8.clear(); pixels_rgba32.clear(); }
            bool is_built() const { return !pixels_alpha8.empty() || !pixels_rgba32.empty(); }
            void get_data_as_rgba32() {
                if(pixels_alpha8.empty()) {
                    utils::logger.log(utils::e_log_type::error, "pixels_alpha8 is empty.");
                    return;
                }

                if(pixels_rgba32.empty()) {
                    pixels_rgba32.resize(size.x * size.y * 4);
                    const std::uint8_t* src{ pixels_alpha8.data() };
                    std::uint32_t* dst{ pixels_rgba32.data() };
                    for(const int& n : std::views::iota(1, size.x* size.y + 1) | std::views::reverse)
                        *dst++ = (*src++ << 24) | 0xFFFFFF;
                }
            }
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
        void setup_font(c_font* font, font_config_t* config, const float& ascent, const float& descent) {
            font->lookup_table.dirty = true;
            font->size = config->size_pixels;
            font->config = config;
            font->container_atlas = this;
            font->ascent = ascent; font->descent = descent;
        }

        void build() {
            if(!font_loader) {
                utils::logger.log(utils::e_log_type::error, "font_loader is empty.");
                return;
            }
            font_loader->build(this);
        }

        void build_finish() {
            if(texture.pixels_alpha8.empty()) {
                utils::logger.log(utils::e_log_type::error, "texture pixels_alpha8 is empty.");
                return;
            }

            for(std::unique_ptr<c_font>& font : fonts) {
                if(font->lookup_table.dirty) font->build_lookup_table();
            }
        }

        void clear() { clear_input_data(); texture.clear(); fonts.clear(); }
        void clear_input_data() {
            if(locked) {
	            utils::logger.log(utils::e_log_type::error, "cannot modify a locked atlas between begin_render() and end_render/render().");
            	return;
            }

            for(font_config_t& config : configs) {
                config.font = nullptr;
            }

            configs.clear();
        }

        c_font* add_font(font_config_t* config) {
            if(locked) { utils::logger.log(utils::e_log_type::error, "cannot modify a locked atlas between begin_render() and end_render/render()."); return nullptr; }
            if(config->font) { utils::logger.log(utils::e_log_type::error, "config font pointer is empty."); return nullptr; }
            if(config->data.empty()) { utils::logger.log(utils::e_log_type::error, "cofig font data is empty."); return nullptr; }
            if(config->size_pixels <= 0.f) { utils::logger.log(utils::e_log_type::error, "config size_pixels <= 0.f."); return nullptr; }

            fonts.push_back(std::make_unique<c_font>());

            configs.push_back(*config);
            font_config_t& cfg{ configs.back() };

            if(!cfg.font) cfg.font = fonts.back().get();
            if(cfg.data.empty()) cfg.data = config->data;

            texture.clear();
            return cfg.font;
        }

        c_font* add_font_default(font_config_t* config = nullptr) {
            font_config_t cfg{ config ? *config : font_config_t{ } };
            if(!config) {
                cfg.oversample = 1;
                cfg.pixel_snap_h = true;
            }
            if(cfg.size_pixels <= 0.0f) cfg.size_pixels = 13.0f;
            cfg.glyph_config.offset.y = 1.0f * std::floor(cfg.size_pixels / 13.0f);

            return add_font_from_file_ttf(std::format("{}\\fonts\\Tahoma.ttf", std::getenv("windir")), cfg.size_pixels, &cfg, cfg.glyph_config.ranges ? cfg.glyph_config.ranges : glyph_t::ranges_default());
        }

        c_font* add_font_from_file_ttf(const std::string_view& filename, const float& size_pixels, font_config_t* config = nullptr, const std::uint16_t* glyph_ranges = glyph_t::ranges_default()) {
            if(locked) { utils::logger.log(utils::e_log_type::error, "cannot modify a locked atlas between begin_render() and end_render/render()."); return nullptr; }

            std::ifstream file{ filename.data(), std::ios::in | std::ios::binary | std::ios::ate };
            if(!file.is_open()) { utils::logger.log(utils::e_log_type::error, "cannot open font file."); return nullptr; }

            std::vector<char> font_file((std::size_t)file.tellg());
            file.seekg(0, std::ios::beg);
            file.read(font_file.data(), font_file.size());
            file.close();

            return add_font_from_memory_ttf(font_file, size_pixels, config, glyph_ranges);
        }

        c_font* add_font_from_memory_ttf(const std::vector<char>& font_file, float size_pixels, font_config_t* config = nullptr, const std::uint16_t* glyph_ranges = glyph_t::ranges_default()) {
            if(locked) { utils::logger.log(utils::e_log_type::error, "cannot modify a locked atlas between begin_render() and end_render/render()."); return nullptr; }

            font_config_t cfg{ config ? *config : font_config_t{ } };
            if(!cfg.data.empty()) utils::logger.log(utils::e_log_type::warning, "config font data is not empty.");

            cfg.data = font_file;
            cfg.size_pixels = size_pixels;
            if(glyph_ranges) cfg.glyph_config.ranges = glyph_ranges;

            return add_font(&cfg);
        }

        c_font* add_font_from_memory_compressed_ttf(const std::vector<std::uint8_t>& compressed_ttf, const float& size_pixels, font_config_t* config = nullptr, const std::uint16_t* glyph_ranges = glyph_t::ranges_default()) {
            std::vector<char> buf_decompressed_data(stb::decompress_length((std::uint8_t*)compressed_ttf.data()));
            stb::decompress((std::uint8_t*)buf_decompressed_data.data(), compressed_ttf.data());

            font_config_t cfg{ config ? *config : font_config_t{ } };
            if(!cfg.data.empty()) utils::logger.log(utils::e_log_type::warning, "config font data is not empty.");

            return add_font_from_memory_ttf(buf_decompressed_data, size_pixels, &cfg, glyph_ranges);
        }
    } atlas{ };

    struct atlases_handler_t {
    public:
        bool changed{ true };
        std::vector<c_atlas*> atlases{ &atlas };

    public:
        void add(c_atlas* atlas) {
            atlases.push_back(atlas);
            changed = true;
        }
    } atlases_handler{ };

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