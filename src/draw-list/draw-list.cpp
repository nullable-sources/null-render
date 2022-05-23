#include <draw-list/draw-list.h>

namespace null::render {
    void c_draw_list::draw_data_t::deindex_all_buffers() {
        vtx_buffer_t new_vtx_buffer{ };
        total_vtx_count = total_idx_count = 0;
        for(c_draw_list* cmd_list : cmd_lists) { ;
            if(cmd_list->idx_buffer.empty()) continue;

            new_vtx_buffer.resize(cmd_list->idx_buffer.size());
            for(int i = 0; i < cmd_list->idx_buffer.size(); i++)
                new_vtx_buffer[i] = cmd_list->vtx_buffer[cmd_list->idx_buffer[i]];
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
        cmd_header.clip_rect = (clip_rect_stack.size() == 0) ? parent_shared_data->clip_rect_fullscreen : clip_rect_stack.data()[clip_rect_stack.size() - 1];
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
        if(sizeof(std::uint16_t) == 2 && (vtx_buffer.size() + vtx_count >= (1 << 16)) && (parent_shared_data->initialize_flags & e_draw_list_flags::allow_vtx_offset)) {
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
            { a, parent_shared_data->font->container_atlas->texture.uv_white_pixel, color },
            { { c.x, a.y }, parent_shared_data->font->container_atlas->texture.uv_white_pixel, color },
            { c, parent_shared_data->font->container_atlas->texture.uv_white_pixel, color },
            { { a.x, c.y }, parent_shared_data->font->container_atlas->texture.uv_white_pixel, color },
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
    
    void c_draw_list::path_rect(vec2_t a, vec2_t b, float rounding, e_corner_flags flags) {
        rounding = std::min(rounding, std::fabsf(b.x - a.x) * (flags & e_corner_flags::top || flags & e_corner_flags::bot ? 0.5f : 1.f) - 1.f);
        rounding = std::min(rounding, std::fabsf(b.y - a.y) * (flags & e_corner_flags::left || flags & e_corner_flags::right ? 0.5f : 1.f) - 1.f);

        if(rounding <= 0.0f || flags == e_corner_flags{ }) {
            path.push_back(a);
            path.push_back({ b.x, a.y });
            path.push_back(b);
            path.push_back({a.x, b.y });
        } else {
            float rounding_tl = flags & e_corner_flags::top_left ? rounding : 0.f;
            float rounding_tr = flags & e_corner_flags::top_right ? rounding : 0.f;
            float rounding_br = flags & e_corner_flags::bot_right ? rounding : 0.f;
            float rounding_bl = flags & e_corner_flags::bot_left ? rounding : 0.f;

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

        a_min_of_12 *= shared_data_t::arc_fast_tessellation_multiplier;
        a_max_of_12 *= shared_data_t::arc_fast_tessellation_multiplier;

        for(int a = a_min_of_12; a <= a_max_of_12; a++) {
            const vec2_t& c = parent_shared_data->arc_fast_vtx[a % parent_shared_data->arc_fast_vtx.size()];
            path.push_back(center + c * radius);
        }
    }

    void c_draw_list::path_arc_to(vec2_t center, float radius, float a_min, float a_max, int num_segments) {
        if(radius == 0.0f) {
            path.push_back(center);
            return;
        }

        for(int i = 0; i <= num_segments; i++) {
            const float a = a_min + ((float)i / (float)num_segments) * (a_max - a_min);
            path.push_back(center + vec2_t{ cosf(a), sinf(a) } * radius);
        }
    }
    
    void c_draw_list::draw_text(multicolor_text_t str, vec2_t pos, e_text_flags flags, c_font* font, float size) {
        font = font ? font : parent_shared_data->font;
        size = size > 0.f ? size : font->size;

        if(font->container_atlas->texture.id != cmd_header.texture_id)
            throw std::runtime_error("font->container_atlas->texture.id != cmd_header.texture_id");

        std::string str_unite = str.unite();
        if(flags & e_text_flags::aligin_mask) {
            vec2_t str_size = font->calc_text_size(str_unite, size);
            if(str_size <= 0.f) return;

            if(flags & e_text_flags::aligin_right) pos.x -= str_size.x;
            if(flags & e_text_flags::aligin_bottom) pos.y -= str_size.y;
            if(flags & e_text_flags::aligin_center_x) pos.x -= str_size.x / 2.f;
            if(flags & e_text_flags::aligin_center_y) pos.y -= str_size.y / 2.f;
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

                        if(flags & e_text_flags::outline && !parent_shared_data->text_outline_offsets.empty()) {
                            for(vec2_t offset : parent_shared_data->text_outline_offsets) {
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

                        //necessary for the correct drawing order of the outline and body text.
                        //because the outline draw call runs parallel to the body text draw calls, glyphs of new letters can overlap past ones.
                        vtx_offset += 4;
                    }
                }
                draw_pos.x += glyph->advance_x * scale;
            }
        }
    }

    void c_draw_list::draw_rect(vec2_t a, vec2_t b, color_t color, float thickness, float rounding, e_corner_flags flags) {
        if(color.a() <= 0.f) return;

        path_rect(a + 0.50f, b - (parent_shared_data->initialize_flags & e_draw_list_flags::anti_aliased_lines ? 0.50f : 0.49f), rounding, flags);
        path_stroke(color, true, thickness);
    }

    void c_draw_list::draw_rect_filled(vec2_t a, vec2_t b, color_t color, float rounding, e_corner_flags flags) {
        if(color.a() <= 0.f) return;

        if(rounding > 0.0f) {
            path_rect(a, b, rounding, flags);
            path_fill_convex(color);
        } else prim_rect(a, b, color);
    }

    void c_draw_list::draw_convex_poly_filled(std::vector<vec2_t> points, color_t color) {
        if(points.size() < 3 || color <= 0.f) return;

        if(parent_shared_data->initialize_flags & e_draw_list_flags::anti_aliased_fill) {
            for(int i = 2; i < points.size(); i++) prim_insert_idx({ (std::uint16_t)(vtx_buffer.size()), (std::uint16_t)(vtx_buffer.size() + ((i - 1) << 1)), (std::uint16_t)(vtx_buffer.size() + (i << 1)) });

            std::vector<vec2_t> temp_normals(points.size());
            for(int i0 = points.size() - 1, i1 = 0; i1 < points.size(); i0 = i1++) {
                vec2_t p = points[i1] - points[i0];
                if(p.length() > 0.f) p *= 1.f / p.length();

                temp_normals[i0] = { p.y, -p.x };
            }

            std::size_t idx = vtx_buffer.size();
            for(int i0 = points.size() - 1, i1 = 0; i1 < points.size(); i0 = i1++) {
                vec2_t p = (temp_normals[i0] + temp_normals[i1]) / 2;
                p *= 1.f / std::max(std::powf(p.length(), 2), 0.5f);
                p *= 0.5f; //aa_size

                prim_insert_idx({
                    (std::uint16_t)(idx + (i1 << 1)),       (std::uint16_t)(idx + (i0 << 1)),       (std::uint16_t)(idx + 1 + (i0 << 1)),
                    (std::uint16_t)(idx + 1 + (i0 << 1)),   (std::uint16_t)(idx + 1 + (i1 << 1)),   (std::uint16_t)(idx + (i1 << 1))
                    });
                
                prim_insert_vtx({
                    { points[i1] - p, parent_shared_data->font->container_atlas->texture.uv_white_pixel, color },
                    { points[i1] + p, parent_shared_data->font->container_atlas->texture.uv_white_pixel, color_t{ color, 0.f } }
                    });
            }
        } else {
            for(int i = 2; i < points.size(); i++) prim_insert_idx({ (std::uint16_t)(vtx_buffer.size()), (std::uint16_t)(vtx_buffer.size() + i - 1), (std::uint16_t)(vtx_buffer.size() + i) });
            for(vec2_t point : points) prim_add_vtx({ point, parent_shared_data->font->container_atlas->texture.uv_white_pixel, color });
        }
    }

    void c_draw_list::draw_poly_line(std::vector<vec2_t> points, color_t color, bool closed, float thickness) {
        if(points.size() < 2 || color.a() <= 0.f) return;

        const int count = closed ? points.size() : points.size() - 1;
        const bool thick_line = (thickness > 1.0f);

        if(parent_shared_data->initialize_flags & e_draw_list_flags::anti_aliased_lines) {
            static const float aa_size = 1.0f;

            thickness = std::max(thickness, 1.0f);
            const int integer_thickness = (int)thickness;
            const float fractional_thickness = thickness - integer_thickness;

            const bool use_texture = (parent_shared_data->initialize_flags & e_draw_list_flags::anti_aliased_lines_use_texture) && (integer_thickness < 63) && (fractional_thickness <= 0.00001f);

            std::vector<vec2_t> temp_normals(points.size() * ((use_texture || !thick_line) ? 3 : 5));
            std::vector<vec2_t> temp_points(temp_normals.size() + points.size());

            for(int i1 = 0; i1 < count; i1++) {
                vec2_t p = points[(i1 + 1) == points.size() ? 0 : i1 + 1] - points[i1];
                if(p.length() > 0.f) p *= 1.f / p.length();

                temp_normals[i1] = { p.y, -p.x };
            }

            if(!closed) *std::prev(temp_normals.end()) = *std::prev(temp_normals.end(), 2);

            if(use_texture || !thick_line) {
                const float half_draw_size = use_texture ? ((thickness * 0.5f) + 1) : aa_size;
                if(!closed) {
                    temp_points[0] = points.front() + temp_normals.front() * half_draw_size;
                    temp_points[1] = points.front() - temp_normals.front() * half_draw_size;
                    temp_points[(points.size() - 1) * 2]        = points.back() + temp_normals[points.size() - 1] * half_draw_size;
                    temp_points[(points.size() - 1) * 2 + 1]    = points.back() - temp_normals[points.size() - 1] * half_draw_size;
                }

                unsigned int idx = vtx_buffer.size();
                for(int i1 = 0; i1 < count; i1++) {
                    const int i2 = (i1 + 1) == points.size() ? 0 : i1 + 1;
                    const unsigned int _idx = ((i1 + 1) == points.size()) ? vtx_buffer.size() : (idx + (use_texture ? 2 : 3));

                    vec2_t p = (temp_normals[i1] + temp_normals[i2]) / 2;
                    p *= 1.f / std::max(std::powf(p.length(), 2), 0.5f);
                    p *= half_draw_size;

                    temp_points[i2 * 2] = points[i2] + p;
                    temp_points[i2 * 2 + 1] = points[i2] - p;

                    if(use_texture) {
                        prim_insert_idx({
                            (std::uint16_t)(_idx),      (std::uint16_t)(idx),       (std::uint16_t)(idx + 1),
                            (std::uint16_t)(_idx + 1),  (std::uint16_t)(idx + 1),   (std::uint16_t)(_idx)
                            });
                    } else {
                        prim_insert_idx({
                            (std::uint16_t)(_idx),      (std::uint16_t)(idx),       (std::uint16_t)(idx + 2),
                            (std::uint16_t)(idx + 2),   (std::uint16_t)(_idx + 2),  (std::uint16_t)(_idx),
                            (std::uint16_t)(_idx + 1),  (std::uint16_t)(idx + 1),   (std::uint16_t)(idx),
                            (std::uint16_t)(idx),       (std::uint16_t)(_idx),      (std::uint16_t)(_idx + 1)
                            });
                    }

                    idx = _idx;
                }

                if(use_texture) {
                    rect_t tex_uvs = parent_shared_data->font->container_atlas->texture.uv_lines[integer_thickness];
                    for(int i = 0; i < points.size(); i++) {
                        prim_insert_vtx({
                            { temp_points[i * 2],       tex_uvs.min, color },
                            { temp_points[i * 2 + 1],   tex_uvs.max, color }
                            });
                    }
                } else {
                    for(int i = 0; i < points.size(); i++) {
                        prim_insert_vtx({
                            { points[i],                parent_shared_data->font->container_atlas->texture.uv_white_pixel, color },
                            { temp_points[i * 2],       parent_shared_data->font->container_atlas->texture.uv_white_pixel, color_t{ color, 0.f } },
                            { temp_points[i * 2 + 1],   parent_shared_data->font->container_atlas->texture.uv_white_pixel, color_t{ color, 0.f } }
                            });
                    }
                }
            } else {
                const float half_inner_thickness = (thickness - aa_size) * 0.5f;

                if(!closed) {
                    temp_points[0] = points.front() + temp_normals.front() * (half_inner_thickness + aa_size);
                    temp_points[1] = points.front() + temp_normals.front() * (half_inner_thickness);
                    temp_points[2] = points.front() - temp_normals.front() * (half_inner_thickness);
                    temp_points[3] = points.front() - temp_normals.front() * (half_inner_thickness + aa_size);
                    temp_points[(points.size() - 1) * 4]        = points.back() + temp_normals[(points.size() - 1)] * (half_inner_thickness + aa_size);
                    temp_points[(points.size() - 1) * 4 + 1]    = points.back() + temp_normals[(points.size() - 1)] * (half_inner_thickness);
                    temp_points[(points.size() - 1) * 4 + 2]    = points.back() - temp_normals[(points.size() - 1)] * (half_inner_thickness);
                    temp_points[(points.size() - 1) * 4 + 3]    = points.back() - temp_normals[(points.size() - 1)] * (half_inner_thickness + aa_size);
                }

                unsigned int idx = vtx_buffer.size();
                for(int i1 = 0; i1 < count; i1++) {
                    const int i2 = (i1 + 1) == points.size() ? 0 : (i1 + 1);
                    const unsigned int _idx = (i1 + 1) == points.size() ? vtx_buffer.size() : (idx + 4);

                    vec2_t p = (temp_normals[i1] + temp_normals[i2]) / 2;
                    p *= 1.f / std::max(std::powf(p.length(), 2), 0.5f);
                    vec2_t p_out{ p * (half_inner_thickness + aa_size) }, p_in{ p * half_inner_thickness };
                    temp_points[i2 * 4]     = points[i2] + p_out;
                    temp_points[i2 * 4 + 1] = points[i2] + p_in;
                    temp_points[i2 * 4 + 2] = points[i2] - p_in;
                    temp_points[i2 * 4 + 3] = points[i2] - p_out;

                    prim_insert_idx({
                        (std::uint16_t)(_idx + 1),  (std::uint16_t)(idx + 1),   (std::uint16_t)(idx + 2),
                        (std::uint16_t)(idx + 2),   (std::uint16_t)(_idx + 2),  (std::uint16_t)(_idx + 1),
                        (std::uint16_t)(_idx + 1),  (std::uint16_t)(idx + 1),   (std::uint16_t)(idx),
                        (std::uint16_t)(idx),       (std::uint16_t)(_idx),      (std::uint16_t)(_idx + 1),
                        (std::uint16_t)(_idx + 2),  (std::uint16_t)(idx + 2),   (std::uint16_t)(idx + 3),
                        (std::uint16_t)(idx + 3),   (std::uint16_t)(_idx + 3),  (std::uint16_t)(_idx + 2),
                        });

                    idx = _idx;
                }

                for(int i = 0; i < points.size(); i++) {
                    prim_insert_vtx({
                        { temp_points[i * 4],       parent_shared_data->font->container_atlas->texture.uv_white_pixel, color_t(color, 0.f) },
                        { temp_points[i * 4 + 1],   parent_shared_data->font->container_atlas->texture.uv_white_pixel, color },
                        { temp_points[i * 4 + 2],   parent_shared_data->font->container_atlas->texture.uv_white_pixel, color },
                        { temp_points[i * 4 + 3],   parent_shared_data->font->container_atlas->texture.uv_white_pixel, color_t(color, 0.f) }
                        });
                }
            }
        } else {
            for(int i1 = 0; i1 < count; i1++) {
                const int i2 = (i1 + 1) == points.size() ? 0 : i1 + 1;
                vec2_t p = points[i2] - points[i1];
                if(p.length() > 0.f) p *= 1.f / p.length();
                p *= thickness / 2;

                prim_insert_idx({
                    (std::uint16_t)vtx_buffer.size(), (std::uint16_t)(vtx_buffer.size() + 1), (std::uint16_t)(vtx_buffer.size() + 2),
                    (std::uint16_t)vtx_buffer.size(), (std::uint16_t)(vtx_buffer.size() + 2), (std::uint16_t)(vtx_buffer.size() + 3)
                    });

                prim_insert_vtx({
                    { points[i1] + vec2_t{ p.y, -p.x }, parent_shared_data->font->container_atlas->texture.uv_white_pixel, color },
                    { points[i2] + vec2_t{ p.y, -p.x }, parent_shared_data->font->container_atlas->texture.uv_white_pixel, color },
                    { points[i2] + vec2_t{ -p.y, p.x }, parent_shared_data->font->container_atlas->texture.uv_white_pixel, color },
                    { points[i1] + vec2_t{ -p.y, p.x }, parent_shared_data->font->container_atlas->texture.uv_white_pixel, color }
                    });
            }
        }
    }

    void c_draw_list::draw_circle(vec2_t center, color_t clr, float radius, int num_segments, float thickness) {
        if(clr.a() <= 0.f || radius <= 0.f) return;

        parent_shared_data->get_auto_circle_num_segments(num_segments, radius);

        const float a_max = (std::numbers::pi * 2.f) * (num_segments - 1.f) / num_segments;
        if(num_segments == 12) path_arc_to_fast(center, radius - 0.5f, 0, 11);
        else path_arc_to(center, radius - 0.5f, 0.f, a_max, num_segments - 1);
        path_stroke(clr, true, thickness);
    }

    void c_draw_list::draw_circle_filled(vec2_t center, color_t clr, float radius, int num_segments) {
        if(clr.a() <= 0.f || radius <= 0.f) return;

        parent_shared_data->get_auto_circle_num_segments(num_segments, radius);

        const float a_max = (std::numbers::pi * 2.f) * (num_segments - 1.f) / num_segments;
        if(num_segments == 12) path_arc_to_fast(center, radius, 0, 11);
        else path_arc_to(center, radius, 0.0f, a_max, num_segments - 1);
        path_fill_convex(clr);
    }
}