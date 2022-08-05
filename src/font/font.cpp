#include <draw-list/draw-list.h>

namespace null::render {
    void c_font::c_atlas::texture_t::get_data_as_rgba32() {
        if(pixels_alpha8.empty()) throw std::runtime_error("!pixels_alpha8");

        if(pixels_rgba32.empty()) {
            pixels_rgba32.resize(size.x * size.y * 4);
            const std::uint8_t* src = pixels_alpha8.data();
            std::uint32_t* dst = pixels_rgba32.data();
            for(int n : std::views::iota(1, size.x * size.y + 1) | std::views::reverse)
                *dst++ = (std::uint32_t)color_t{ 255, 255, 255, (std::uint32_t)*src++ };
        }
    }
    
    void c_font::c_atlas::setup_font(c_font* font, config_t* config, float ascent, float descent) {
        if(!config->merge_mode) {
            font->lookup_table.dirty = true;
            font->size = config->size_pixels;
            font->config = config; font->config_count = 0;
            font->container_atlas = this;
            font->ascent = ascent; font->descent = descent;
        }
        font->config_count++;
    }
    
    void c_font::c_atlas::build_initialize() {
        if(pack_id_cursors < 0) {
            custom_rects.push_back(custom_rect_t{ rect_t{ vec2_t{ std::numeric_limits<std::uint16_t>::max() } , vec2_t{ 2 } } });
            pack_id_cursors = custom_rects.size() - 1;
        }
        if(pack_id_lines < 0) {
            custom_rects.push_back(custom_rect_t{ rect_t{ vec2_t{ std::numeric_limits<std::uint16_t>::max() } , vec2_t{ 65, 64 } } });
            pack_id_lines = custom_rects.size() - 1;
        }
    }

    void c_font::c_atlas::build_finish() {
        if(texture.pixels_alpha8.empty()) throw std::runtime_error("!texture.pixels_alpha8");
        render_default_texture_data();
        render_lines_texture_data();

        for(custom_rect_t& rect : custom_rects) {
            if(!rect.font || rect.glyph_id == 0)
                continue;

            if(rect.font->container_atlas != this) throw std::runtime_error("rect.font->container_atlas != this");
            rect.font->add_glyph(nullptr, rect.glyph_id, rect_t{ rect.glyph_offset, rect.glyph_offset + rect.size.max }, rect.size * texture.uv_scale, rect.glyph_advance_x);
        }

        for(c_font& font : fonts) {
            if(font.lookup_table.dirty) font.build_lookup_table();
            if(font.ellipsis_char != (std::uint16_t)-1) continue;

            std::vector<std::uint16_t> ellipsis_variants{ (std::uint16_t)0x2026, (std::uint16_t)0x0085 };
            for(std::uint16_t& variant : ellipsis_variants) {
                if(font.find_glyph(variant, false)) {
                    font.ellipsis_char = variant;
                    break;
                }
            }
        }
    }

