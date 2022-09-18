#include <draw-list/draw-list.h>

namespace null::render {
    void c_draw_list::draw_data_t::deindex_all_buffers() {
        vtx_buffer_t new_vtx_buffer{ };
        total_vtx_count = total_idx_count = 0;
        for(c_draw_list* cmd_list : cmd_lists) {
            if(cmd_list->idx_buffer.empty()) continue;

            new_vtx_buffer.resize(cmd_list->idx_buffer.size());
            for(int i : std::views::iota((size_t)0, cmd_list->idx_buffer.size()))
                new_vtx_buffer[i] = cmd_list->vtx_buffer[cmd_list->idx_buffer[i]];
            cmd_list->vtx_buffer.swap(new_vtx_buffer);
            cmd_list->idx_buffer.clear();

            total_vtx_count += cmd_list->vtx_buffer.size();
        }
    }

    void c_draw_list::draw_data_t::add_draw_list(c_draw_list* draw_list) {
        draw_list->pop_unused_draw_cmd();
        if(draw_list->cmd_buffer.empty()) return;

        if(draw_list->vtx_buffer.empty()) throw std::runtime_error{ "vtx_buffer empty" };
        if(draw_list->idx_buffer.empty()) throw std::runtime_error{ "idx_buffer empty" };

        layers.push_back(draw_list);
    }

    void c_draw_list::draw_data_t::setup() {
        valid = true;
        cmd_lists = layers;
        total_vtx_count = total_idx_count = 0;
        std::ranges::for_each(layers, [&](const c_draw_list* draw_list) {
            total_vtx_count += draw_list->vtx_buffer.size();
            total_idx_count += draw_list->idx_buffer.size();
            });
    }

    void c_draw_list::shade_verts_linear_uv(const vtx_buffer_t::iterator& vtx_start, const vtx_buffer_t::iterator& vtx_end, const rect_t& rect, const rect_t& uv, bool clamp) {
        const vec2_t scale {
            rect.size().x != 0.0f ? (uv.size().x / rect.size().x) : 0.0f,
            rect.size().y != 0.0f ? (uv.size().y / rect.size().y) : 0.0f
        };

        std::for_each(vtx_start, vtx_end, [&](vertex_t& vertex) {
            if(clamp) vertex.uv = std::clamp(uv.min + (vertex.pos - rect.min) * scale, std::min(uv.min, uv.max), std::max(uv.min, uv.max));
            else vertex.uv = uv.min + (vertex.pos - rect.min) * scale;
            });
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
        if((vtx_buffer.size() + vtx_count >= (1 << 16)) && (parent_shared_data->initialize_flags & e_draw_list_flags::allow_vtx_offset)) {
            cmd_header.vtx_offset = vtx_buffer.size();
            on_changed_vtx_offset();
        }
    }

