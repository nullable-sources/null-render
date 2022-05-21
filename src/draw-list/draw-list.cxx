module null.render;
import :draw_list;
import :font;

#define NORMALIZE2F_OVER_ZERO(VX,VY) do { float d2 = VX*VX + VY*VY; if (d2 > 0.0f) { float inv_len = 1.0f / sqrtf(d2); VX *= inv_len; VY *= inv_len; } } while (0)
#define FIXNORMAL2F(VX,VY) do { float d2 = VX*VX + VY*VY; if (d2 < 0.5f) d2 = 0.5f; float inv_lensq = 1.0f / d2; VX *= inv_lensq; VY *= inv_lensq; } while (0)

namespace null::render {
    void c_draw_list::draw_data_t::de_index_all_buffers() {
        vtx_buffer_t new_vtx_buffer{ };
        total_vtx_count = total_idx_count = 0;
        for(c_draw_list* cmd_list : cmd_lists) { ;
            if(cmd_list->idx_buffer.empty())
                continue;
            new_vtx_buffer.resize(cmd_list->idx_buffer.size());
            for(int j = 0; j < cmd_list->idx_buffer.size(); j++)
                new_vtx_buffer[j] = cmd_list->vtx_buffer[cmd_list->idx_buffer[j]];
            cmd_list->vtx_buffer.swap(new_vtx_buffer);
            cmd_list->idx_buffer.clear();
            total_vtx_count += cmd_list->vtx_buffer.size();
        }
    }

    void c_draw_list::draw_data_t::add_draw_list(c_draw_list* draw_list) {
        draw_list->pop_unused_draw_cmd();
        if(draw_list->cmd_buffer.empty()) return;

        if(draw_list->vtx_buffer.empty()) throw std::runtime_error("vtx_buffer empty");
        if(draw_list->idx_buffer.empty()) throw std::runtime_error("idx_buffer empty");

        layers.push_back(draw_list);
    }

    void c_draw_list::draw_data_t::setup() {
        valid = true;
        cmd_lists = layers;
        total_vtx_count = total_idx_count = 0;
        for(c_draw_list* draw_list : layers) {
            total_vtx_count += draw_list->vtx_buffer.size();
            total_idx_count += draw_list->idx_buffer.size();
        }
    }

    void c_draw_list::shade_verts_linear_uv(vtx_buffer_t::iterator vtx_start, vtx_buffer_t::iterator vtx_end, rect_t rect, rect_t uv, bool clamp) {
        const vec2_t scale = vec2_t{
            rect.size().x != 0.0f ? (uv.size().x / rect.size().x) : 0.0f,
            rect.size().y != 0.0f ? (uv.size().y / rect.size().y) : 0.0f };

        for(vtx_buffer_t::iterator vertex = vtx_start; vertex < vtx_end; ++vertex) {
            if(clamp) vertex->uv = std::clamp(uv.min + (vertex->pos - rect.min) * scale, std::min(uv.min, uv.max), std::max(uv.min, uv.max));
            else vertex->uv = uv.min + (vertex->pos - rect.min) * scale;
        }
    }
    
    void c_draw_list::on_changed_clip_rect() {
        if(cmd_buffer.back().element_count != 0 && cmd_buffer.back().clip_rect != cmd_header.clip_rect) {
            add_draw_cmd();
            return;
        }

        if(cmd_buffer.back().element_count == 0 && cmd_buffer.size() > 1 && cmd_header == *std::prev(cmd_buffer.end(), 2)) {
            cmd_buffer.pop_back();
            return;
        }

        cmd_buffer.back().clip_rect = cmd_header.clip_rect;
    }

    void c_draw_list::push_clip_rect(rect_t rect, bool intersect_with_current_rect) {
        if(intersect_with_current_rect) {
            rect = { math::max(rect.min, cmd_header.clip_rect.min), math::min(rect.max, cmd_header.clip_rect.max) };
        }

        rect.max = std::max(rect.min, rect.max);

        clip_rect_stack.push_back(rect);
        cmd_header.clip_rect = rect;
        on_changed_clip_rect();
    }