    bool c_font::c_atlas::build_with_stb_truetype() {
        if(locked) throw std::runtime_error("cannot modify a locked atlas between begin_render() and end_render/render()!");
        if(configs.empty()) throw std::runtime_error("configs.empty()");

        build_initialize();

        texture = texture_t{ };

        std::vector<build_src_t> src_array(configs.size());
        std::vector<build_dst_t> dst_array(fonts.size());

        std::ranges::transform(src_array, configs, src_array.begin(), [&](build_src_t src, config_t& config) {
            if(!config.font) throw std::runtime_error("!cfg.font");
            if(config.font->is_loaded() && config.font->container_atlas != this) throw std::runtime_error("cfg.font->is_loaded() && cfg.font->container_atlas != this");

            if(auto iterator = std::ranges::find_if(fonts, [=](const c_font& font) { return config.font == &font; }); iterator != fonts.end()) src.dst_index = std::distance(fonts.begin(), iterator);
            else throw std::runtime_error("can't find font config");

            const int font_offset = stbtt_GetFontOffsetForIndex((std::uint8_t*)config.data.data(), config.index);
            if(font_offset < 0) throw std::runtime_error("font_data is incorrect, or font_no cannot be found.");
            if(!stbtt_InitFont(&src.font_info, (std::uint8_t*)config.data.data(), font_offset))
                throw std::runtime_error("!stbtt_InitFont");

            build_dst_t& dst = dst_array[src.dst_index];
            src.src_ranges = config.glyph_config.ranges ? config.glyph_config.ranges : glyph_t::ranges_default();
            for(const std::uint16_t* src_range = src.src_ranges; src_range[0] && src_range[1]; src_range += 2)
                src.glyphs_highest = std::max(src.glyphs_highest, (int)src_range[1]);
            dst.src_count++;
            dst.glyphs_highest = std::max(dst.glyphs_highest, src.glyphs_highest);

            return src;
            });

        int total_glyphs_count{ };
        for(build_src_t& src : src_array) {
            build_dst_t& dst = dst_array[src.dst_index];
            src.glyphs_set.resize((src.glyphs_highest + 32) >> 5, 0);
            if(dst.glyphs_set.empty()) dst.glyphs_set.resize((dst.glyphs_highest + 31) >> 5, 0); 

            for(const std::uint16_t* src_range = src.src_ranges; src_range[0] && src_range[1]; src_range += 2) {
                for(std::uint32_t codepoint : std::views::iota(src_range[0], src_range[1] + (std::uint32_t)1)) {
                    if(dst.glyphs_set[codepoint >> 5] & (std::uint32_t)1 << (codepoint & 31)) continue;
                    if(!stbtt_FindGlyphIndex(&src.font_info, codepoint)) continue;

                    src.glyphs_count++;
                    dst.glyphs_count++;
                    src.glyphs_set[codepoint >> 5] |= (std::uint32_t)1 << (codepoint & 31);
                    dst.glyphs_set[codepoint >> 5] |= (std::uint32_t)1 << (codepoint & 31);
                    total_glyphs_count++;
                    codepoint = codepoint;
                }
            }
        }

        for(build_src_t& src : src_array) {
            for(int idx : std::views::iota((size_t)0, src.glyphs_set.size())) {
                if(!src.glyphs_set[idx]) continue;

                for(std::uint32_t bit_n : std::views::iota(0, 32)) {
                    if(src.glyphs_set[idx] & ((std::uint16_t)1 << bit_n))
                        src.glyphs_list.push_back(((idx) << 5) + bit_n);
                }
            }

            src.glyphs_set.clear();
            if(src.glyphs_list.size() != src.glyphs_count) throw std::runtime_error("src.glyphs_list.size() != src.glyphs_count");
        }

        std::vector<stbrp_rect> buf_rects(total_glyphs_count);
        std::vector<stbtt_packedchar> buf_packedchars(total_glyphs_count);

        int total_surface{ }, buf_rects_out_n{ }, buf_packedchars_out_n{ };
        std::ranges::transform(src_array, configs, src_array.begin(), [&](build_src_t src, const config_t& config) {
            if(!src.glyphs_count)
                return src;

            src.rects = &buf_rects[buf_rects_out_n];
            src.packed_chars = &buf_packedchars[buf_packedchars_out_n];
            buf_rects_out_n += src.glyphs_count;
            buf_packedchars_out_n += src.glyphs_count;

            src.pack_range = { config.size_pixels, 0, src.glyphs_list.data(), (int)src.glyphs_list.size(),
                src.packed_chars, (std::uint8_t)config.oversample.x, (std::uint8_t)config.oversample.y
            };

            const float scale = (config.size_pixels > 0) ? stbtt_ScaleForPixelHeight(&src.font_info, config.size_pixels) : stbtt_ScaleForMappingEmToPixels(&src.font_info, -config.size_pixels);
            for(int i : std::views::iota((size_t)0, src.glyphs_list.size())) {
                int x0{ }, y0{ }, x1{ }, y1{ };
                const int glyph_index_in_font{ stbtt_FindGlyphIndex(&src.font_info, src.glyphs_list[i]) };
                if(!glyph_index_in_font) throw std::runtime_error("!glyph_index_in_font");
                stbtt_GetGlyphBitmapBoxSubpixel(&src.font_info, glyph_index_in_font, scale * config.oversample.x, scale * config.oversample.y, 0, 0, &x0, &y0, &x1, &y1);
                src.rects[i].w = (stbrp_coord)(x1 - x0 + texture.glyph_padding + config.oversample.x - 1);
                src.rects[i].h = (stbrp_coord)(y1 - y0 + texture.glyph_padding + config.oversample.y - 1);
                total_surface += src.rects[i].w * src.rects[i].h;
            }

            return src;
            });

        const int surface_sqrt = std::sqrt(total_surface) + 1;
        texture.size.y = 0;
        if(texture.desired_width > 0) texture.size.x = texture.desired_width;
        else texture.size.x = (surface_sqrt >= 4096 * 0.7f) ? 4096 : (surface_sqrt >= 2048 * 0.7f) ? 2048 : (surface_sqrt >= 1024 * 0.7f) ? 1024 : 512;

        stbtt_pack_context spc{ };
        stbtt_PackBegin(&spc, nullptr, texture.size.x, /*TEX_HEIGHT_MAX*/ 1024 * 32, 0, texture.glyph_padding, nullptr);
        pack_custom_rects(spc.pack_info);

        for(build_src_t& src : src_array) {
            if(src.glyphs_count == 0)
                continue;

            stbrp_pack_rects((stbrp_context*)spc.pack_info, src.rects, src.glyphs_count);

            for(int i : std::views::iota(0, src.glyphs_count)) {
                if(src.rects[i].was_packed) texture.size.y = std::max((int)texture.size.y, src.rects[i].y + src.rects[i].h);
            }
        }

        texture.size.y = std::powf(2.f, std::ceilf(std::logf(texture.size.y) / std::logf(2.f)));
        texture.uv_scale = vec2_t{ 1.f } / texture.size;
        texture.pixels_alpha8.resize(texture.size.x* texture.size.y, 0);
        spc.pixels = texture.pixels_alpha8.data();
        spc.height = texture.size.y;

        std::ranges::transform(src_array, configs, src_array.begin(), [&](build_src_t src, const config_t& config) {
            if(src.glyphs_count == 0)
                return src;

            stbtt_PackFontRangesRenderIntoRects(&spc, &src.font_info, &src.pack_range, 1, src.rects);

            if(config.rasterizer_multiply != 1.0f) {
                std::array<std::uint8_t, 256> multiply_table{ };
                multiply_calc_lookup_table(multiply_table, config.rasterizer_multiply);

                for(stbrp_rect* rect = &src.rects[0]; int glyph_i : std::views::iota(0, src.glyphs_count)) {
                    if(rect->was_packed)
                        multiply_rect_alpha8(multiply_table, texture.pixels_alpha8, rect_t{ rect->x, rect->y, rect->w, rect->h }, texture.size.x);
                    rect++;
                }
            }
            src.rects = nullptr;

            return src;
            });

        stbtt_PackEnd(&spc);

        std::ranges::transform(src_array, configs, src_array.begin(), [&](build_src_t src, config_t& config) {
            if(src.glyphs_count == 0)
                return src;

            c_font* dst_font{ config.font };

            const float font_scale{ stbtt_ScaleForPixelHeight(&src.font_info, config.size_pixels) };
            int unscaled_ascent{ }, unscaled_descent{ }, unscaled_line_gap{ };
            stbtt_GetFontVMetrics(&src.font_info, &unscaled_ascent, &unscaled_descent, &unscaled_line_gap);

            const float ascent{ std::floorf(unscaled_ascent * font_scale + ((unscaled_ascent > 0.0f) ? +1 : -1)) };
            const float descent{ std::floorf(unscaled_descent * font_scale + ((unscaled_descent > 0.0f) ? +1 : -1)) };
            setup_font(dst_font, &config, ascent, descent);
            const vec2_t font_off{ config.glyph_config.offset + vec2_t{ 0, round(dst_font->ascent) } };

            for(int glyph_i : std::views::iota(0, src.glyphs_count)) {
                const int codepoint = src.glyphs_list[glyph_i];
                stbtt_aligned_quad q{ };
                float unused_x{ }, unused_y{ };
                stbtt_GetPackedQuad(src.packed_chars, texture.size.x, texture.size.y, glyph_i, &unused_x, &unused_y, &q, 0);
                dst_font->add_glyph(&config, (std::uint16_t)codepoint, rect_t{ q.x0, q.y0, q.x1, q.y1 } + font_off, rect_t{ q.s0, q.t0, q.s1, q.t1 }, src.packed_chars[glyph_i].xadvance);
            }

            return src;
            });

        build_finish();
        return true;
    }
    