    void c_draw_list::prim_rect(const vec2_t& a, const vec2_t& c, const color_t& color) {
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

    void c_draw_list::prim_rect_uv(const vec2_t& a, const vec2_t& c, const vec2_t& uv_a, const vec2_t& uv_c, const color_t& color) {
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

    void c_draw_list::prim_quad_uv(const std::array<std::pair<vec2_t, vec2_t>, 4>& points, const color_t& color) {
        prim_insert_idx({
            (std::uint16_t)vtx_buffer.size(), (std::uint16_t)(vtx_buffer.size() + 1), (std::uint16_t)(vtx_buffer.size() + 2),
            (std::uint16_t)vtx_buffer.size(), (std::uint16_t)(vtx_buffer.size() + 2), (std::uint16_t)(vtx_buffer.size() + 3)
            });

        std::ranges::for_each(points, [&](const std::pair<vec2_t, vec2_t>& point) { prim_add_vtx({ point.first, point.second, color }); });
    }
    
    void c_draw_list::path_rect(const vec2_t& a, const vec2_t& b, float rounding, e_corner_flags flags) {
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

    void c_draw_list::path_arc_to_fast(const vec2_t& center, float radius, int a_min_of_12, int a_max_of_12) {
        if(radius == 0.0f || a_min_of_12 > a_max_of_12) {
            path.push_back(center);
            return;
        }

        a_min_of_12 *= shared_data_t::arc_fast_tessellation_multiplier;
        a_max_of_12 *= shared_data_t::arc_fast_tessellation_multiplier;

        std::ranges::for_each(std::views::iota(a_min_of_12, a_max_of_12 + 1), [=](const int& a){
            path.push_back(center + parent_shared_data->arc_fast_vtx[a % parent_shared_data->arc_fast_vtx.size()] * radius);
            });
    }

    void c_draw_list::path_arc_to(const vec2_t& center, float radius, float a_min, float a_max, int num_segments) {
        if(radius == 0.f) {
            path.push_back(center);
            return;
        }

        std::ranges::for_each(std::views::iota(0, num_segments + 1), [=](const int& i) {
            const float a{ a_min + ((float)i / (float)num_segments) * (a_max - a_min) };
            path.push_back(center + vec2_t{ cosf(a), sinf(a) } *radius);
            });
    }

    void c_draw_list::draw_line(const vec2_t& a, const vec2_t& b, const color_t& color, float thickness) {
        if(color.a() <= 0.f) return;

        path.push_back(a + 0.5f);
        path.push_back(b + 0.5f);
        path_stroke(color, false, thickness);
    }

    void c_draw_list::draw_rect(const vec2_t& a, const vec2_t& b, const color_t& color, float thickness, float rounding, e_corner_flags flags) {
        if(color.a() <= 0.f) return;

        path_rect(a + 0.50f, b - (parent_shared_data->initialize_flags & e_draw_list_flags::anti_aliased_lines ? 0.50f : 0.49f), rounding, flags);
        path_stroke(color, true, thickness);
    }

    void c_draw_list::draw_rect_filled(const vec2_t& a, const vec2_t& b, const color_t& color, float rounding, e_corner_flags flags) {
        if(color.a() <= 0.f) return;

        if(rounding > 0.0f) {
            path_rect(a, b, rounding, flags);
            path_fill_convex(color);
        } else prim_rect(a, b, color);
    }

    void c_draw_list::draw_convex_poly_filled(const std::vector<vec2_t>& points, const color_t& color) {
        if(points.size() < 3 || color.a() <= 0.f) return;

        if(parent_shared_data->initialize_flags & e_draw_list_flags::anti_aliased_fill) {
            static constexpr float aa_size{ 1.f };
            std::ranges::for_each(std::views::iota(2, (int)points.size()), [=](const int& i) {
                prim_insert_idx({
                    (std::uint16_t)(vtx_buffer.size()), (std::uint16_t)(vtx_buffer.size() + ((i - 1) << 1)), (std::uint16_t)(vtx_buffer.size() + (i << 1))
                    });
                });

            std::vector<vec2_t> temp_normals(points.size());
            for(int i0{ (int)points.size() - 1 }; int i1 : std::views::iota((size_t)0, points.size())) {
                vec2_t delta = points[i1] - points[i0];
                if(float d2{ std::powf(delta.length(), 2) }; d2 > 0.f) delta *= 1.f / std::sqrtf(d2);

                temp_normals[i0] = { delta.y, -delta.x };
                i0 = i1;
            }

            size_t idx{ vtx_buffer.size() };
            for(int i0{ (int)points.size() - 1 }; int i1 : std::views::iota((size_t)0, points.size())) {
                vec2_t delta = (temp_normals[i0] + temp_normals[i1]) / 2.f;
                if(float d2{ std::powf(delta.length(), 2) }; d2 > 0.000001f) delta *= 1.f / std::min(d2, 100.f);
                delta *= aa_size / 2.f;

                prim_insert_idx({
                    (std::uint16_t)(idx + (i1 << 1)),       (std::uint16_t)(idx + (i0 << 1)),       (std::uint16_t)(idx + 1 + (i0 << 1)),
                    (std::uint16_t)(idx + 1 + (i0 << 1)),   (std::uint16_t)(idx + 1 + (i1 << 1)),   (std::uint16_t)(idx + (i1 << 1))
                    });
                
                prim_insert_vtx({
                    { points[i1] - delta, parent_shared_data->font->container_atlas->texture.uv_white_pixel, color },
                    { points[i1] + delta, parent_shared_data->font->container_atlas->texture.uv_white_pixel, color_t{ color, 0.f } }
                    });
                i0 = i1;
            }
        } else {
            for(int i : std::views::iota((size_t)2, points.size())) prim_insert_idx({ (std::uint16_t)(vtx_buffer.size()), (std::uint16_t)(vtx_buffer.size() + i - 1), (std::uint16_t)(vtx_buffer.size() + i) });
            std::ranges::for_each(points, [&](const vec2_t& point) { prim_add_vtx({ point, parent_shared_data->font->container_atlas->texture.uv_white_pixel, color }); });
        }
    }

    void c_draw_list::draw_poly_line(const std::vector<vec2_t>& points, const color_t& color, bool closed, float thickness) {
        if(points.size() < 2 || color.a() <= 0.f) return;

        const int count{ int(closed ? points.size() : points.size() - 1) };
        const bool thick_line{ thickness > 1.0f };

        if(parent_shared_data->initialize_flags & e_draw_list_flags::anti_aliased_lines) {
            static constexpr float aa_size{ 1.0f };

            thickness = std::max(thickness, 1.0f);

            const bool use_texture{ (parent_shared_data->initialize_flags & e_draw_list_flags::anti_aliased_lines_use_texture) && ((int)thickness < 63) && (thickness - (int)thickness <= 0.00001f) };

            std::vector<vec2_t> temp_normals(points.size() * (use_texture || !thick_line ? 3 : 5));
            std::vector<vec2_t> temp_points(temp_normals.size() + points.size());

            for(int i1 : std::views::iota(0, count)) {
                vec2_t delta{ points[i1 + 1 == points.size() ? 0 : i1 + 1] - points[i1] };
                if(float d2{ std::powf(delta.length(), 2) }; d2 > 0.f) delta *= 1.f / std::sqrtf(d2);
                temp_normals[i1] = { delta.y, -delta.x };
            }

            if(!closed) //*std::prev(temp_normals.end(), points.size() - 1) = *std::prev(temp_normals.end(), points.size() - 2);
                temp_normals[points.size() - 1] = temp_normals[points.size() - 2];

            if(use_texture || !thick_line) {
                const float half_draw_size{ use_texture ? thickness * 0.5f + 1 : aa_size };
                if(!closed) {
                    temp_points[0] = points.front() + temp_normals.front() * half_draw_size;
                    temp_points[1] = points.front() - temp_normals.front() * half_draw_size;
                    temp_points[(points.size() - 1) * 2]        = points.back() + temp_normals[points.size() - 1] * half_draw_size;
                    temp_points[(points.size() - 1) * 2 + 1]    = points.back() - temp_normals[points.size() - 1] * half_draw_size;
                }

                size_t idx{ vtx_buffer.size() };
                for(int i1 : std::views::iota(0, count)) {
                    const bool last_point{ i1 + 1 == points.size() };
                    const int i2{ last_point ? 0 : (i1 + 1) };
                    const std::uint32_t _idx{ std::uint32_t(last_point ? vtx_buffer.size() : (idx + (use_texture ? 2 : 3))) };

                    vec2_t delta{ (temp_normals[i1] + temp_normals[i2]) / 2.f };
                    if(float d2{ std::powf(delta.length(), 2) }; d2 > 0.000001f) delta *= 1.f / std::min(d2, 100.f);
                    delta *= half_draw_size;

                    temp_points[i2 * 2] = points[i2] + delta;
                    temp_points[i2 * 2 + 1] = points[i2] - delta;

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
                    const rect_t& tex_uvs{ parent_shared_data->font->container_atlas->texture.uv_lines[(int)thickness] };
                    for(int i : std::views::iota((size_t)0, points.size())) {
                        prim_insert_vtx({
                            { temp_points[i * 2],       tex_uvs.min, color },
                            { temp_points[i * 2 + 1],   tex_uvs.max, color }
                            });
                    }
                } else {
                    for(int i : std::views::iota((size_t)0, points.size())) {
                        prim_insert_vtx({
                            { points[i],                parent_shared_data->font->container_atlas->texture.uv_white_pixel, color },
                            { temp_points[i * 2],       parent_shared_data->font->container_atlas->texture.uv_white_pixel, color_t{ color, 0.f } },
                            { temp_points[i * 2 + 1],   parent_shared_data->font->container_atlas->texture.uv_white_pixel, color_t{ color, 0.f } }
                            });
                    }
                }
            } else {
                const float half_inner_thickness{ (thickness - aa_size) * 0.5f };

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

                size_t idx{ vtx_buffer.size() };
                for(int i1 : std::views::iota(0, count)) {
                    const bool last_point{ i1 + 1 == points.size() };
                    const int i2{ last_point ? 0 : (i1 + 1) };
                    const std::uint32_t _idx{ std::uint32_t(last_point ? vtx_buffer.size() : (idx + 4)) };

                    vec2_t delta{ (temp_normals[i1] + temp_normals[i2]) / 2.f };
                    if(float d2{ std::powf(delta.length(), 2) }; d2 > 0.000001f) delta *= 1.f / std::min(d2, 100.f);
                    vec2_t out = delta * (half_inner_thickness + aa_size), in = delta * half_inner_thickness;
                    temp_points[i2 * 4]     = points[i2] + out;
                    temp_points[i2 * 4 + 1] = points[i2] + in;
                    temp_points[i2 * 4 + 2] = points[i2] - in;
                    temp_points[i2 * 4 + 3] = points[i2] - out;

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

                for(int i : std::views::iota((size_t)0, points.size())) {
                    prim_insert_vtx({
                        { temp_points[i * 4],       parent_shared_data->font->container_atlas->texture.uv_white_pixel, color_t(color, 0.f) },
                        { temp_points[i * 4 + 1],   parent_shared_data->font->container_atlas->texture.uv_white_pixel, color },
                        { temp_points[i * 4 + 2],   parent_shared_data->font->container_atlas->texture.uv_white_pixel, color },
                        { temp_points[i * 4 + 3],   parent_shared_data->font->container_atlas->texture.uv_white_pixel, color_t(color, 0.f) }
                        });
                }
            }
        } else {
            for(int i1 : std::views::iota(0, count)) {
                const int i2{ i1 + 1 == points.size() ? 0 : (i1 + 1) };
                vec2_t delta{ points[i2] - points[i1] };
                if(float d2{ std::powf(delta.length(), 2) }; d2 > 0.f) delta *= 1.f / std::sqrtf(d2);
                delta *= thickness / 2.f;

                prim_insert_idx({
                    (std::uint16_t)vtx_buffer.size(), (std::uint16_t)(vtx_buffer.size() + 1), (std::uint16_t)(vtx_buffer.size() + 2),
                    (std::uint16_t)vtx_buffer.size(), (std::uint16_t)(vtx_buffer.size() + 2), (std::uint16_t)(vtx_buffer.size() + 3)
                    });

                prim_insert_vtx({
                    { points[i1] + vec2_t{ delta.y, -delta.x }, parent_shared_data->font->container_atlas->texture.uv_white_pixel, color },
                    { points[i2] + vec2_t{ delta.y, -delta.x }, parent_shared_data->font->container_atlas->texture.uv_white_pixel, color },
                    { points[i2] + vec2_t{ -delta.y, delta.x }, parent_shared_data->font->container_atlas->texture.uv_white_pixel, color },
                    { points[i1] + vec2_t{ -delta.y, delta.x }, parent_shared_data->font->container_atlas->texture.uv_white_pixel, color }
                    });
            }
        }
    }

    void c_draw_list::draw_circle(const vec2_t& center, const color_t& clr, float radius, int num_segments, float thickness) {
        if(clr.a() <= 0.f || radius <= 0.f) return;

        parent_shared_data->get_auto_circle_num_segments(num_segments, radius);

        if(num_segments == 12) path_arc_to_fast(center, radius - 0.5f, 0, 11);
        else path_arc_to(center, radius - 0.5f, 0.f, (std::numbers::pi * 2.f) * (num_segments - 1.f) / num_segments, num_segments - 1);
        path_stroke(clr, true, thickness);
    }

    void c_draw_list::draw_circle_filled(const vec2_t& center, const color_t& clr, float radius, int num_segments) {
        if(clr.a() <= 0.f || radius <= 0.f) return;

        parent_shared_data->get_auto_circle_num_segments(num_segments, radius);

        if(num_segments == 12) path_arc_to_fast(center, radius, 0, 11);
        else path_arc_to(center, radius, 0.0f, (std::numbers::pi * 2.f) * (num_segments - 1.f) / num_segments, num_segments - 1);
        path_fill_convex(clr);
    }
}