    void c_draw_list::pop_clip_rect() {
        clip_rect_stack.pop_back();
        cmd_header.clip_rect = (clip_rect_stack.size() == 0) ? shared_data->clip_rect_fullscreen : clip_rect_stack.data()[clip_rect_stack.size() - 1];
        on_changed_clip_rect();
    }

    void c_draw_list::on_changed_texture_id() {
        if(cmd_buffer.back().element_count != 0 && cmd_buffer.back().texture_id != cmd_header.texture_id) {
            add_draw_cmd();
            return;
        }

        if(cmd_buffer.back().element_count == 0 && cmd_buffer.size() > 1 && cmd_header == *std::prev(cmd_buffer.end(), 2)) {
            cmd_buffer.pop_back();
            return;
        }

        cmd_buffer.back().texture_id = cmd_header.texture_id;
    }

    void c_draw_list::push_texture_id(void* texture_id) {
        texture_id_stack.push_back(texture_id);
        cmd_header.texture_id = texture_id;
        on_changed_texture_id();
    }

    void c_draw_list::pop_texture_id() {
        texture_id_stack.pop_back();
        cmd_header.texture_id = (texture_id_stack.size() == 0) ? (void*)nullptr : texture_id_stack.data()[texture_id_stack.size() - 1];
        on_changed_texture_id();
    }


    void c_draw_list::on_changed_vtx_offset() {
        if(cmd_buffer.back().element_count) {
            add_draw_cmd();
            return;
        }

        cmd_buffer.back().vtx_offset = cmd_header.vtx_offset;
    }

    void c_draw_list::vtx_check(int vtx_count) {
        if(sizeof(std::uint16_t) == 2 && (vtx_buffer.size() + vtx_count >= (1 << 16)) && (flags & e_draw_list_flags::allow_vtx_offset)) {
            cmd_header.vtx_offset = vtx_buffer.size();
            on_changed_vtx_offset();
        }
    }

    void c_draw_list::prim_rect(vec2_t a, vec2_t c, color_t color) {
        prim_insert_idx({
            (std::uint16_t)vtx_buffer.size(), (std::uint16_t)(vtx_buffer.size() + 1), (std::uint16_t)(vtx_buffer.size() + 2),
            (std::uint16_t)vtx_buffer.size(), (std::uint16_t)(vtx_buffer.size() + 2), (std::uint16_t)(vtx_buffer.size() + 3)
            });
        
        prim_insert_vtx({
            { a, shared_data->font->container_atlas->texture.uv_white_pixel, color },
            { { c.x, a.y }, shared_data->font->container_atlas->texture.uv_white_pixel, color },
            { c, shared_data->font->container_atlas->texture.uv_white_pixel, color },
            { { a.x, c.y }, shared_data->font->container_atlas->texture.uv_white_pixel, color },
            });
    }

    void c_draw_list::prim_rect_uv(vec2_t a, vec2_t c, vec2_t uv_a, vec2_t uv_c, color_t color) {
        prim_insert_idx({
            (std::uint16_t)vtx_buffer.size(), (std::uint16_t)(vtx_buffer.size() + 1), (std::uint16_t)(vtx_buffer.size() + 2),
            (std::uint16_t)vtx_buffer.size(), (std::uint16_t)(vtx_buffer.size() + 2), (std::uint16_t)(vtx_buffer.size() + 3)
            });

        prim_insert_vtx({
            { a, uv_a, color },
            { { c.x, a.y }, { uv_c.x, uv_a.y }, color },
            { c, uv_c, color },
            { { a.x, c.y }, { uv_a.x, uv_c.y }, color }
            });
    }

