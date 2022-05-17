#define _HAS_ITERATOR_DEBUGGING 0
#define STB_TRUETYPE_IMPLEMENTATION
#define STB_RECT_PACK_IMPLEMENTATION
#define STBRP_STATIC
#define STBTT_STATIC

#include <stb/stb_rect_pack.h>
#include <stb/stb_truetype.h>
module null.render;
import :font;
import :draw_list;

namespace null::render {
    void c_font::c_atlas::texture_t::render_1bpp_rect_from_string(rect_t bpp_rect, const char* in_str, char in_marker_char, std::uint8_t in_marker_pixel_value) {
        if(bpp_rect.size() > size) throw std::runtime_error("bpp_rect.size() > size");
        if(bpp_rect.min < vec2_t{ 0.f }) throw std::runtime_error("bpp_rect.min < 0.f");
        
        std::uint8_t* out_pixel = pixels_alpha8 + (int)bpp_rect.min.x + (int)(bpp_rect.min.y * size.x);
        for(int off_y = 0; off_y < bpp_rect.max.y; off_y++, out_pixel += (int)size.x, in_str += (int)bpp_rect.max.x)
            for(int off_x = 0; off_x < bpp_rect.max.x; off_x++)
                out_pixel[off_x] = (in_str[off_x] == in_marker_char) ? in_marker_pixel_value : 0x00;
    }