    void c_font::c_atlas::render_lines_texture_data() {
        custom_rect_t* rect = &custom_rects[pack_id_lines];
        if(!rect->is_packed()) throw std::runtime_error("!rect->is_packed()");

        for(std::uint32_t n : std::views::iota(0, 64)) {
            std::uint32_t pad_left{ ((int)rect->size.max.x - n) / 2 }, pad_right{ (int)rect->size.max.x - (pad_left + n) };

            if(pad_left + n + pad_right != rect->size.max.x) throw std::runtime_error("pad_left + line_width + pad_right != rect->size.max.x");
            if(n >= rect->size.max.y) throw std::runtime_error("y >= rect->size.max.y");
            
            //@todo: get rid of memeset
            std::uint8_t* write_ptr{ &texture.pixels_alpha8[int(rect->size.min.x + ((rect->size.min.y + n) * texture.size.x))] };
            memset(write_ptr, 0x00, pad_left);
            memset(write_ptr + pad_left, 0xFF, n);
            memset(write_ptr + pad_left + n, 0x00, pad_right);

            rect_t uv{ rect_t{ rect->size.min + vec2_t{ pad_left - 1, n }, vec2_t{ pad_left + n + 1, n + 1 } } * texture.uv_scale };
            float half_v{ (uv.min.y + uv.max.y) * 0.5f };
            texture.uv_lines[n] = rect_t{ uv.min.x, half_v, uv.max.x, half_v };
        }
    }