    void c_draw_list::prim_quad_uv(std::array<vec2_t, 4> points, std::array<vec2_t, 4> uvs, color_t color) {
        prim_insert_idx({
            (std::uint16_t)vtx_buffer.size(), (std::uint16_t)(vtx_buffer.size() + 1), (std::uint16_t)(vtx_buffer.size() + 2),
            (std::uint16_t)vtx_buffer.size(), (std::uint16_t)(vtx_buffer.size() + 2), (std::uint16_t)(vtx_buffer.size() + 3)
            });

        std::transform(points.begin(), points.end(), uvs.begin(), points.begin(), [&](vec2_t& point, vec2_t& uv) { prim_add_vtx({ point, uv, color }); return point; });
    }
    
    void c_draw_list::path_rect(vec2_t a, vec2_t b, float rounding, e_corner_flags rounding_corners) {
        rounding = std::min(rounding, std::fabsf(b.x - a.x) * (rounding_corners & e_corner_flags::top || rounding_corners & e_corner_flags::bot ? 0.5f : 1.f) - 1.f);
        rounding = std::min(rounding, std::fabsf(b.y - a.y) * (rounding_corners & e_corner_flags::left || rounding_corners & e_corner_flags::right ? 0.5f : 1.f) - 1.f);

        if(rounding <= 0.0f || rounding_corners == e_corner_flags{ }) {
            path.push_back(a);
            path.push_back({ b.x, a.y });
            path.push_back(b);
            path.push_back({a.x, b.y });
        } else {
            float rounding_tl = rounding_corners & e_corner_flags::top_left ? rounding : 0.f;
            float rounding_tr = rounding_corners & e_corner_flags::top_right ? rounding : 0.f;
            float rounding_br = rounding_corners & e_corner_flags::bot_right ? rounding : 0.f;
            float rounding_bl = rounding_corners & e_corner_flags::bot_left ? rounding : 0.f;

            path_arc_to_fast(a + rounding_tl, rounding_tl, 6, 9);
            path_arc_to_fast(vec2_t{ b.x, a.y } + vec2_t{ -rounding_tr, rounding_tr }, rounding_tr, 9, 12);
            path_arc_to_fast(b - rounding_br, rounding_br, 0, 3);
            path_arc_to_fast(vec2_t{ a.x, b.y } + vec2_t{ rounding_bl, -rounding_bl}, rounding_bl, 3, 6);
        }
    }

    void c_draw_list::path_arc_to_fast(vec2_t center, float radius, int a_min_of_12, int a_max_of_12) {
        if(radius == 0.0f || a_min_of_12 > a_max_of_12) {
            path.push_back(center);
            return;
        }

        path.reserve(path.size() + (a_max_of_12 - a_min_of_12 + 1));
        for(int a = a_min_of_12; a <= a_max_of_12; a++) {
            const vec2_t& c = shared_data->arc_fast_vtx[a % shared_data->arc_fast_vtx.size()];
            path.push_back(vec2_t(center.x + c.x * radius, center.y + c.y * radius));
        }
    }
    
