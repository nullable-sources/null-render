#include <font/font.h>
#include <future>
#include <font/loaders/truetype/truetype.h>

namespace null::render {
    void c_truetype_loader::build(c_atlas* atlas) {
        if(atlas->locked) { utils::logger(utils::e_log_type::error, "cannot modify a locked atlas between begin_render() and end_render/render()."); return; }
        if(atlas->configs.empty()) { utils::logger(utils::e_log_type::warning, "configs array is empty."); return; }

        atlas->texture = c_atlas::texture_t{ };

        std::vector<build_src_t> src_array(atlas->configs.size());
        std::vector<build_data_t> dst_array(atlas->fonts.size());

        for(auto [src, config] : std::views::zip(src_array, atlas->configs)) {
            if(!config.font) { utils::logger(utils::e_log_type::warning, "config font is empty."); continue; }
            if(config.font->is_loaded() && config.font->container_atlas != atlas) {
                utils::logger(utils::e_log_type::warning, "font already loaded by another atlas.");
                continue;
            }

            if(auto iterator{ std::ranges::find_if(atlas->fonts, [=](const std::unique_ptr<c_font>& font) { return config.font == font.get(); }) }; iterator != atlas->fonts.end()) src.dst_index = std::distance(atlas->fonts.begin(), iterator);
            else { utils::logger(utils::e_log_type::error, "font configuration could not be found."); continue; }

            const int font_offset{ stbtt_GetFontOffsetForIndex((std::uint8_t*)config.data.data(), config.index) };
            if(font_offset < 0) { utils::logger(utils::e_log_type::error, "font_data is incorrect, or font_no cannot be found."); continue; }
            if(auto result{ stbtt_InitFont(&src.font_info, (std::uint8_t*)config.data.data(), font_offset) }; !result) {
                utils::logger(utils::e_log_type::error, "stbtt_InitFont failed, return code {}.", result);
                continue;
            }

            build_data_t& dst{ dst_array[src.dst_index] };
            src.src_ranges = config.glyph_config.ranges ? config.glyph_config.ranges : c_font::glyph_t::ranges_default();
            for(const std::uint16_t* src_range{ src.src_ranges }; src_range[0] && src_range[1]; src_range += 2)
                src.glyphs_highest = std::max(src.glyphs_highest, (int)src_range[1]);
            dst.glyphs_highest = std::max(dst.glyphs_highest, src.glyphs_highest);
        }

        int total_glyphs_count{ };
        for(build_src_t& src : src_array) {
            build_data_t& dst{ dst_array[src.dst_index] };
            src.glyphs_set.resize((src.glyphs_highest + 32) >> 5, 0);
            if(dst.glyphs_set.empty()) dst.glyphs_set.resize((dst.glyphs_highest + 31) >> 5, 0);

            for (const std::uint16_t* src_range{ src.src_ranges }; src_range[0] && src_range[1]; src_range += 2) {
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
            for(int idx : std::views::iota(0u, src.glyphs_set.size())) {
                if(!src.glyphs_set[idx]) continue;

                for(std::uint32_t bit_n : std::views::iota(0, 32)) {
                    if(src.glyphs_set[idx] & ((std::uint16_t)1 << bit_n))
                        src.glyphs_list.push_back(((idx) << 5) + bit_n);
                }
            }

            src.glyphs_set.clear();
            if(src.glyphs_list.size() != src.glyphs_count) utils::logger(utils::e_log_type::error, "src.glyphs_list.size() != src.glyphs_count.");
        }

        std::vector<stbrp_rect> buf_rects(total_glyphs_count);
        std::vector<stbtt_packedchar> buf_packedchars(total_glyphs_count);

        int total_surface{ }, buf_rects_out_n{ }, buf_packedchars_out_n{ };

        for(auto [src, config] : std::views::zip(src_array, atlas->configs)) {
            if(!src.glyphs_count) continue;

            src.rects = &buf_rects[buf_rects_out_n];
            src.packed_chars = &buf_packedchars[buf_packedchars_out_n];
            buf_rects_out_n += src.glyphs_count;
            buf_packedchars_out_n += src.glyphs_count;

            bool sdf{ config.render_mode_type == e_render_mode_type::sdf };
            src.pack_range = { sdf ? sdf_pixel_size : config.size_pixels, 0, src.glyphs_list.data(), (int)src.glyphs_list.size(),
                src.packed_chars, (std::uint8_t)(sdf ? 1 : config.oversample.x), (std::uint8_t)(sdf ? 1 : config.oversample.y)
            };

            float scale{ (sdf || config.size_pixels > 0) ? stbtt_ScaleForPixelHeight(&src.font_info, sdf ? sdf_pixel_size : config.size_pixels) : stbtt_ScaleForMappingEmToPixels(&src.font_info, -config.size_pixels) };
            int padding{ atlas->texture.glyph_padding + (2 * sdf_padding + 1) };
            for(int i : std::views::iota(0u, src.glyphs_list.size())) {
                int x0{ }, y0{ }, x1{ }, y1{ };
                int glyph_index_in_font{ stbtt_FindGlyphIndex(&src.font_info, src.glyphs_list[i]) };
                if(!glyph_index_in_font) { utils::logger(utils::e_log_type::error, "stbtt_FindGlyphIndex return 0."); continue; }
                stbtt_GetGlyphBitmapBoxSubpixel(&src.font_info, glyph_index_in_font, scale * (sdf ? 1 : config.oversample.x), scale * (sdf ? 1 : config.oversample.y), 0, 0, &x0, &y0, &x1, &y1);
                src.rects[i].w = (stbrp_coord)(x1 - x0 + padding + (sdf ? 0 : config.oversample.x - 1));
                src.rects[i].h = (stbrp_coord)(y1 - y0 + padding + (sdf ? 0 : config.oversample.y - 1));
                total_surface += src.rects[i].w * src.rects[i].h;
            }
        }

        const int surface_sqrt{ (int)std::sqrt(total_surface) + 1 };
        atlas->texture.size.y = 0;
        if(atlas->texture.desired_width > 0) atlas->texture.size.x = atlas->texture.desired_width;
        else atlas->texture.size.x = (surface_sqrt >= 4096 * 0.7f) ? 4096 : (surface_sqrt >= 2048 * 0.7f) ? 2048 : (surface_sqrt >= 1024 * 0.7f) ? 1024 : 512;

        stbtt_pack_context spc{ };
        stbtt_PackBegin(&spc, nullptr, atlas->texture.size.x, 1024 * 32, 0, atlas->texture.glyph_padding + (2 * sdf_padding + 1), nullptr);

        for(build_src_t& src : src_array) {
            if(src.glyphs_count == 0)
                continue;

            stbrp_pack_rects((stbrp_context*)spc.pack_info, src.rects, src.glyphs_count);

            for(int i : std::views::iota(0, src.glyphs_count)) {
                if(src.rects[i].was_packed) atlas->texture.size.y = std::max((int)atlas->texture.size.y, src.rects[i].y + src.rects[i].h);
            }
        }

        atlas->texture.size.y = std::powf(2.f, std::ceilf(std::logf(atlas->texture.size.y) / std::logf(2.f)));
        atlas->texture.pixels_alpha8.resize(atlas->texture.size.x * atlas->texture.size.y, 0);
        spc.pixels = atlas->texture.pixels_alpha8.data();
        spc.height = atlas->texture.size.y;

        struct async_args_t {
        public:
            stbtt_pack_context* spc{ };
            const stbtt_fontinfo* info{ };
            stbtt_pack_range range{ };
            stbrp_rect* rects{ };
            bool sdf{ };
        };

        std::mutex mutex{ };
        std::vector<std::future<void>> futures{ };
        for(auto [src, config] : std::views::zip(src_array, atlas->configs)) {
            if(!src.glyphs_count) continue;

            static constexpr int batch_size{ 10 };
            for(int i{ 0 }; i < src.pack_range.num_chars; i += batch_size) {
                async_args_t args{ &spc, &src.font_info, src.pack_range, &src.rects[i], config.render_mode_type == e_render_mode_type::sdf };
                args.range.chardata_for_range = &args.range.chardata_for_range[i];
                args.range.first_unicode_codepoint_in_range += i;
                if(args.range.array_of_unicode_codepoints)
                    args.range.array_of_unicode_codepoints = &args.range.array_of_unicode_codepoints[i];
                args.range.num_chars = std::min(batch_size, args.range.num_chars - i);

                futures.push_back(std::async([&](async_args_t args) {
                    mutex.lock();
                    stbtt_PackFontRangesRenderIntoRects(args.spc, args.info, &args.range, 1, args.rects, args.sdf);
                    mutex.unlock();
                    }, args));
            }
        }

        std::ranges::for_each(futures, &std::future<void>::wait);

        stbtt_PackEnd(&spc);

        for(auto [src, config] : std::views::zip(src_array, atlas->configs)) {
            if(!src.glyphs_count) continue;

            c_font* dst_font{ config.font };

            bool sdf{ config.render_mode_type == e_render_mode_type::sdf };
            const float font_scale{ stbtt_ScaleForPixelHeight(&src.font_info, sdf ? sdf_pixel_size : config.size_pixels) };
            int unscaled_ascent{ }, unscaled_descent{ }, unscaled_line_gap{ };
            stbtt_GetFontVMetrics(&src.font_info, &unscaled_ascent, &unscaled_descent, &unscaled_line_gap);

            const float ascent{ std::floorf(unscaled_ascent * font_scale + ((unscaled_ascent > 0.0f) ? +1 : -1)) };
            const float descent{ std::floorf(unscaled_descent * font_scale + ((unscaled_descent > 0.0f) ? +1 : -1)) };
            atlas->setup_font(dst_font, &config, ascent, descent);
            const vec2_t font_off{ config.glyph_config.offset + vec2_t{ 0.f, std::round(dst_font->ascent) } };

            for(int glyph_i : std::views::iota(0, src.glyphs_count)) {
                const int codepoint{ src.glyphs_list[glyph_i] };
                stbtt_aligned_quad q{ };
                float unused_x{ }, unused_y{ };
                stbtt_GetPackedQuad(src.packed_chars, atlas->texture.size.x, atlas->texture.size.y, glyph_i, &unused_x, &unused_y, &q, 0);
                rect_t corners{ rect_t{ q.x0, q.y0, q.x1, q.y1 } + font_off };
                rect_t uvs{ rect_t{ q.s0, q.t0, q.s1, q.t1 } };

                float scale{ 1.0f };
                if(sdf) {
                    vec2_t<float> xy_padding{ sdf_padding };
                    vec2_t<float> uv_padding{ xy_padding / atlas->texture.size };

                    scale = float(config.size_pixels) / sdf_pixel_size;
                    corners.min -= xy_padding;
                    corners.max += xy_padding;

                    uvs.min -= uv_padding;
                    uvs.max += uv_padding;
                }

                dst_font->add_glyph(&config, (std::uint16_t)codepoint, corners * scale, uvs, src.packed_chars[glyph_i].xadvance * scale);
            }
        }

        atlas->build_finish();
    }
}