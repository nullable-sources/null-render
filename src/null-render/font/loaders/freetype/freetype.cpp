#include <future>
#include "freetype.h"

namespace null::render {
    void c_freetype_loader::build(c_atlas* atlas) {
        if(atlas->locked) { utils::logger(utils::e_log_type::error, "cannot modify a locked atlas between begin_render() and end_render/render()."); return; }
        if(atlas->configs.empty()) { utils::logger(utils::e_log_type::warning, "configs array is empty."); return; }

        FT_Library ft_library{ };
        if(auto result = FT_Init_FreeType(&ft_library); result) {
            utils::logger(utils::e_log_type::error, "FT_Init_FreeType failed, return code {}.", result);
            return;
        }

        atlas->texture = c_atlas::texture_t{ };

        std::vector<build_src_t> src_array(atlas->configs.size());
        std::vector<build_data_t> dst_array(atlas->fonts.size());

        for(auto [src, config] : std::views::zip(src_array, atlas->configs)) {
            if(!config.font) { utils::logger(utils::e_log_type::warning, "config font is empty."); continue; }
            if(config.font->is_loaded() && config.font->container_atlas != atlas) {
                utils::logger(utils::e_log_type::warning, "font already loaded by another atlas.");
                continue;
            }

            if(auto iterator = std::ranges::find_if(atlas->fonts, [=](const std::unique_ptr<c_font>& font) { return config.font == font.get(); }); iterator != atlas->fonts.end()) src.dst_index = std::distance(atlas->fonts.begin(), iterator);
            else { utils::logger(utils::e_log_type::error, "font configuration could not be found."); continue; }

            if(auto result = FT_New_Memory_Face(ft_library, (std::uint8_t*)config.data.data(), (std::uint32_t)config.data.size(), (std::uint32_t)config.index, &src.freetype.face); result) {
                utils::logger(utils::e_log_type::error, "FT_New_Memory_Face failed, return code {}.", result);
                continue;
            }
            if(auto result = FT_Select_Charmap(src.freetype.face, FT_ENCODING_UNICODE); result) {
                utils::logger(utils::e_log_type::error, "FT_Select_Charmap failed, return code {}.", result);
                continue;
            }

            FT_Size_RequestRec req{
                .type{ FT_SIZE_REQUEST_TYPE_REAL_DIM },
                .width{ 0 },
                .height{ (FT_Long)config.size_pixels * 64 },
                .horiResolution{ 0 }, .vertResolution{ 0 }
            };
            if(auto result = FT_Request_Size(src.freetype.face, &req); result) {
                utils::logger(utils::e_log_type::error, "FT_Request_Size failed, return code {}.", result);
                continue;
            }

            FT_Size_Metrics metrics = src.freetype.face->size->metrics;
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

            switch(config.render_mode_type) {
                case e_render_mode_type::monochrome: { src.freetype.render_mode = FT_RENDER_MODE_MONO; } break;
                case e_render_mode_type::sdf: { src.freetype.render_mode = FT_RENDER_MODE_SDF; }
                default: { src.freetype.render_mode = FT_RENDER_MODE_NORMAL; } break;
            }

            build_data_t& dst = dst_array[src.dst_index];
            src.src_ranges = config.glyph_config.ranges ? config.glyph_config.ranges : c_font::glyph_t::ranges_default();
            for(const std::uint16_t* src_range = src.src_ranges; src_range[0] && src_range[1]; src_range += 2)
                src.glyphs_highest = std::max(src.glyphs_highest, (int)src_range[1]);
            dst.glyphs_highest = std::max(dst.glyphs_highest, src.glyphs_highest);
        }

        int total_glyphs_count{ };
        for(build_src_t& src : src_array) {
            build_data_t& dst = dst_array[src.dst_index];
            src.glyphs_set.resize((src.glyphs_highest + 32) >> 5, 0);
            if(dst.glyphs_set.empty()) dst.glyphs_set.resize((dst.glyphs_highest + 31) >> 5, 0);

            for(const std::uint16_t* src_range = src.src_ranges; src_range[0] && src_range[1]; src_range += 2) {
                for(const std::uint32_t& codepoint : std::views::iota(src_range[0], src_range[1] + (std::uint32_t)1)) {
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
            for(int idx : std::views::iota(0u, src.glyphs_set.size())) {
                if(!src.glyphs_set[idx]) continue;

                for(std::uint32_t bit_n : std::views::iota(0, 32)) {
                    if(src.glyphs_set[idx] & ((std::uint16_t)1 << bit_n))
                        src.glyphs_list.push_back({ { ((idx) << 5) + bit_n } });
                }
            }

            src.glyphs_set.clear();
            if(src.glyphs_list.size() != src.glyphs_count) utils::logger(utils::e_log_type::error, "src.glyphs_list.size() != src.glyphs_count.");
        }

        std::mutex mutex{ };
        std::vector<std::future<void>> futures{ };

        int total_surface{ }, buf_rects_out_n{ }, buf_packedchars_out_n{ };
        std::vector<stbrp_rect> buf_rects((size_t)total_glyphs_count);
        for(auto [src, config] : std::views::zip(src_array, atlas->configs)) {
            src.rects = &buf_rects[buf_rects_out_n];
            buf_rects_out_n += src.glyphs_count;

            for(int i : std::views::iota(0u, src.glyphs_list.size())) {
                futures.push_back(std::async(std::launch::async, [&](build_src_t* src, src_glyph_t* glyph, int index) {
                    std::lock_guard lock_guard(mutex);

                    std::uint32_t glyph_index = FT_Get_Char_Index(src->freetype.face, glyph->glyph.codepoint);
                    if(!glyph_index) { utils::logger(utils::e_log_type::error, "FT_Get_Char_Index return 0."); return; }
                    if(auto result = FT_Load_Glyph(src->freetype.face, glyph_index, src->freetype.flags); result) { utils::logger(utils::e_log_type::error, "FT_Load_Glyph failed, return code {}.", result); return; }

                    if(src->freetype.rasterizer_flags & e_rasterizer_flags::bold) FT_GlyphSlot_Embolden(src->freetype.face->glyph);
                    if(src->freetype.rasterizer_flags & e_rasterizer_flags::oblique) FT_GlyphSlot_Oblique(src->freetype.face->glyph);

                    if(auto result = FT_Render_Glyph(src->freetype.face->glyph, src->freetype.render_mode); result) {
                        utils::logger(utils::e_log_type::error, "FT_Render_Glyph failed, return code {}.", result);
                        return;
                    }

                    glyph->glyph.corners.min = vec2_t(src->freetype.face->glyph->bitmap.width, src->freetype.face->glyph->bitmap.rows);
                    glyph->glyph.texture_coordinates.min = vec2_t(src->freetype.face->glyph->bitmap_left, -src->freetype.face->glyph->bitmap_top);
                    glyph->glyph.advance_x = std::ceilf(std::floor(src->freetype.face->glyph->advance.x) / 64);

                    const FT_Bitmap* ft_bitmap = &src->freetype.face->glyph->bitmap;
                    if(!ft_bitmap) {
                        utils::logger(utils::e_log_type::error, "ft_bitmap is empty.");
                        return;
                    }

                    glyph->bitmap.resize(ft_bitmap->width * (ft_bitmap->rows * ft_bitmap->pitch));
                    switch(ft_bitmap->pixel_mode) {
                        case FT_PIXEL_MODE_GRAY: {
                            for(std::uint32_t y : std::views::iota(0u, ft_bitmap->rows)) {
                                for(std::uint32_t x : std::views::iota(0u, ft_bitmap->width)) {
                                    glyph->bitmap[y * (int)glyph->glyph.corners.min.x + x] = ft_bitmap->buffer[y * ft_bitmap->pitch + x];
                                }
                            }
                        } break;
                        case FT_PIXEL_MODE_MONO: {
                            for(std::uint32_t y : std::views::iota(0u, ft_bitmap->rows)) {
                                std::uint8_t bits{ };
                                const std::uint8_t* bits_ptr = ft_bitmap->buffer + y * ft_bitmap->pitch;
                                for(uint32_t x = 0; x < ft_bitmap->width; x++, bits <<= 1) {
                                    if(!(x & 7)) bits = *bits_ptr++;
                                    glyph->bitmap[y * (int)glyph->glyph.corners.min.x + x] = (bits & 0x80) ? 255 : 0;
                                }
                            }
                        } break;
                        default: utils::logger(utils::e_log_type::error, "unsupported pixel mode.");
                    }

                    src->rects[index].w = (stbrp_coord)(glyph->glyph.corners.min.x + atlas->texture.glyph_padding);
                    src->rects[index].h = (stbrp_coord)(glyph->glyph.corners.min.y + atlas->texture.glyph_padding);
                    total_surface += src->rects[index].w * src->rects[index].h;
                }, &src, &src.glyphs_list[i], i));
            }
        }

        std::ranges::for_each(futures, &std::future<void>::wait);

        int surface_sqrt = std::sqrtf(total_surface) + 1;
        atlas->texture.size = vec2_t((surface_sqrt >= 4096 * 0.7f) ? 4096
            : (surface_sqrt >= 2048 * 0.7f) ? 2048
            : (surface_sqrt >= 1024 * 0.7f) ? 1024
            : 512,
            0);

        std::vector<stbrp_node> pack_nodes((size_t)atlas->texture.size.x - atlas->texture.glyph_padding);
        stbrp_context pack_context{ };
        stbrp_init_target(&pack_context, atlas->texture.size.x, 1024 * 32, pack_nodes.data(), pack_nodes.size());

        for(build_src_t& src : src_array) {
            if(!src.glyphs_count) continue;

            stbrp_pack_rects(&pack_context, src.rects, src.glyphs_count);

            for(int i : std::views::iota(0, src.glyphs_count)) {
                if(src.rects[i].was_packed) atlas->texture.size.y = std::max((int)atlas->texture.size.y, src.rects[i].y + src.rects[i].h);
            }
        }

        atlas->texture.size.y = std::powf(2.f, std::ceilf(std::logf(atlas->texture.size.y) / std::logf(2.f)));
        atlas->texture.pixels_alpha8.resize(atlas->texture.size.x * atlas->texture.size.y);

        for(auto [src, config] : std::views::zip(src_array, atlas->configs)) {
            if(!src.glyphs_count) continue;

            c_font* dst_font = config.font;

            atlas->setup_font(dst_font, &config, src.freetype.info.ascender, src.freetype.info.descender);

            for(int i : std::views::iota(0, src.glyphs_count)) {
                src_glyph_t& glyph = src.glyphs_list[i];
                stbrp_rect& pack_rect = src.rects[i];
                if(!pack_rect.was_packed) { utils::logger(utils::e_log_type::error, "pack_rect not packed."); continue; }
                if(!pack_rect.w && !pack_rect.h) continue;

                if(glyph.glyph.corners.min + atlas->texture.glyph_padding > vec2_t{ pack_rect.w, pack_rect.h }) {
                    utils::logger(utils::e_log_type::error, "info.size + texture.glyph_padding > vec2_t{ pack_rect.w, pack_rect.h }."); continue;
                }

                vec2_t<float> t(pack_rect.x, pack_rect.y);
                for(int y : std::views::iota(0, glyph.glyph.corners.min.y)) {
                    std::copy(std::next(glyph.bitmap.begin(), glyph.glyph.corners.min.x * y),
                        std::next(glyph.bitmap.begin(), glyph.glyph.corners.min.x * (y + 1)),
                        std::next(atlas->texture.pixels_alpha8.begin(), (t.y * atlas->texture.size.x) + t.x + (atlas->texture.size.x * y)));
                }

                dst_font->add_glyph(&config, (std::uint16_t)glyph.glyph.codepoint, rect_t(glyph.glyph.texture_coordinates.min + config.glyph_config.offset + vec2_t(0.f, round(dst_font->ascent))) + rect_t(vec2_t<float>(), glyph.glyph.corners.min), rect_t(t, t + glyph.glyph.corners.min) / atlas->texture.size, glyph.glyph.advance_x);
            }
        }

        atlas->build_finish();

        for(build_src_t& src : src_array) {
            if(src.freetype.face) { FT_Done_Face(src.freetype.face); src.freetype.face = nullptr; }
        }

        if(auto result = FT_Done_Library(ft_library); result)
            utils::logger(utils::e_log_type::error, "FT_Done_Library failed, result code {}.", result);
    }
}