    void c_draw_list::draw_text(multicolor_text_t str, vec2_t pos, e_text_flags flags, c_font* font, float size) {
        font = font ? font : shared_data->font;
        size = size > 0.f ? size : font->size;

        if(font->container_atlas->texture.id != cmd_header.texture_id)
            throw std::runtime_error("font->container_atlas->texture.id != cmd_header.texture_id");

        std::string str_unite = str.unite();
        if(flags & e_text_flags::aligin_mask) {
            vec2_t str_size = font->calc_text_size(str_unite, size);
            if(str_size <= 0.f) return;

            if(flags & e_text_flags::aligin_right) pos.x -= str_size.x;
            if(flags & e_text_flags::aligin_bottom) pos.y -= str_size.y;
            if(flags & e_text_flags::aligin_centre_x) pos.x -= str_size.x / 2.f;
            if(flags & e_text_flags::aligin_centre_y) pos.y -= str_size.y / 2.f;
        }

        pos = { std::floorf(pos.x), std::floorf(pos.y) };
        if(pos.y > cmd_header.clip_rect.max.y) return;

        const float scale = size / font->size;
        const float line_height = font->size * scale;

        if(pos.y + line_height < cmd_header.clip_rect.min.y) {
            while(pos.y + line_height < cmd_header.clip_rect.min.y) {
                pos.y += line_height;

                multicolor_text_t::data_t::iterator finded = std::find_if(str.data.begin(), str.data.end(), [&str_unite](multicolor_text_t::data_t::value_type& multicolor_data) {
                    std::string::iterator new_line = std::find(multicolor_data.first.begin(), multicolor_data.first.end(), '\n');
                    if(new_line != multicolor_data.first.end()) {
                        multicolor_data.first.erase(multicolor_data.first.begin(), std::next(new_line));
                        str_unite.erase(str_unite.begin(), std::find(str_unite.begin(), str_unite.end(), '\n')); //rebuilding str_unite every time is not a good idea, so change it right here
                    }
                    return new_line != multicolor_data.first.end();
                    });

                if(finded != str.data.end()) str.data.erase(str.data.begin(), (*finded).first.empty() ? std::next(finded) : finded);
                else return; //if all the text is outside the clip_rect, we don't need to draw it
            }
        }

        //a more correct solution would be text_size > clip_rect.max.y
        //but calculating text_size each time would be too resource-intensive and unjustified
        if(str_unite.size() > 10000) {
            float y = pos.y;
            while(y < cmd_header.clip_rect.max.y) {
                y += line_height;
                multicolor_text_t::data_t::iterator finded = std::find_if(str.data.begin(), str.data.end(), [&str_unite](multicolor_text_t::data_t::value_type& multicolor_data) {
                    if(std::string::iterator new_line; (new_line = std::find(multicolor_data.first.begin(), multicolor_data.first.end(), '\n')) != multicolor_data.first.end()) {
                        multicolor_data.first.erase(new_line, multicolor_data.first.end());
                        return true;
                    }
                    return false;
                    });

                if(finded != str.data.end()) str.data.erase((*finded).first.empty() ? finded : std::next(finded), str.data.end());
                else return;
            }
        }

        int vtx_offset{ }; //offset for outline
        vec2_t draw_pos{ pos };
        for(multicolor_text_t::data_t::value_type multicolor_data : str.data) {
            for(std::string::iterator s = multicolor_data.first.begin(); s != multicolor_data.first.end();) {
                std::uint32_t c = *s;
                if(c < 0x80) s += 1;
                else {
                    s += impl::get_char_from_utf8(&c, std::string(s, multicolor_data.first.end()));
                    if(c == 0) break;
                }

                if(c < 32) {
                    if(c == '\n') {
                        draw_pos.x = pos.x;
                        draw_pos.y += line_height;
                        if(draw_pos.y > cmd_header.clip_rect.max.y)
                            break;
                        continue;
                    } if(c == '\r') continue;
                }

                const c_font::glyph_t* glyph = font->find_glyph((std::uint16_t)c);
                if(!glyph) continue;

                if(glyph->visible) {
                    rect_t corners{ rect_t{ draw_pos } + glyph->corners * scale };
                    if(corners.min.x <= cmd_header.clip_rect.max.x && corners.max.x >= cmd_header.clip_rect.min.x) {
                        rect_t uvs = glyph->texture_coordinates;

                        if(flags & e_text_flags::outline && !shared_data->text_outline_offsets.empty()) {
                            for(vec2_t offset : shared_data->text_outline_offsets) {
                                prim_insert_idx({
                                    (std::uint16_t)vtx_buffer.size(), (std::uint16_t)(vtx_buffer.size() + 1), (std::uint16_t)(vtx_buffer.size() + 2),
                                    (std::uint16_t)vtx_buffer.size(), (std::uint16_t)(vtx_buffer.size() + 2), (std::uint16_t)(vtx_buffer.size() + 3)
                                    });

                                rect_t pos = corners + offset;
                                prim_insert_vtx(vtx_buffer.end() - vtx_offset,
                                    {
                                        { pos.min,                  uvs.min,                    {0, 0, 0} },
                                        { { pos.max.x, pos.min.y }, { uvs.max.x, uvs.min.y },   {0, 0, 0} },
                                        { pos.max,                  uvs.max,                    {0, 0, 0} },
                                        { { pos.min.x, pos.max.y }, { uvs.min.x, uvs.max.y },   {0, 0, 0} }
                                    });
                            }
                        }
                        prim_rect_uv(corners.min, corners.max, uvs.min, uvs.max, multicolor_data.second); //main text

                        //Necessary for the correct drawing order of the outline and body text.
                        //Because the outline draw call runs parallel to the body text draw calls, glyphs of new letters can overlap past ones.
                        vtx_offset += 4;
                    }
                }
                draw_pos.x += glyph->advance_x * scale;
            }
        }
    }

