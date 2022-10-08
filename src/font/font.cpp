#include <draw-list/draw-list.h>

namespace null::render {
    void c_font::build_lookup_table() {
        int max_codepoint{ (int)(*std::ranges::max_element(glyphs, [](const glyph_t& a, const glyph_t& b) { return a.codepoint < b.codepoint; })).codepoint };

        if(glyphs.size() >= std::numeric_limits<std::uint16_t>::max()) throw std::runtime_error{ "glyphs.size() >= 0xFFFF" };
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
        if(lookup_table.indexes.empty()) throw std::runtime_error{ "lookup_table.indexes.empty()" };

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

    c_font* c_font::get_current_font() {
        return current_font ? current_font : &atlas.fonts.front();
    }

    void c_font::set_current_font(c_font* font) {
        if(!font || !font->is_loaded()) throw std::runtime_error{ "!font || !font->is_loaded()" };
        if(font->scale <= 0.f) throw std::runtime_error{ "font->scale <= 0.f" };
        current_font = font;
    }

    void c_font::push_font(c_font* font) {
        if(!font) font = get_current_font();
        set_current_font(font);
        fonts.push_back(font);
    }

    void c_font::pop_font(c_font* font) {
        fonts.pop_back();
        set_current_font(fonts.empty() ? get_current_font() : fonts.back());
    }

    void c_atlas::texture_t::get_data_as_rgba32() {
        if(pixels_alpha8.empty()) throw std::runtime_error{ "!pixels_alpha8" };

        if(pixels_rgba32.empty()) {
            pixels_rgba32.resize(size.x * size.y * 4);
            const std::uint8_t* src = pixels_alpha8.data();
            std::uint32_t* dst = pixels_rgba32.data();
            for(int n : std::views::iota(1, size.x * size.y + 1) | std::views::reverse)
                *dst++ = (std::uint32_t)color_t{ 255, 255, 255, (std::uint32_t)*src++ };
        }
    }
    
    void c_atlas::setup_font(c_font* font, c_font::config_t* config, float ascent, float descent) {
        if(!config->merge_mode) {
            font->lookup_table.dirty = true;
            font->size = config->size_pixels;
            font->config = config; font->config_count = 0;
            font->container_atlas = this;
            font->ascent = ascent; font->descent = descent;
        }
        font->config_count++;
    }
    
    void c_atlas::build_initialize() {
        if(pack_id_cursors < 0) {
            custom_rects.push_back(custom_rect_t{ rect_t{ vec2_t{ std::numeric_limits<std::uint16_t>::max() } , vec2_t{ 2 } } });
            pack_id_cursors = custom_rects.size() - 1;
        }
        if(pack_id_lines < 0) {
            custom_rects.push_back(custom_rect_t{ rect_t{ vec2_t{ std::numeric_limits<std::uint16_t>::max() } , vec2_t{ 65, 64 } } });
            pack_id_lines = custom_rects.size() - 1;
        }
    }

    void c_atlas::build_finish() {
        if(texture.pixels_alpha8.empty()) throw std::runtime_error{ "!texture.pixels_alpha8" };
        render_default_texture_data();
        render_lines_texture_data();

        for(custom_rect_t& rect : custom_rects) {
            if(!rect.font || rect.glyph_id == 0)
                continue;

            if(rect.font->container_atlas != this) throw std::runtime_error{ "rect.font->container_atlas != this" };
            rect.font->add_glyph(nullptr, rect.glyph_id, rect_t{ rect.glyph_offset, rect.glyph_offset + rect.size.max }, rect.size * texture.uv_scale, rect.glyph_advance_x);
        }

        for(c_font& font : fonts) {
            if(font.lookup_table.dirty) font.build_lookup_table();
            if(font.ellipsis_char != (std::uint16_t)-1) continue;

            for(std::uint16_t& variant : std::vector<std::uint16_t>{ (std::uint16_t)0x2026, (std::uint16_t)0x0085 }) {
                if(font.find_glyph(variant, false)) {
                    font.ellipsis_char = variant;
                    break;
                }
            }
        }
    }

    bool c_atlas::build() {
        if(locked) throw std::runtime_error{ "cannot modify a locked atlas between begin_render() and end_render/render()!" };
        if(configs.empty()) throw std::runtime_error{ "configs.empty()" };
        
        FT_Library ft_library{ };
        if(FT_Init_FreeType(&ft_library))
            throw std::runtime_error{ "FT_Init_FreeType != 0" };

        build_initialize();
        texture = texture_t{ };

        std::vector<build_src_t> src_array{ configs.size() };
        std::vector<build_data_t> dst_array{ fonts.size() };

        std::ranges::transform(src_array, configs, src_array.begin(), [&](build_src_t src, c_font::config_t& config) {
            if(!config.font) throw std::runtime_error{ "!cfg.font" };
            if(config.font->is_loaded() && config.font->container_atlas != this) throw std::runtime_error{ "cfg.font->is_loaded() && cfg.font->container_atlas != this" };

            if(auto iterator{ std::ranges::find_if(fonts, [=](const c_font& font) { return config.font == &font; }) }; iterator != fonts.end()) src.dst_index = std::distance(fonts.begin(), iterator);
            else throw std::runtime_error{ "can't find font config" };

            if(FT_New_Memory_Face(ft_library, (std::uint8_t*)config.data.data(), (std::uint32_t)config.data.size(), (std::uint32_t)config.index, &src.freetype.face))
                throw std::runtime_error{ "FT_New_Memory_Face != 0" };
            if(FT_Select_Charmap(src.freetype.face, FT_ENCODING_UNICODE))
                throw std::runtime_error{ "FT_Select_Charmap != 0" };

            FT_Size_RequestRec req{
                .type = FT_SIZE_REQUEST_TYPE_REAL_DIM,
                .width = 0,
                .height = (FT_Long)config.size_pixels * 64,
                .horiResolution = 0, .vertResolution = 0
            };
            if(FT_Request_Size(src.freetype.face, &req))
                throw std::runtime_error{ "FT_Request_Size != 0" };

            FT_Size_Metrics metrics{ src.freetype.face->size->metrics };
            src.freetype.info = build_src_t::freetype_t::info_t{
                (std::uint32_t)config.size_pixels,
                std::ceilf(std::floor(metrics.ascender) / 64),
                std::ceilf(std::floor(metrics.descender) / 64),
                std::ceilf(std::floor(metrics.height) / 64),
                std::ceilf(std::floor(metrics.height - metrics.ascender + metrics.descender) / 64),
                std::ceilf(std::floor(metrics.max_advance) / 64)
            };

            src.freetype.rasterizer_flags = config.rasterizer_flags;
            src.freetype.flags = FT_LOAD_NO_BITMAP;
            if(config.rasterizer_flags & e_rasterizer_flags::no_hinting) src.freetype.flags |= FT_LOAD_NO_HINTING;
            if(config.rasterizer_flags & e_rasterizer_flags::no_auto_hint) src.freetype.flags |= FT_LOAD_NO_AUTOHINT;
            if(config.rasterizer_flags & e_rasterizer_flags::force_auto_hint) src.freetype.flags |= FT_LOAD_FORCE_AUTOHINT;
            if(config.rasterizer_flags & e_rasterizer_flags::light_hinting) src.freetype.flags |= FT_LOAD_TARGET_LIGHT;
            else if(config.rasterizer_flags & e_rasterizer_flags::mono_hinting) src.freetype.flags |= FT_LOAD_TARGET_MONO;
            else src.freetype.flags |= FT_LOAD_TARGET_NORMAL;

            if(config.rasterizer_flags & e_rasterizer_flags::monochrome) src.freetype.render_mode = FT_RENDER_MODE_MONO;
            else src.freetype.render_mode = FT_RENDER_MODE_NORMAL;

            build_data_t& dst{ dst_array[src.dst_index] };
            src.src_ranges = config.glyph_config.ranges ? config.glyph_config.ranges : c_font::glyph_t::ranges_default();
            for(const std::uint16_t* src_range{ src.src_ranges }; src_range[0] && src_range[1]; src_range += 2)
                src.glyphs_highest = std::max(src.glyphs_highest, (int)src_range[1]);
            dst.glyphs_highest = std::max(dst.glyphs_highest, src.glyphs_highest);

            return src;
            });

        int total_glyphs_count{ };
        for(build_src_t& src : src_array) {
            build_data_t& dst{ dst_array[src.dst_index] };
            src.glyphs_set.resize((src.glyphs_highest + 32) >> 5, 0);
            if(dst.glyphs_set.empty()) dst.glyphs_set.resize((dst.glyphs_highest + 31) >> 5, 0);

            for(const std::uint16_t* src_range{ src.src_ranges }; src_range[0] && src_range[1]; src_range += 2) {
                for(std::uint32_t codepoint : std::views::iota(src_range[0], src_range[1] + (std::uint32_t)1)) {
                    if(dst.glyphs_set[codepoint >> 5] & (std::uint32_t)1 << (codepoint & 31)) continue;
                    if(!FT_Get_Char_Index(src.freetype.face, codepoint)) continue;

                    src.glyphs_count++;
                    dst.glyphs_count++;
                    src.glyphs_set[codepoint >> 5] |= (std::uint32_t)1 << (codepoint & 31);
                    dst.glyphs_set[codepoint >> 5] |= (std::uint32_t)1 << (codepoint & 31);
                    total_glyphs_count++;
                }
            }
        }

        for(build_src_t& src : src_array) {
            for(int idx : std::views::iota((size_t)0, src.glyphs_set.size())) {
                if(!src.glyphs_set[idx]) continue;

                for(std::uint32_t bit_n : std::views::iota(0, 32)) {
                    if(src.glyphs_set[idx] & ((std::uint16_t)1 << bit_n))
                        src.glyphs_list.push_back({ { ((idx) << 5) + bit_n } });
                }
            }

            src.glyphs_set.clear();
            if(src.glyphs_list.size() != src.glyphs_count) throw std::runtime_error{ "src.glyphs_list.size() != src.glyphs_count" };
        }

        int total_surface{ }, buf_rects_out_n{ };
        std::vector<stbrp_rect> buf_rects{ (size_t)total_glyphs_count };
        std::ranges::transform(src_array, configs, src_array.begin(), [&](build_src_t src, const c_font::config_t& config) {
            if(!src.glyphs_count) return src;

            src.rects = &buf_rects[buf_rects_out_n];
            buf_rects_out_n += src.glyphs_count;

            bool multiply_enabled{ config.rasterizer_multiply != 1.0f };
            std::array<std::uint8_t, 256> multiply_table{ };
            if(multiply_enabled) multiply_calc_lookup_table(multiply_table, config.rasterizer_multiply);

            for(int i : std::views::iota((size_t)0, src.glyphs_list.size())) {
                src_glyph_t& glyph{ src.glyphs_list[i] };

                std::uint32_t glyph_index{ FT_Get_Char_Index(src.freetype.face, glyph.glyph.codepoint) };
                if(!glyph_index) throw std::runtime_error{ "!FT_Get_Char_Index" };
                if(FT_Load_Glyph(src.freetype.face, glyph_index, src.freetype.flags)) throw std::runtime_error{ "FT_Load_Glyph != 0" };

                if(src.freetype.rasterizer_flags & e_rasterizer_flags::bold) FT_GlyphSlot_Embolden(src.freetype.face->glyph);
                if(src.freetype.rasterizer_flags & e_rasterizer_flags::oblique) FT_GlyphSlot_Oblique(src.freetype.face->glyph);

                if(FT_Render_Glyph(src.freetype.face->glyph, src.freetype.render_mode))
                    throw std::runtime_error{ "FT_Render_Glyph != 0" };

                glyph.glyph.corners.min = vec2_t{ src.freetype.face->glyph->bitmap.width, src.freetype.face->glyph->bitmap.rows };
                glyph.glyph.texture_coordinates.min = vec2_t{ src.freetype.face->glyph->bitmap_left, -src.freetype.face->glyph->bitmap_top };
                glyph.glyph.advance_x = std::ceilf(std::floor(src.freetype.face->glyph->advance.x) / 64);

                const FT_Bitmap* ft_bitmap{ &src.freetype.face->glyph->bitmap };
                if(!ft_bitmap) throw std::runtime_error{ "!ft_bitmap" };

                glyph.bitmap.resize(ft_bitmap->width * (ft_bitmap->rows * ft_bitmap->pitch));
                switch(ft_bitmap->pixel_mode) {
                    case FT_PIXEL_MODE_GRAY: {
                        std::ranges::for_each(std::views::iota((std::uint32_t)0, ft_bitmap->rows), [&](const std::uint32_t& y) {
                            std::ranges::for_each(std::views::iota((std::uint32_t)0, ft_bitmap->width), [&](const std::uint32_t& x) {
                                std::uint8_t& buffer_data{ ft_bitmap->buffer[y * ft_bitmap->pitch + x] };
                                glyph.bitmap[y * (int)glyph.glyph.corners.min.x + x] = multiply_enabled ? multiply_table[buffer_data] : buffer_data;
                                });
                            });
                    } break;
                    case FT_PIXEL_MODE_MONO: {
                        for(std::uint32_t y : std::views::iota((std::uint32_t)0, ft_bitmap->rows)) {
                            std::uint8_t bits{ };
                            const std::uint8_t* bits_ptr{ ft_bitmap->buffer + y * ft_bitmap->pitch };
                            for(uint32_t x{ 0 }; x < ft_bitmap->width; x++, bits <<= 1) {
                                if(!(x & 7)) bits = *bits_ptr++;
                                glyph.bitmap[y * (int)glyph.glyph.corners.min.x + x] = (bits & 0x80) ? (multiply_enabled ? multiply_table[255] : 255) : (multiply_enabled ? multiply_table[0] : 0);
                            }
                        }
                    } break;
                    default: throw std::runtime_error("unknown bitmap pixel mode");
                }

                src.rects[i].w = (stbrp_coord)(glyph.glyph.corners.min.x + texture.glyph_padding);
                src.rects[i].h = (stbrp_coord)(glyph.glyph.corners.min.y + texture.glyph_padding);
                total_surface += src.rects[i].w * src.rects[i].h;
            }

            return src;
            });

        int surface_sqrt{ (int)std::sqrtf(total_surface) + 1 };
        texture.size = vec2_t{ (surface_sqrt >= 4096 * 0.7f) ? 4096
            : (surface_sqrt >= 2048 * 0.7f) ? 2048
            : (surface_sqrt >= 1024 * 0.7f) ? 1024
            : 512,
            0 };

        std::vector<stbrp_node> pack_nodes{ (size_t)texture.size.x - texture.glyph_padding };
        stbrp_context pack_context{ };
        stbrp_init_target(&pack_context, texture.size.x, 1024 * 32, pack_nodes.data(), pack_nodes.size());
        pack_custom_rects(&pack_context);

        for(build_src_t& src : src_array) {
            if(!src.glyphs_count) continue;

            stbrp_pack_rects(&pack_context, src.rects, src.glyphs_count);

            for(int i : std::views::iota(0, src.glyphs_count)) {
                if(src.rects[i].was_packed) texture.size.y = std::max((int)texture.size.y, src.rects[i].y + src.rects[i].h);
            }
        }

        texture.size.y = std::powf(2.f, std::ceilf(std::logf(texture.size.y) / std::logf(2.f)));
        texture.uv_scale = vec2_t{ 1.f } / texture.size;
        texture.pixels_alpha8.resize(texture.size.x * texture.size.y);

        std::ranges::transform(src_array, configs, src_array.begin(), [&](build_src_t src, c_font::config_t& config) {
            if(!src.glyphs_count) return src;

            c_font* dst_font{ config.font };

            setup_font(dst_font, &config, src.freetype.info.ascender, src.freetype.info.descender);

            for(int i : std::views::iota(0, src.glyphs_count)) {
                src_glyph_t& glyph{ src.glyphs_list[i] };
                stbrp_rect& pack_rect{ src.rects[i] };
                if(!pack_rect.was_packed) throw std::runtime_error{ "!pack_rect.was_packed" };
                if(!pack_rect.w && !pack_rect.h) continue;

                if(glyph.glyph.corners.min + texture.glyph_padding > vec2_t{ pack_rect.w, pack_rect.h })
                    throw std::runtime_error{ "info.size + texture.glyph_padding > vec2_t{ pack_rect.w, pack_rect.h }" };
                
                vec2_t t{ vec2_t{ pack_rect.x, pack_rect.y } + texture.glyph_padding };
                for(int y : std::views::iota(0, glyph.glyph.corners.min.y)) {
                    std::copy(std::next(glyph.bitmap.begin(), glyph.glyph.corners.min.x * y),
                        std::next(glyph.bitmap.begin(), glyph.glyph.corners.min.x * (y + 1)),
                        std::next(texture.pixels_alpha8.begin(), (t.y * texture.size.x) + t.x + (texture.size.x * y)));
                }

                dst_font->add_glyph(&config, (std::uint16_t)glyph.glyph.codepoint, rect_t{ glyph.glyph.texture_coordinates.min + config.glyph_config.offset + vec2_t{ 0, round(dst_font->ascent) } } + rect_t{ vec2_t{ }, glyph.glyph.corners.min }, rect_t{ t, t + glyph.glyph.corners.min } / texture.size, glyph.glyph.advance_x);
            }

            return src;
            });

        build_finish();

        std::ranges::for_each(src_array, [](build_src_t& src) {
            if(src.freetype.face) { FT_Done_Face(src.freetype.face); src.freetype.face = nullptr; }
            });

        if(FT_Done_Library(ft_library))
            throw std::runtime_error{ "FT_Done_Library != 0" };
    }
    
    void c_atlas::render_lines_texture_data() {
        custom_rect_t* rect = &custom_rects[pack_id_lines];
        if(!rect->is_packed()) throw std::runtime_error{ "!rect->is_packed()" };

        for(std::uint32_t n : std::views::iota(0, 64)) {
            std::uint32_t pad_left{ ((int)rect->size.max.x - n) / 2 }, pad_right{ (int)rect->size.max.x - (pad_left + n) };

            if(pad_left + n + pad_right != rect->size.max.x) throw std::runtime_error{ "pad_left + line_width + pad_right != rect->size.max.x" };
            if(n >= rect->size.max.y) throw std::runtime_error{ "y >= rect->size.max.y" };
            
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

    void c_atlas::render_default_texture_data() {
        custom_rect_t* rect{ &custom_rects[pack_id_cursors] };
        if(!rect->is_packed()) throw std::runtime_error{ "!rect->is_packed()" };
        if(rect->size.max != 2) throw std::runtime_error{ "rect->size.max != 2" };

        const int offset{ (int)(rect->size.min.x + rect->size.min.y * texture.size.x) };
        texture.pixels_alpha8[offset] = texture.pixels_alpha8[offset + 1] = texture.pixels_alpha8[offset + texture.size.x] = texture.pixels_alpha8[offset + texture.size.x + 1] = 0xFF;

        texture.uv_white_pixel = (rect->size.min + vec2_t{ 0.5f }) * texture.uv_scale;
    }

    void c_atlas::pack_custom_rects(stbrp_context* stbrp_context_opaque) {
        if(!stbrp_context_opaque) throw std::runtime_error{ "stbrp_context_opaque == nullptr" };

        std::vector<custom_rect_t>& user_rects{ custom_rects };
        if(user_rects.empty()) throw std::runtime_error{ "user_rects empty" };

        std::vector<stbrp_rect> pack_rects{ user_rects.size() };
        for(int i : std::views::iota((size_t)0, user_rects.size())) {
            pack_rects[i].w = user_rects[i].size.max.x;
            pack_rects[i].h = user_rects[i].size.max.y;
        }

        stbrp_pack_rects(stbrp_context_opaque, &pack_rects[0], pack_rects.size());
        for(int i : std::views::iota((size_t)0, pack_rects.size())) {
            if(pack_rects[i].was_packed) {
                user_rects[i].size.min.x = pack_rects[i].x;
                user_rects[i].size.min.y = pack_rects[i].y;
                
                if(pack_rects[i].w != user_rects[i].size.max.x) throw std::runtime_error{ "pack_rects[i].w != user_rects[i].size.max.x" };
                if(pack_rects[i].h != user_rects[i].size.max.y) throw std::runtime_error{ "pack_rects[i].h != user_rects[i].size.max.y" };

                texture.size.y = std::max((int)texture.size.y, pack_rects[i].y + pack_rects[i].h);
            }
        }
    }

    void c_atlas::multiply_calc_lookup_table(std::array<std::uint8_t, 256>& out_table, float in_multiply_factor) {
        for(std::uint32_t i : std::views::iota(0, 256)) {
            std::uint32_t value{ (std::uint32_t)(i * in_multiply_factor) };
            out_table[i] = value > 255 ? 255 : (value & 0xFF);
        }
    }

    c_font* c_atlas::add_font(c_font::config_t* config) {
        if(locked) throw std::runtime_error{ "cannot modify a locked atlas between begin_render() and end_render/render()!" };
        if(config->font) throw std::runtime_error{ "config->font == nullptr" };
        if(config->data.empty()) throw std::runtime_error{ "cofig->data.empty()" };
        if(config->size_pixels <= 0.f) throw std::runtime_error{ "config->size_pixels <= 0.f" };

        if(!config->merge_mode) fonts.push_back(c_font{ });
        else if(fonts.empty()) throw std::runtime_error{ "cannot use merge_mode for the first font" };

        configs.push_back(*config);
        c_font::config_t& cfg{ configs.back() };
        
        if(!cfg.font) cfg.font = &fonts.back();
        if(!cfg.owned_by_atlas) {
            cfg.owned_by_atlas = true;
        }

        if(cfg.font->ellipsis_char == (std::uint16_t)-1)
            cfg.font->ellipsis_char = config->ellipsis_char;

        texture.clear();
        return cfg.font;
    }

    c_font* c_atlas::add_font_default(c_font::config_t* config) {
        c_font::config_t cfg{ config ? *config : c_font::config_t{ } };
        if(!config) {
            cfg.oversample = 1;
            cfg.pixel_snap_h = true;
        }
        if(cfg.size_pixels <= 0.0f) cfg.size_pixels = 13.0f * 1.0f;
        cfg.ellipsis_char = (std::uint16_t)0x0085;
        cfg.glyph_config.offset.y = 1.0f * floor(cfg.size_pixels / 13.0f);

        return add_font_from_memory_compressed_base_85_ttf(compressed_fonts::proggy_clean, cfg.size_pixels, &cfg, cfg.glyph_config.ranges ? cfg.glyph_config.ranges : c_font::glyph_t::ranges_default());
    }

    c_font* c_atlas::add_font_from_file_ttf(const char* filename, float size_pixels, c_font::config_t* config, const std::uint16_t* glyph_ranges) {
        if(locked) throw std::runtime_error{ "cannot modify a locked atlas between begin_render() and end_render/render()!" };
        
        std::ifstream file{ filename, std::ios::in | std::ios::binary | std::ios::ate };
        if(!file.is_open()) throw std::runtime_error{ "cannot open font file" };
        
        std::vector<char> font_file((std::size_t)file.tellg());
        file.seekg(0, std::ios::beg);
        file.read(font_file.data(), font_file.size());
        file.close();

        return add_font_from_memory_ttf(font_file, size_pixels, config, glyph_ranges);
    }

    c_font* c_atlas::add_font_from_memory_ttf(const std::vector<char>& font_file, float size_pixels, c_font::config_t* config, const std::uint16_t* glyph_ranges) {
        if(locked) throw std::runtime_error{ "cannot modify a locked atlas between begin_render() and end_render/render()!" };

        c_font::config_t cfg{ config ? *config : c_font::config_t{ } };
        if(!cfg.data.empty()) throw std::runtime_error{ "!cfg.data.empty()" };
        
        cfg.data = font_file;
        cfg.size_pixels = size_pixels;
        if(glyph_ranges) cfg.glyph_config.ranges = glyph_ranges;
        
        return add_font(&cfg);
    }

    c_font* c_atlas::add_font_from_memory_compressed_ttf(const std::vector<char>& compressed_ttf, float size_pixels, c_font::config_t* config, const std::uint16_t* glyph_ranges) {
        std::vector<char> buf_decompressed_data(impl::stb::decompress_length((std::uint8_t*)compressed_ttf.data()));
        impl::stb::decompress((std::uint8_t*)buf_decompressed_data.data(), (const std::uint8_t*)compressed_ttf.data());

        c_font::config_t cfg{ config ? *config : c_font::config_t{ } };
        if(!cfg.data.empty()) throw std::runtime_error{ "!cfg.data.empty()" };
        cfg.owned_by_atlas = true;

        return add_font_from_memory_ttf(buf_decompressed_data, size_pixels, &cfg, glyph_ranges);
    }

    c_font* c_atlas::add_font_from_memory_compressed_base_85_ttf(const char* compressed_font_data_base85, float size_pixels, c_font::config_t* config, const std::uint16_t* glyph_ranges) {
        std::vector<char> compressed_data((strlen(compressed_font_data_base85) + 4) / 5 * 4);
        impl::decode85((const std::uint8_t*)compressed_font_data_base85, (std::uint8_t*)compressed_data.data());
        return add_font_from_memory_compressed_ttf(compressed_data, size_pixels, config, glyph_ranges);
    }


    void c_atlas::clear_input_data() {
        if(locked) throw std::runtime_error{ "cannot modify a locked atlas between begin_render() and end_render/render()!" };

        for(c_font::config_t& config : configs) {
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
}