    void c_font::c_atlas::texture_t::get_data_as_rgba32() {
        if(!pixels_alpha8) throw std::runtime_error("!pixels_alpha8");

        if(!pixels_rgba32) {
            pixels_rgba32 = (std::uint32_t*)malloc((std::size_t)size.x * (std::size_t)size.y * 4);
            const std::uint8_t* src = pixels_alpha8;
            std::uint32_t* dst = pixels_rgba32;
            for(int n = size.x * size.y; n > 0; n--)
                *dst++ = (std::uint32_t)color_t{ 255, 255, 255, (std::uint32_t)(*src++) };
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
        if(!texture.pixels_alpha8) throw std::runtime_error("!texture.pixels_alpha8");
        render_default_texture_data();
        render_lines_texture_data();

        for(custom_rect_t& r : custom_rects) {
            if(!r.font || r.glyph_id == 0)
                continue;

            if(r.font->container_atlas != this) throw std::runtime_error("r.font->container_atlas != this");
            rect_t uv = r.size * texture.uv_scale;
            r.font->add_glyph(nullptr, (std::uint16_t)r.glyph_id, rect_t{ r.glyph_offset, r.glyph_offset + r.size.max }, uv, r.glyph_advance_x);
        }

        for(c_font& font : fonts) {
            if(font.lookup_table.dirty)
                font.build_lookup_table();

            if(font.ellipsis_char != (std::uint16_t)-1)
                continue;

            std::vector<std::uint16_t> ellipsis_variants = { (std::uint16_t)0x2026, (std::uint16_t)0x0085 };
            for(std::uint16_t& variant : ellipsis_variants)
                if(font.find_glyph(variant, false)) {
                    font.ellipsis_char = variant;
                    break;
                }
        }
    }

    bool c_font::c_atlas::build_with_stb_truetype() {
        if(locked) throw std::runtime_error("cannot modify a locked atlas between begin_render() and end_render/render()!");
        if(configs.empty()) throw std::runtime_error("configs.empty()");

        build_initialize();

        texture.clear();
        texture = texture_t{ };

        std::vector<build_src_t> src_array(configs.size());
        std::vector<build_dst_t> dst_array(fonts.size());

        std::transform(src_array.begin(), src_array.end(), configs.begin(), src_array.begin(), [&](build_src_t src, config_t& config) {
            if(!config.font) throw std::runtime_error("!cfg.font");
            if(config.font->is_loaded() && config.font->container_atlas != this) throw std::runtime_error("cfg.font->is_loaded() && cfg.font->container_atlas != this");

            src.dst_index = -1;
            for(int output_i = 0; output_i < fonts.size() && src.dst_index == -1; output_i++) {
                if(config.font == &fonts[output_i])
                    src.dst_index = output_i;
            }

            if(src.dst_index == -1)
                throw std::runtime_error("src.dst_index == -1");

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

        int total_glyphs_count = 0;
        for(build_src_t& src : src_array) {
            build_dst_t& dst = dst_array[src.dst_index];
            src.glyphs_set.resize(((src.glyphs_highest + 1) + 31) >> 5, 0);
            if(dst.glyphs_set.empty()) dst.glyphs_set.resize(((dst.glyphs_highest + 1) + 31) >> 5, 0);

            std::uint32_t codepoint;
            for(const std::uint16_t* src_range = src.src_ranges; src_range[0] && src_range[1]; src_range += 2) {
                for(std::uint32_t codepoint = src_range[0]; codepoint <= src_range[1]; codepoint++) {
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
            src.glyphs_list.reserve(src.glyphs_count);

            for(std::vector<std::uint32_t>::iterator it = src.glyphs_set.begin(); it < src.glyphs_set.end(); it++) {
                if(*it) {
                    for(std::uint32_t bit_n = 0; bit_n < 32; bit_n++) {
                        if(*it & ((std::uint32_t)1 << bit_n)) {
                            if(src.glyphs_list.size() <= 0) src.glyphs_list = std::vector<int>{ (int)(((it - src.glyphs_set.begin()) << 5) + bit_n) };
                            else src.glyphs_list.push_back((int)(((it - src.glyphs_set.begin()) << 5) + bit_n));
                        }
                    }
                }
            }

            src.glyphs_set.clear();
            if(src.glyphs_list.size() != src.glyphs_count) throw std::runtime_error("src.glyphs_list.size() != src.glyphs_count");
        }

        std::vector<stbrp_rect> buf_rects(total_glyphs_count);
        std::vector<stbtt_packedchar> buf_packedchars(total_glyphs_count);

        int total_surface = 0, buf_rects_out_n = 0, buf_packedchars_out_n = 0;
        std::transform(src_array.begin(), src_array.end(), configs.begin(), src_array.begin(), [&](build_src_t src, config_t config) {
            if(!src.glyphs_count)
                return src;

            src.rects = &buf_rects[buf_rects_out_n];
            src.packed_chars = &buf_packedchars[buf_packedchars_out_n];
            buf_rects_out_n += src.glyphs_count;
            buf_packedchars_out_n += src.glyphs_count;

            src.pack_range.font_size = config.size_pixels;
            src.pack_range.first_unicode_codepoint_in_range = 0;
            src.pack_range.array_of_unicode_codepoints = src.glyphs_list.data();
            src.pack_range.num_chars = src.glyphs_list.size();
            src.pack_range.chardata_for_range = src.packed_chars;
            src.pack_range.h_oversample = (std::uint8_t)config.oversample.x;
            src.pack_range.v_oversample = (std::uint8_t)config.oversample.y;

            const float scale = (config.size_pixels > 0) ? stbtt_ScaleForPixelHeight(&src.font_info, config.size_pixels) : stbtt_ScaleForMappingEmToPixels(&src.font_info, -config.size_pixels);
            const int padding = texture.glyph_padding;
            for(int i = 0; i < src.glyphs_list.size(); i++) {
                int x0, y0, x1, y1;
                const int glyph_index_in_font = stbtt_FindGlyphIndex(&src.font_info, src.glyphs_list[i]);
                if(!glyph_index_in_font) throw std::runtime_error("!glyph_index_in_font");
                stbtt_GetGlyphBitmapBoxSubpixel(&src.font_info, glyph_index_in_font, scale * config.oversample.x, scale * config.oversample.y, 0, 0, &x0, &y0, &x1, &y1);
                src.rects[i].w = (stbrp_coord)(x1 - x0 + padding + config.oversample.x - 1);
                src.rects[i].h = (stbrp_coord)(y1 - y0 + padding + config.oversample.y - 1);
                total_surface += src.rects[i].w * src.rects[i].h;
            }

            return src;
            });

        const int surface_sqrt = (int)sqrtf((float)total_surface) + 1;
        texture.size.y = 0;
        if(texture.desired_width > 0) texture.size.x = texture.desired_width;
        else texture.size.x = (surface_sqrt >= 4096 * 0.7f) ? 4096 : (surface_sqrt >= 2048 * 0.7f) ? 2048 : (surface_sqrt >= 1024 * 0.7f) ? 1024 : 512;

        const int TEX_HEIGHT_MAX = 1024 * 32;
        stbtt_pack_context spc{ };
        stbtt_PackBegin(&spc, nullptr, texture.size.x, TEX_HEIGHT_MAX, 0, texture.glyph_padding, nullptr);
        pack_custom_rects(spc.pack_info);

        for(build_src_t& src : src_array) {
            if(src.glyphs_count == 0)
                continue;

            stbrp_pack_rects((stbrp_context*)spc.pack_info, src.rects, src.glyphs_count);

            for(int i = 0; i < src.glyphs_count; i++) {
                if(src.rects[i].was_packed)
                    texture.size.y = std::max((int)texture.size.y, src.rects[i].y + src.rects[i].h);
            }
        }

        texture.size.y = std::powf(2.f, std::ceilf(std::logf(texture.size.y) / std::logf(2.f)));
        texture.uv_scale = vec2_t{ 1.f } / texture.size;
        texture.pixels_alpha8 = (std::uint8_t*)malloc(texture.size.x * texture.size.y);
        memset(texture.pixels_alpha8, 0, texture.size.x * texture.size.y);
        spc.pixels = texture.pixels_alpha8;
        spc.height = texture.size.y;

        std::transform(src_array.begin(), src_array.end(), configs.begin(), src_array.begin(), [&](build_src_t src, config_t config) {
            if(src.glyphs_count == 0)
                return src;

            stbtt_PackFontRangesRenderIntoRects(&spc, &src.font_info, &src.pack_range, 1, src.rects);

            if(config.rasterizer_multiply != 1.0f) {
                std::array<std::uint8_t, 256> multiply_table;
                multiply_calc_lookup_table(multiply_table, config.rasterizer_multiply);
                stbrp_rect* r = &src.rects[0];
                for(int glyph_i = 0; glyph_i < src.glyphs_count; glyph_i++, r++) {
                    if(r->was_packed)
                        multiply_rect_alpha8(multiply_table, texture.pixels_alpha8, rect_t{ r->x, r->y, r->w, r->h }, texture.size.x);
                }
            }
            src.rects = nullptr;

            return src;
            });

        stbtt_PackEnd(&spc);

        std::transform(src_array.begin(), src_array.end(), configs.begin(), src_array.begin(), [&](build_src_t src, config_t& config) {
            if(src.glyphs_count == 0)
                return src;

            c_font* dst_font = config.font;

            const float font_scale = stbtt_ScaleForPixelHeight(&src.font_info, config.size_pixels);
            int unscaled_ascent, unscaled_descent, unscaled_line_gap;
            stbtt_GetFontVMetrics(&src.font_info, &unscaled_ascent, &unscaled_descent, &unscaled_line_gap);

            const float ascent = std::floorf(unscaled_ascent * font_scale + ((unscaled_ascent > 0.0f) ? +1 : -1));
            const float descent = std::floorf(unscaled_descent * font_scale + ((unscaled_descent > 0.0f) ? +1 : -1));
            setup_font(dst_font, &config, ascent, descent);
            const float font_off_x = config.glyph_config.offset.x;
            const float font_off_y = config.glyph_config.offset.y + round(dst_font->ascent);

            for(int glyph_i = 0; glyph_i < src.glyphs_count; glyph_i++) {
                const int codepoint = src.glyphs_list[glyph_i];
                const stbtt_packedchar& pc = src.packed_chars[glyph_i];
                stbtt_aligned_quad q;
                float unused_x = 0.0f, unused_y = 0.0f;
                stbtt_GetPackedQuad(src.packed_chars, texture.size.x, texture.size.y, glyph_i, &unused_x, &unused_y, &q, 0);
                dst_font->add_glyph(&config, (std::uint16_t)codepoint, rect_t{ q.x0 + font_off_x, q.y0 + font_off_y, q.x1 + font_off_x, q.y1 + font_off_y }, rect_t{ q.s0, q.t0, q.s1, q.t1 }, pc.xadvance);
            }

            return src;
            });

        build_finish();
        return true;
    }
    
    void c_font::c_atlas::render_lines_texture_data() {
        custom_rect_t* r = &custom_rects[pack_id_lines];
        if(!r->is_packed()) throw std::runtime_error("!r->is_packed()");

        for(std::uint32_t n = 0; n < 64; n++) {
            std::uint32_t y = n;
            std::uint32_t line_width = n;
            std::uint32_t pad_left = (r->size.max.x - line_width) / 2;
            std::uint32_t pad_right = r->size.max.x - (pad_left + line_width);

            if(pad_left + line_width + pad_right != r->size.max.x) throw std::runtime_error("pad_left + line_width + pad_right != r->size.max.x");
            if(y >= r->size.max.y) throw std::runtime_error("y >= r->size.max.y");
            
            std::uint8_t* write_ptr = &texture.pixels_alpha8[int(r->size.min.x + ((r->size.min.y + y) * texture.size.x))];
            memset(write_ptr, 0x00, pad_left);
            memset(write_ptr + pad_left, 0xFF, line_width);
            memset(write_ptr + pad_left + line_width, 0x00, pad_right);

            vec2_t uv0 = vec2_t{ (float)(r->size.min.x + pad_left - 1), (float)(r->size.min.y + y) } * texture.uv_scale;
            vec2_t uv1 = vec2_t{ (float)(r->size.min.x + pad_left + line_width + 1), (float)(r->size.min.y + y + 1) } * texture.uv_scale;
            float half_v = (uv0.y + uv1.y) * 0.5f;
            texture.uv_lines[n] = rect_t{ uv0.x, half_v, uv1.x, half_v };
        }
    }

    void c_font::c_atlas::render_default_texture_data() {
        custom_rect_t* r = &custom_rects[pack_id_cursors];
        if(!r->is_packed()) throw std::runtime_error("!r->is_packed()");

        const int w = texture.size.x;
        if(r->size.max.x != 2) throw std::runtime_error("r->size.max.x != 2");
        if(r->size.max.y != 2) throw std::runtime_error("r->size.max.y != 2");

        const int offset = (int)r->size.min.x + (int)r->size.min.y * w;
        texture.pixels_alpha8[offset] = texture.pixels_alpha8[offset + 1] = texture.pixels_alpha8[offset + w] = texture.pixels_alpha8[offset + w + 1] = 0xFF;

        texture.uv_white_pixel = (r->size.min + vec2_t{ 0.5f, 0.5f }) * texture.uv_scale;
    }

    void c_font::c_atlas::pack_custom_rects(void* stbrp_context_opaque) {
        stbrp_context* pack_context = (stbrp_context*)stbrp_context_opaque;
        if(pack_context == nullptr) throw std::runtime_error("pack_context == nullptr");

        std::vector<custom_rect_t>& user_rects = custom_rects;
        if(user_rects.size() < 1) throw std::runtime_error("user_rects.size() < 1");

        std::vector<stbrp_rect> pack_rects{ user_rects.size() };
        for(int i = 0; i < user_rects.size(); i++) {
            pack_rects[i].w = user_rects[i].size.max.x;
            pack_rects[i].h = user_rects[i].size.max.y;
        }

        stbrp_pack_rects(pack_context, &pack_rects[0], pack_rects.size());
        for(int i = 0; i < pack_rects.size(); i++) {
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
        for(std::uint32_t i = 0; i < 256; i++) {
            std::uint32_t value = (std::uint32_t)(i * in_multiply_factor);
            out_table[i] = value > 255 ? 255 : (value & 0xFF);
        }
    }

    void c_font::c_atlas::multiply_rect_alpha8(std::array<std::uint8_t, 256> table, std::uint8_t* pixels, rect_t size, int stride) {
        std::uint8_t* data = pixels + (int)size.min.x + (int)size.min.y * stride;
        for(int j = size.max.x; j > 0; j--, data += stride)
            for(int i = 0; i < size.max.y; i++)
                data[i] = table[data[i]];
    }

    c_font* c_font::c_atlas::add_font(config_t* config) {
        if(locked) throw std::runtime_error("cannot modify a locked atlas between begin_render() and end_render/render()!");
        if(config->font) throw std::runtime_error("config->font == nullptr");
        if(config->data.empty()) throw std::runtime_error("cofig->data.empty()");
        if(config->size_pixels <= 0.f) throw std::runtime_error("config->size_pixels <= 0.f");

        if(!config->merge_mode) fonts.push_back(c_font{ });
        else if(fonts.empty()) throw std::runtime_error("cannot use merge_mode for the first font");

        configs.push_back(*config);
        config_t& cfg = configs.back();
        
        if(!cfg.font) cfg.font = &fonts.back();
        if(!cfg.owned_by_atlas) {
            //new_font_cfg.data = config->data;
            cfg.owned_by_atlas = true;
        }

        if(cfg.font->ellipsis_char == (std::uint16_t)-1)
            cfg.font->ellipsis_char = config->ellipsis_char;

        texture.clear();
        return cfg.font;
    }

    c_font* c_font::c_atlas::add_font_default(config_t* config) {
        config_t cfg = config ? *config : config_t();
        if(!config) {
            cfg.oversample = vec2_t{ 1 };
            cfg.pixel_snap_h = true;
        }
        if(cfg.size_pixels <= 0.0f) cfg.size_pixels = 13.0f * 1.0f;
        cfg.ellipsis_char = (std::uint16_t)0x0085;
        cfg.glyph_config.offset.y = 1.0f * floor(cfg.size_pixels / 13.0f);

        const std::uint16_t* glyph_ranges = cfg.glyph_config.ranges ? cfg.glyph_config.ranges : glyph_t::ranges_default();
        c_font* font = add_font_from_memory_compressed_base_85_ttf(compressed_fonts::proggy_clean, cfg.size_pixels, &cfg, glyph_ranges);
        return font;
    }

    c_font* c_font::c_atlas::add_font_from_file_ttf(const char* filename, float size_pixels, config_t* config, const std::uint16_t* glyph_ranges) {
        if(locked) throw std::runtime_error("cannot modify a locked atlas between begin_render() and end_render/render()!");
        
        std::ifstream file(filename, std::ios::in | std::ios::binary | std::ios::ate);
        if(!file.is_open()) throw std::runtime_error("cannot open font file");
        
        std::vector<char> font_file( (std::size_t)file.tellg() );
        file.seekg(0, std::ios::beg);
        file.read(font_file.data(), font_file.size());
        file.close();

        return add_font_from_memory_ttf(font_file, size_pixels, config, glyph_ranges);
    }

    c_font* c_font::c_atlas::add_font_from_memory_ttf(std::vector<char> font_file, float size_pixels, config_t* config, const std::uint16_t* glyph_ranges) {
        if(locked) throw std::runtime_error("cannot modify a locked atlas between begin_render() and end_render/render()!");

        config_t cfg = config ? *config : config_t();
        if(!cfg.data.empty()) throw std::runtime_error("!cfg.data.empty()");
        
        cfg.data = font_file;
        cfg.size_pixels = size_pixels;
        if(glyph_ranges) cfg.glyph_config.ranges = glyph_ranges;
        
        return add_font(&cfg);
    }

    c_font* c_font::c_atlas::add_font_from_memory_compressed_ttf(std::vector<char> compressed_ttf, float size_pixels, config_t* config, const std::uint16_t* glyph_ranges) {
        std::vector<char> buf_decompressed_data(impl::stb::decompress_length((std::uint8_t*)compressed_ttf.data()));
        impl::stb::decompress((std::uint8_t*)buf_decompressed_data.data(), (const std::uint8_t*)compressed_ttf.data(), (std::uint32_t)compressed_ttf.size());

        config_t cfg = config ? *config : config_t();
        if(!cfg.data.empty()) throw std::runtime_error("!cfg.data.empty()");
        cfg.owned_by_atlas = true;

        return add_font_from_memory_ttf(buf_decompressed_data, size_pixels, &cfg, glyph_ranges);
    }

    c_font* c_font::c_atlas::add_font_from_memory_compressed_base_85_ttf(const char* compressed_font_data_base85, float size_pixels, config_t* config, const std::uint16_t* glyph_ranges) {
        std::vector<char> compressed_data((((int)strlen(compressed_font_data_base85) + 4) / 5) * 4);
        impl::decode85((const std::uint8_t*)compressed_font_data_base85, (std::uint8_t*)compressed_data.data());
        c_font* font = add_font_from_memory_compressed_ttf(compressed_data, size_pixels, config, glyph_ranges);
        return font;
    }


    void c_font::c_atlas::clear_input_data() {
        if(locked) throw std::runtime_error("cannot modify a locked atlas between begin_render() and end_render/render()!");
        for(config_t& config : configs) {
            if(config.owned_by_atlas) config.font = nullptr;
        }

        for(c_font& font : fonts) {
            if(font.config >= &configs.front() && font.config <= &configs.back()) {
                font.config = nullptr; font.config_count = 0;
            }
        }

        configs.clear();
        custom_rects.clear();
        pack_id_cursors = pack_id_lines = -1;
    }

    void c_font::c_atlas::clear_fonts() {
        for(c_font& font : fonts) {
            font.~c_font();
        } fonts.clear();
    }

    void c_font::build_lookup_table() {
        int max_codepoint = 0;
        for(glyph_t glyph : glyphs)
            max_codepoint = std::max(max_codepoint, (int)glyph.codepoint);

        if(glyphs.size() >= 0xFFFF) throw std::runtime_error("glyphs.size() >= 0xFFFF");
        lookup_table.advances_x.clear();
        lookup_table.indexes.clear();
        lookup_table.dirty = false;
        used_4k_pages_map.fill(0);
        lookup_table.resize(max_codepoint + 1);

        for(int i = 0; i < glyphs.size(); i++) {
            int codepoint = (int)glyphs[i].codepoint;
            lookup_table.advances_x[codepoint] = glyphs[i].advance_x;
            lookup_table.indexes[codepoint] = (std::uint16_t)i;

            const int page_n = codepoint / 4096;
            used_4k_pages_map[page_n >> 3] |= 1 << (page_n & 7);
        }

        if(find_glyph((std::uint16_t)' ')) {
            if(glyphs.back().codepoint != '\t')
                glyphs.resize(glyphs.size() + 1);
            glyph_t& tab_glyph = glyphs.back();
            tab_glyph = *find_glyph((std::uint16_t)' ');
            tab_glyph.codepoint = '\t';
            tab_glyph.advance_x *= 4;
            lookup_table.advances_x[(int)tab_glyph.codepoint] = (float)tab_glyph.advance_x;
            lookup_table.indexes[(int)tab_glyph.codepoint] = (std::uint16_t)(glyphs.size() - 1);
        }

        set_glyph_visible((std::uint16_t)' ', false);
        set_glyph_visible((std::uint16_t)'\t', false);

        fallback_glyph = find_glyph(fallback_char, false);
        fallback_advance_x = fallback_glyph ? fallback_glyph->advance_x : 0.0f;
        for(int i = 0; i < max_codepoint + 1; i++)
            if(lookup_table.advances_x[i] < 0.0f)
                lookup_table.advances_x[i] = fallback_advance_x;
    }

    c_font::glyph_t* c_font::find_glyph(std::uint16_t c, bool fallback) {
        if(c >= (size_t)lookup_table.indexes.size()) return fallback ? fallback_glyph : nullptr;
        const std::uint16_t i = lookup_table.indexes.data()[c];
        if(i == (std::uint16_t)-1) return fallback ? fallback_glyph : nullptr;
        return &glyphs.data()[i];
    }

    void c_font::add_glyph(config_t* cfg, std::uint16_t codepoint, rect_t corners, rect_t texture_coordinates, float advance_x) {
        if(cfg) {
            const float advance_x_original = advance_x;
            advance_x = std::clamp(advance_x, cfg->glyph_config.min_advance_x, cfg->glyph_config.max_advance_x);
            if(advance_x != advance_x_original) {
                corners += vec2_t{ cfg->pixel_snap_h ? floor((advance_x - advance_x_original) * 0.5f) : (advance_x - advance_x_original) * 0.5f, 0.f };
            }

            if(cfg->pixel_snap_h) advance_x = round(advance_x);
            advance_x += cfg->glyph_config.extra_spacing.x;
        }

        static std::vector<glyph_t> aye{ };

        glyphs.push_back({ codepoint, corners.min != corners.max, advance_x, corners, texture_coordinates });
        lookup_table.dirty = true;
    }

    void c_font::add_remap_char(std::uint16_t dst, std::uint16_t src, bool overwrite_dst) {
        if(lookup_table.indexes.empty()) throw std::runtime_error("lookup_table.indexes.empty()");
        std::uint32_t index_size = (std::uint32_t)lookup_table.indexes.size();

        if(dst < index_size && lookup_table.indexes[dst] == (std::uint16_t)-1 && !overwrite_dst) return;
        if(src >= index_size && dst >= index_size) return;

        lookup_table.resize(dst + 1);
        lookup_table.indexes[dst] = (src < index_size) ? lookup_table.indexes[src] : (std::uint16_t)-1;
        lookup_table.advances_x[dst] = (src < index_size) ? lookup_table.advances_x[src] : 1.0f;
    }

    bool c_font::is_glyph_range_unused(std::uint32_t c_begin, std::uint32_t c_last) {
        std::uint32_t page_begin = (c_begin / 4096);
        std::uint32_t page_last = (c_last / 4096);
        for(std::uint32_t page_n = page_begin; page_n <= page_last; page_n++)
            if((page_n >> 3) < sizeof(used_4k_pages_map))
                if(used_4k_pages_map[page_n >> 3] & (1 << (page_n & 7)))
                    return false;
        return true;
    }

    vec2_t c_font::calc_text_size(std::string str, float custom_size) {
        custom_size = custom_size < 0.f ? size : custom_size;

        vec2_t result{ };
        vec2_t line_size{ 0, custom_size };

        for(std::string::iterator s = str.begin(); s < str.end();) {
            std::uint32_t c = *s;
            if(c < 0x80) {
                s += 1;
            } else {
                s += impl::get_char_from_utf8(&c, std::string(s, str.end()));
                if(c == 0) break;
            }

            if(c < 32) {
                if(c == '\n') {
                    result.x = std::max(result.x, line_size.x);
                    result.y += line_size.y;
                    line_size.x = 0;
                    continue;
                } if(c == '\r') continue;
            }

            line_size.x += (c < lookup_table.advances_x.size() ? lookup_table.advances_x[c] : fallback_advance_x) * scale;
        }

        result.x = std::max(result.x, line_size.x);
        if(line_size.x > 0 || result.y == 0) result.y += line_size.y;

        return result;
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