    void c_draw_list::draw_rect_filled(vec2_t a, vec2_t b, color_t color, float rounding, e_corner_flags flags) {
        if(color.a() == 0.f) return;

        if(rounding > 0.0f) {
            path_rect(a, b, rounding, flags);
            path_fill_convex(color);
        } else prim_rect(a, b, color);
    }

    void c_draw_list::draw_convex_poly_filled(std::vector<vec2_t> points, color_t color) {
        if(points.size() < 3) return;

        if(flags & e_draw_list_flags::anti_aliased_fill) {
            std::size_t vtx_inner_idx = vtx_buffer.size();
            std::size_t vtx_outer_idx = vtx_buffer.size() + 1;
            for(int i = 2; i < points.size(); i++) prim_insert_idx({ (std::uint16_t)(vtx_inner_idx), (std::uint16_t)(vtx_inner_idx + ((i - 1) << 1)), (std::uint16_t)(vtx_inner_idx + (i << 1)) });

            std::vector<vec2_t> temp_normals(points.size());
            for(int i0 = points.size() - 1, i1 = 0; i1 < points.size(); i0 = i1++) {
                vec2_t p = points[i1] - points[i0];
                if(p.length() > 0.f) p *= 1.f / p.length();

                temp_normals[i0] = { p.x, -p.y };
            }

            for(int i0 = points.size() - 1, i1 = 0; i1 < points.size(); i0 = i1++) {
                vec2_t p = (temp_normals[i0] + temp_normals[i1]) / 2;
                p *= 1.f / std::min(std::powf(p.length(), 2), 0.5f);
                p *= 0.5f; //AA_SIZE

                prim_insert_idx({
                    (std::uint16_t)(vtx_inner_idx + (i1 << 1)), (std::uint16_t)(vtx_inner_idx + (i0 << 1)), (std::uint16_t)(vtx_outer_idx + (i0 << 1)),
                    (std::uint16_t)(vtx_outer_idx + (i0 << 1)), (std::uint16_t)(vtx_outer_idx + (i1 << 1)), (std::uint16_t)(vtx_inner_idx + (i1 << 1))
                    });
                
                prim_insert_vtx({
                    { points[i1] - p, shared_data->font->container_atlas->texture.uv_white_pixel, color },
                    { points[i1] + p, shared_data->font->container_atlas->texture.uv_white_pixel, color_t{ color, 0.f } }
                    });
            }
        } else {
            for(int i = 2; i < points.size(); i++) prim_insert_idx({ (std::uint16_t)(vtx_buffer.size()), (std::uint16_t)(vtx_buffer.size() + i - 1), (std::uint16_t)(vtx_buffer.size() + i) });
            for(vec2_t point : points) prim_add_vtx({ point, shared_data->font->container_atlas->texture.uv_white_pixel, color });
        }
    }
}