    void c_font::c_atlas::render_default_texture_data() {
        custom_rect_t* rect = &custom_rects[pack_id_cursors];
        if(!rect->is_packed()) throw std::runtime_error("!rect->is_packed()");

        if(rect->size.max.x != 2) throw std::runtime_error("rect->size.max.x != 2");
        if(rect->size.max.y != 2) throw std::runtime_error("rect->size.max.y != 2");

        const int offset{ (int)(rect->size.min.x + rect->size.min.y * texture.size.x) };
        texture.pixels_alpha8[offset] = texture.pixels_alpha8[offset + 1] = texture.pixels_alpha8[offset + texture.size.x] = texture.pixels_alpha8[offset + texture.size.x + 1] = 0xFF;

        texture.uv_white_pixel = (rect->size.min + vec2_t{ 0.5f }) * texture.uv_scale;
    }

    void c_font::c_atlas::pack_custom_rects(void* stbrp_context_opaque) {
        stbrp_context* pack_context{ (stbrp_context*)stbrp_context_opaque };
        if(!pack_context) throw std::runtime_error("pack_context == nullptr");

        std::vector<custom_rect_t>& user_rects{ custom_rects };
        if(user_rects.empty()) throw std::runtime_error("user_rects empty");

        std::vector<stbrp_rect> pack_rects(user_rects.size());
        for(int i : std::views::iota((size_t)0, user_rects.size())) {
            pack_rects[i].w = user_rects[i].size.max.x;
            pack_rects[i].h = user_rects[i].size.max.y;
        }

        stbrp_pack_rects(pack_context, &pack_rects[0], pack_rects.size());
        for(int i : std::views::iota((size_t)0, pack_rects.size())) {
            if(pack_rects[i].was_packed) {
                user_rects[i].size.min.x = pack_rects[i].x;
                user_rects[i].size.min.y = pack_rects[i].y;
                
                if(pack_rects[i].w != user_rects[i].size.max.x) throw std::runtime_error("pack_rects[i].w != user_rects[i].size.max.x");
                if(pack_rects[i].h != user_rects[i].size.max.y) throw std::runtime_error("pack_rects[i].h != user_rects[i].size.max.y");

                texture.size.y = std::max((int)texture.size.y, pack_rects[i].y + pack_rects[i].h);
            }
        }
    }

    void c_font::c_atlas::multiply_calc_lookup_table(std::array<std::uint8_t, 256>& out_table, float in_multiply_factor) {
        for(std::uint32_t i : std::views::iota(0, 256)) {
            std::uint32_t value{ (std::uint32_t)(i * in_multiply_factor) };
            out_table[i] = value > 255 ? 255 : (value & 0xFF);
        }
    }

    void c_font::c_atlas::multiply_rect_alpha8(const std::array<std::uint8_t, 256>& table, std::vector<std::uint8_t>& pixels, const rect_t& size, int stride) {
        for(int data_i{ (int)(size.min.x + size.min.y * stride) }; int j : std::views::iota(1, size.max.x + 1) | std::views::reverse) {
            for(int i : std::views::iota(0, size.max.y))
                pixels[data_i + i] = table[pixels[data_i + i]];
            data_i += stride;
        }
    }

