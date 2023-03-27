#include <future>
#include <font/font.h>
#include <font/loaders/freetype/freetype.h>

namespace null::render {
	void c_freetype_loader::build(c_atlas* atlas) {
        if(atlas->locked) throw std::runtime_error{ "cannot modify a locked atlas between begin_render() and end_render/render()!" };
        if(atlas->configs.empty()) throw std::runtime_error{ "configs.empty()" };

        FT_Library ft_library{ };
        if(FT_Init_FreeType(&ft_library))
            throw std::runtime_error{ "FT_Init_FreeType != 0" };

        atlas->texture = c_atlas::texture_t{ };

        std::vector<build_src_t> src_array{ atlas->configs.size() };
        std::vector<build_data_t> dst_array{ atlas->fonts.size() };

        for(std::tuple<build_src_t&, c_font::config_t&> tuple : std::views::zip(src_array, atlas->configs)) {
            build_src_t& src{ std::get<build_src_t&>(tuple) };
            c_font::config_t& config{ std::get<c_font::config_t&>(tuple) };

            if(!config.font) throw std::runtime_error{ "!cfg.font" };
            if(config.font->is_loaded() && config.font->container_atlas != atlas) throw std::runtime_error{ "cfg.font->is_loaded() && cfg.font->container_atlas != this" };

            if(auto iterator{ std::ranges::find_if(atlas->fonts, [=](const std::unique_ptr<c_font>& font) { return config.font == font.get(); }) }; iterator != atlas->fonts.end()) src.dst_index = std::distance(atlas->fonts.begin(), iterator);
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

            switch(config.render_mode_type) {
                case e_render_mode_type::monochrome: { src.freetype.render_mode = FT_RENDER_MODE_MONO; } break;
                case e_render_mode_type::sdf: { utils::logger.log(utils::e_log_type::warning, "freetype font loader does not support sdf fonts yet."); }
                default: { src.freetype.render_mode = FT_RENDER_MODE_NORMAL; } break;
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

            for(const std::uint16_t* src_range{ src.src_ranges }; src_range[0] && src_range[1]; src_range += 2) {
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
            for(const int& idx : std::views::iota((size_t)0, src.glyphs_set.size())) {
                if(!src.glyphs_set[idx]) continue;

                for(const std::uint32_t& bit_n : std::views::iota(0, 32)) {
                    if(src.glyphs_set[idx] & ((std::uint16_t)1 << bit_n))
                        src.glyphs_list.push_back({ { ((idx) << 5) + bit_n } });
                }
            }

            src.glyphs_set.clear();
            if(src.glyphs_list.size() != src.glyphs_count) throw std::runtime_error{ "src.glyphs_list.size() != src.glyphs_count" };
        }

        std::mutex mutex{ };
        std::vector<std::future<void>> futures{ };

        int total_surface{ }, buf_rects_out_n{ }, buf_packedchars_out_n{ };
        std::vector<stbrp_rect> buf_rects{ (size_t)total_glyphs_count };
        for(std::tuple<build_src_t&, c_font::config_t&> tuple : std::views::zip(src_array, atlas->configs)) {
            build_src_t& src{ std::get<build_src_t&>(tuple) };
            c_font::config_t& config{ std::get<c_font::config_t&>(tuple) };

            src.rects = &buf_rects[buf_rects_out_n];
            buf_rects_out_n += src.glyphs_count;

            for(const int& i : std::views::iota((size_t)0, src.glyphs_list.size())) {
                futures.push_back(std::async(std::launch::async, [&](build_src_t* src, src_glyph_t* glyph, int index) {
                    mutex.lock();

                    std::uint32_t glyph_index{ FT_Get_Char_Index(src->freetype.face, glyph->glyph.codepoint) };
                    if(!glyph_index) throw std::runtime_error{ "!FT_Get_Char_Index" };
                    if(FT_Load_Glyph(src->freetype.face, glyph_index, src->freetype.flags)) throw std::runtime_error{ "FT_Load_Glyph != 0" };

                    if(src->freetype.rasterizer_flags & e_rasterizer_flags::bold) FT_GlyphSlot_Embolden(src->freetype.face->glyph);
                    if(src->freetype.rasterizer_flags & e_rasterizer_flags::oblique) FT_GlyphSlot_Oblique(src->freetype.face->glyph);

                    if(FT_Render_Glyph(src->freetype.face->glyph, src->freetype.render_mode))
                        throw std::runtime_error{ "FT_Render_Glyph != 0" };

                    glyph->glyph.corners.min = vec2_t{ src->freetype.face->glyph->bitmap.width, src->freetype.face->glyph->bitmap.rows };
                    glyph->glyph.texture_coordinates.min = vec2_t{ src->freetype.face->glyph->bitmap_left, -src->freetype.face->glyph->bitmap_top };
                    glyph->glyph.advance_x = std::ceilf(std::floor(src->freetype.face->glyph->advance.x) / 64);

                    const FT_Bitmap* ft_bitmap{ &src->freetype.face->glyph->bitmap };
                    if(!ft_bitmap) throw std::runtime_error{ "!ft_bitmap" };

                    glyph->bitmap.resize(ft_bitmap->width * (ft_bitmap->rows * ft_bitmap->pitch));
                    switch(ft_bitmap->pixel_mode) {
                        case FT_PIXEL_MODE_GRAY: {
                            std::ranges::for_each(std::views::iota((std::uint32_t)0, ft_bitmap->rows), [&](const std::uint32_t& y) {
                                std::ranges::for_each(std::views::iota((std::uint32_t)0, ft_bitmap->width), [&](const std::uint32_t& x) {
                                    std::uint8_t& buffer_data{ ft_bitmap->buffer[y * ft_bitmap->pitch + x] };
                                    glyph->bitmap[y * (int)glyph->glyph.corners.min.x + x] = buffer_data;
                                    });
                                });
                        } break;
                        case FT_PIXEL_MODE_MONO: {
                            for(const std::uint32_t& y : std::views::iota((std::uint32_t)0, ft_bitmap->rows)) {
                                std::uint8_t bits{ };
                                const std::uint8_t* bits_ptr{ ft_bitmap->buffer + y * ft_bitmap->pitch };
                                for(uint32_t x{ 0 }; x < ft_bitmap->width; x++, bits <<= 1) {
                                    if(!(x & 7)) bits = *bits_ptr++;
                                    glyph->bitmap[y * (int)glyph->glyph.corners.min.x + x] = (bits & 0x80) ? 255 : 0;
                                }
                            }
                        } break;
                        default: throw std::runtime_error("unknown bitmap pixel mode");
                    }

                    src->rects[index].w = (stbrp_coord)(glyph->glyph.corners.min.x + atlas->texture.glyph_padding);
                    src->rects[index].h = (stbrp_coord)(glyph->glyph.corners.min.y + atlas->texture.glyph_padding);
                    total_surface += src->rects[index].w * src->rects[index].h;

                    mutex.unlock();
                }, &src, &src.glyphs_list[i], i));
            }
        }

        for(std::future<void>& future : futures) future.wait();

        int surface_sqrt{ (int)std::sqrtf(total_surface) + 1 };
        atlas->texture.size = vec2_t{ (surface_sqrt >= 4096 * 0.7f) ? 4096
            : (surface_sqrt >= 2048 * 0.7f) ? 2048
            : (surface_sqrt >= 1024 * 0.7f) ? 1024
            : 512,
            0 };

        std::vector<stbrp_node> pack_nodes{ (size_t)atlas->texture.size.x - atlas->texture.glyph_padding };
        stbrp_context pack_context{ };
        stbrp_init_target(&pack_context, atlas->texture.size.x, 1024 * 32, pack_nodes.data(), pack_nodes.size());

        for(build_src_t& src : src_array) {
            if(!src.glyphs_count) continue;

            stbrp_pack_rects(&pack_context, src.rects, src.glyphs_count);

            for(const int& i : std::views::iota(0, src.glyphs_count)) {
                if(src.rects[i].was_packed) atlas->texture.size.y = std::max((int)atlas->texture.size.y, src.rects[i].y + src.rects[i].h);
            }
        }

        atlas->texture.size.y = std::powf(2.f, std::ceilf(std::logf(atlas->texture.size.y) / std::logf(2.f)));
        atlas->texture.pixels_alpha8.resize(atlas->texture.size.x * atlas->texture.size.y);

        for(std::tuple<build_src_t&, c_font::config_t&> tuple : std::views::zip(src_array, atlas->configs)) {
            build_src_t& src{ std::get<build_src_t&>(tuple) };
            c_font::config_t& config{ std::get<c_font::config_t&>(tuple) };

            if(!src.glyphs_count) continue;

            c_font* dst_font{ config.font };

            atlas->setup_font(dst_font, &config, src.freetype.info.ascender, src.freetype.info.descender);

            for(const int& i : std::views::iota(0, src.glyphs_count)) {
                src_glyph_t& glyph{ src.glyphs_list[i] };
                stbrp_rect& pack_rect{ src.rects[i] };
                if(!pack_rect.was_packed) throw std::runtime_error{ "!pack_rect.was_packed" };
                if(!pack_rect.w && !pack_rect.h) continue;

                if(glyph.glyph.corners.min + atlas->texture.glyph_padding > vec2_t{ pack_rect.w, pack_rect.h })
                    throw std::runtime_error{ "info.size + texture.glyph_padding > vec2_t{ pack_rect.w, pack_rect.h }" };

                vec2_t<float> t{ vec2_t{ pack_rect.x, pack_rect.y } };
                for(const int& y : std::views::iota(0, glyph.glyph.corners.min.y)) {
                    std::copy(std::next(glyph.bitmap.begin(), glyph.glyph.corners.min.x * y),
                        std::next(glyph.bitmap.begin(), glyph.glyph.corners.min.x * (y + 1)),
                        std::next(atlas->texture.pixels_alpha8.begin(), (t.y * atlas->texture.size.x) + t.x + (atlas->texture.size.x * y)));
                }

                dst_font->add_glyph(&config, (std::uint16_t)glyph.glyph.codepoint, rect_t{ glyph.glyph.texture_coordinates.min + config.glyph_config.offset + vec2_t{ 0.f, round(dst_font->ascent) } } + rect_t{ vec2_t{ }, glyph.glyph.corners.min }, rect_t{ t, t + glyph.glyph.corners.min } / atlas->texture.size, glyph.glyph.advance_x);
            }
        }

        atlas->build_finish();

        std::ranges::for_each(src_array, [](build_src_t& src) {
            if(src.freetype.face) { FT_Done_Face(src.freetype.face); src.freetype.face = nullptr; }
            });

        if(FT_Done_Library(ft_library))
            throw std::runtime_error{ "FT_Done_Library != 0" };
	}
}