    c_font* c_font::c_atlas::add_font(config_t* config) {
        if(locked) throw std::runtime_error("cannot modify a locked atlas between begin_render() and end_render/render()!");
        if(config->font) throw std::runtime_error("config->font == nullptr");
        if(config->data.empty()) throw std::runtime_error("cofig->data.empty()");
        if(config->size_pixels <= 0.f) throw std::runtime_error("config->size_pixels <= 0.f");

        if(!config->merge_mode) fonts.push_back(c_font{ });
        else if(fonts.empty()) throw std::runtime_error("cannot use merge_mode for the first font");

        configs.push_back(*config);
        config_t& cfg{ configs.back() };
        
        if(!cfg.font) cfg.font = &fonts.back();
        if(!cfg.owned_by_atlas) {
            cfg.owned_by_atlas = true;
        }

        if(cfg.font->ellipsis_char == (std::uint16_t)-1)
            cfg.font->ellipsis_char = config->ellipsis_char;

        texture.clear();
        return cfg.font;
    }

    c_font* c_font::c_atlas::add_font_default(config_t* config) {
        config_t cfg{ config ? *config : config_t{ } };
        if(!config) {
            cfg.oversample = vec2_t{ 1 };
            cfg.pixel_snap_h = true;
        }
        if(cfg.size_pixels <= 0.0f) cfg.size_pixels = 13.0f * 1.0f;
        cfg.ellipsis_char = (std::uint16_t)0x0085;
        cfg.glyph_config.offset.y = 1.0f * floor(cfg.size_pixels / 13.0f);

        return add_font_from_memory_compressed_base_85_ttf(compressed_fonts::proggy_clean, cfg.size_pixels, &cfg, cfg.glyph_config.ranges ? cfg.glyph_config.ranges : glyph_t::ranges_default());
    }

    c_font* c_font::c_atlas::add_font_from_file_ttf(const char* filename, float size_pixels, config_t* config, const std::uint16_t* glyph_ranges) {
        if(locked) throw std::runtime_error("cannot modify a locked atlas between begin_render() and end_render/render()!");
        
        std::ifstream file{ filename, std::ios::in | std::ios::binary | std::ios::ate };
        if(!file.is_open()) throw std::runtime_error("cannot open font file");
        
        std::vector<char> font_file((std::size_t)file.tellg());
        file.seekg(0, std::ios::beg);
        file.read(font_file.data(), font_file.size());
        file.close();

        return add_font_from_memory_ttf(font_file, size_pixels, config, glyph_ranges);
    }

    c_font* c_font::c_atlas::add_font_from_memory_ttf(const std::vector<char>& font_file, float size_pixels, config_t* config, const std::uint16_t* glyph_ranges) {
        if(locked) throw std::runtime_error("cannot modify a locked atlas between begin_render() and end_render/render()!");

        config_t cfg{ config ? *config : config_t{ } };
        if(!cfg.data.empty()) throw std::runtime_error("!cfg.data.empty()");
        
        cfg.data = font_file;
        cfg.size_pixels = size_pixels;
        if(glyph_ranges) cfg.glyph_config.ranges = glyph_ranges;
        
        return add_font(&cfg);
    }

    c_font* c_font::c_atlas::add_font_from_memory_compressed_ttf(const std::vector<char>& compressed_ttf, float size_pixels, config_t* config, const std::uint16_t* glyph_ranges) {
        std::vector<char> buf_decompressed_data(impl::stb::decompress_length((std::uint8_t*)compressed_ttf.data()));
        impl::stb::decompress((std::uint8_t*)buf_decompressed_data.data(), (const std::uint8_t*)compressed_ttf.data());

        config_t cfg{ config ? *config : config_t{ } };
        if(!cfg.data.empty()) throw std::runtime_error("!cfg.data.empty()");
        cfg.owned_by_atlas = true;

        return add_font_from_memory_ttf(buf_decompressed_data, size_pixels, &cfg, glyph_ranges);
    }

    c_font* c_font::c_atlas::add_font_from_memory_compressed_base_85_ttf(const char* compressed_font_data_base85, float size_pixels, config_t* config, const std::uint16_t* glyph_ranges) {
        std::vector<char> compressed_data((strlen(compressed_font_data_base85) + 4) / 5 * 4);
        impl::decode85((const std::uint8_t*)compressed_font_data_base85, (std::uint8_t*)compressed_data.data());
        return add_font_from_memory_compressed_ttf(compressed_data, size_pixels, config, glyph_ranges);
    }


    void c_font::c_atlas::clear_input_data() {
        if(locked) throw std::runtime_error("cannot modify a locked atlas between begin_render() and end_render/render()!");

        for(config_t& config : configs) {
            if(config.owned_by_atlas) {
                config.font = nullptr;
                continue;
            }

            for(c_font& font : fonts) {
                if(!font.config) continue;
                if(font.config == &config) {
                    font.config = nullptr;
                    font.config_count = 0;
                    break;
                }
            }
        }

        configs.clear();
        custom_rects.clear();
        pack_id_cursors = pack_id_lines = -1;
    }

    void c_font::c_atlas::clear_fonts() {
        fonts.clear();
    }

    void c_font::build_lookup_table() {
        int max_codepoint{ (int)(*std::ranges::max_element(glyphs, [](const glyph_t& a, const glyph_t& b) { return a.codepoint < b.codepoint; })).codepoint };
        
        if(glyphs.size() >= 0xFFFF) throw std::runtime_error("glyphs.size() >= 0xFFFF");
        lookup_table = lookup_table_t{ };
        lookup_table.resize(max_codepoint + 1);
        used_4k_pages_map.fill(0);

        for(int i : std::views::iota((size_t)0, glyphs.size())) {
            int codepoint = glyphs[i].codepoint;
            lookup_table.advances_x[codepoint] = glyphs[i].advance_x;
            lookup_table.indexes[codepoint] = (std::uint16_t)i;

            const int page_n{ codepoint / 4096 };
            used_4k_pages_map[page_n >> 3] |= 1 << (page_n & 7);
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

    c_font::glyph_t* c_font::find_glyph(std::uint16_t c, bool fallback) {
        if(c >= (size_t)lookup_table.indexes.size()) return fallback ? fallback_glyph : nullptr;
        const std::uint16_t i = lookup_table.indexes[c];
        if(i == (std::uint16_t)-1) return fallback ? fallback_glyph : nullptr;
        return &glyphs[i];
    }

    void c_font::add_glyph(config_t* cfg, std::uint16_t codepoint, rect_t corners, rect_t texture_coordinates, float advance_x) {
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

    void c_font::add_remap_char(std::uint16_t dst, std::uint16_t src, bool overwrite_dst) {
        if(lookup_table.indexes.empty()) throw std::runtime_error("lookup_table.indexes.empty()");

        if(dst < lookup_table.indexes.size() && lookup_table.indexes[dst] == (std::uint16_t)-1 && !overwrite_dst) return;
        if(src >= lookup_table.indexes.size() && dst >= lookup_table.indexes.size()) return;

        lookup_table.resize(dst + 1);
        lookup_table.indexes[dst] = (src < lookup_table.indexes.size()) ? lookup_table.indexes[src] : (std::uint16_t)-1;
        lookup_table.advances_x[dst] = (src < lookup_table.indexes.size()) ? lookup_table.advances_x[src] : 1.0f;
    }

    bool c_font::is_glyph_range_unused(std::uint32_t c_begin, std::uint32_t c_last) {
        for(std::uint32_t page_n : std::views::iota(c_begin / 4096, c_last / 4096 + 1)) {
            if((page_n >> 3) < 2 && used_4k_pages_map[page_n >> 3] & (1 << (page_n & 7)))
                return false;
        }
        return true;
    }

    void set_current_font(c_font* font) {
        if(!font || !font->is_loaded()) throw std::runtime_error("!font || !font->is_loaded()");
        if(font->scale <= 0.f) throw std::runtime_error("font->scale <= 0.f");
        current_font = font;
        shared_data.font = current_font;
    }

    void push_font(c_font* font) {
        if(!font) font = get_current_font();
        set_current_font(font);
        fonts.push_back(font);
    }

    void pop_font(c_font* font) {
        fonts.pop_back();
        set_current_font(fonts.empty() ? get_current_font() : fonts.back());
    }
}