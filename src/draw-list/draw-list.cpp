#include <null-render.h>

namespace null {
    namespace render {
        void c_draw_list::reset() {
            settings.set_circle_segment_max_error(1.60f);

            clear();
            textures = { atlas.texture.data };
            clips = { { vec2_t{ 0.f }, renderer::draw_data_t::screen_size } };
            add_cmd();
        }

        void c_draw_list::restore_clip_rect() { push_clip_rect(0.f, renderer::draw_data_t::screen_size); }
        void c_draw_list::push_clip_rect(rect_t rect, const bool& intersect_with_current_rect) {
            if(intersect_with_current_rect) rect = { math::max(rect.min, clips.back().min), math::min(rect.max, clips.back().max) };

            clips.push_back(rect_t{ rect.min, std::max(rect.min, rect.max) });
            on_change_clip_rect();
        }

        void c_draw_list::add_quad_uv(const std::array<std::pair<vec2_t, vec2_t>, 4>& points, const color_t<int>& color) {
            add_idx({
                0, 1, 2,
                0, 2, 3
                }, vtx_buffer.size());

            std::ranges::transform(points, std::back_inserter(vtx_buffer), [&](const std::pair<vec2_t, vec2_t>& val) { return vertex_t{ val.first, val.second, color }; });
        }

        void c_draw_list::path_rect(const vec2_t& a, const vec2_t& b, float rounding, const e_corner_flags& flags) {
            rounding = std::min(rounding, std::fabsf(b.x - a.x) * ((flags & e_corner_flags::top) == -e_corner_flags::top || (flags & e_corner_flags::bot) == -e_corner_flags::bot ? 0.5f : 1.f) - 1.f);
            rounding = std::min(rounding, std::fabsf(b.y - a.y) * ((flags & e_corner_flags::left) == -e_corner_flags::left || (flags & e_corner_flags::right) == -e_corner_flags::right ? 0.5f : 1.f) - 1.f);

            if(rounding <= 0.0f || flags == e_corner_flags{ }) {
                pathes.push_back(a);
                pathes.push_back({ b.x, a.y });
                pathes.push_back(b);
                pathes.push_back({ a.x, b.y });
            } else {
                float rounding_tl{ flags & e_corner_flags::top_left ? rounding : 0.f };
                float rounding_tr{ flags & e_corner_flags::top_right ? rounding : 0.f };
                float rounding_br{ flags & e_corner_flags::bot_right ? rounding : 0.f };
                float rounding_bl{ flags & e_corner_flags::bot_left ? rounding : 0.f };

                path_arc_to_fast(a + rounding_tl, rounding_tl, 6, 9);
                path_arc_to_fast(vec2_t{ b.x, a.y } + vec2_t{ -rounding_tr, rounding_tr }, rounding_tr, 9, 12);
                path_arc_to_fast(b - rounding_br, rounding_br, 0, 3);
                path_arc_to_fast(vec2_t{ a.x, b.y } + vec2_t{ rounding_bl, -rounding_bl }, rounding_bl, 3, 6);
            }
        }

        void c_draw_list::path_arc_to_fast(const vec2_t& center, const float& radius, const int& a_min_of_12, const int& a_max_of_12) {
            if(!radius || a_min_of_12 > a_max_of_12) pathes.push_back(center);
            else {
                std::ranges::for_each(std::views::iota(a_min_of_12 * settings_t::arc_fast_tessellation_multiplier, a_max_of_12 * settings_t::arc_fast_tessellation_multiplier + 1), [=](const int& a) {
                    pathes.push_back(center + settings.arc_fast_vtx[a % settings.arc_fast_vtx.size()] * radius);
                    });
            }
        }

        void c_draw_list::path_arc_to(const vec2_t& center, const float& radius, const float& a_min, const float& a_max, const int& num_segments) {
            if(radius == 0.f) pathes.push_back(center);
            else {
                std::ranges::for_each(std::views::iota(0, num_segments + 1), [=](const int& i) {
                    const float a{ a_min + ((float)i / (float)num_segments) * (a_max - a_min) };
                    pathes.push_back(center + vec2_t{ cosf(a), sinf(a) } *radius);
                    });
            }
        }

        void c_draw_list::repaint_rect_vertices_in_multicolor(const vec2_t& min, const vec2_t& max, const size_t& vtx_offset, const std::array<color_t<int>, 4>& colors) {
            std::array<color_t<float>, 4> casted_colors{ };
            std::ranges::transform(colors, casted_colors.begin(), [](const color_t<int>& color) { return color; });

            for(vertex_t& vertex : vtx_buffer | std::views::drop(vtx_offset)) { //@credits: https://github.com/ocornut/imgui/issues/3710#issuecomment-1315745540
                vec2_t f{ std::clamp((vertex.pos - min) / (max - min), { 0.f }, { 1.f }) };
                color_t<float> top_delta{ casted_colors[0] + (casted_colors[1] - casted_colors[0]) * color_t<float>{ f.x } };
                color_t<float> bot_delta{ casted_colors[2] + (casted_colors[3] - casted_colors[2]) * color_t<float>{ f.x } };
                vertex.color *= color_t<float>{ top_delta + (bot_delta - top_delta) * color_t<float>{ f.y } };
            }
        }

        void c_draw_list::draw_line(const vec2_t& a, const vec2_t& b, const color_t<int>& color, const float& thickness) {
            if(color.a() <= 0) return;

            pathes.push_back(a + 0.5f);
            pathes.push_back(b + 0.5f);
            path_stroke(color, false, thickness);
        }

        void c_draw_list::draw_rect(const vec2_t& a, const vec2_t& b, const color_t<int>& color, const float& thickness, const float& rounding, const e_corner_flags& flags) {
            if(color.a() <= 0) return;

            path_rect(a + 0.50f, b - (settings.initialize_flags & e_initialize_flags::anti_aliased_lines ? 0.50f : 0.49f), rounding, flags);
            path_stroke(color, true, thickness);
        }

        //@note: colors = { top left, top right, bottom left, bottom right };
        void c_draw_list::draw_rect_multicolor(const vec2_t& a, const vec2_t& b, const std::array<color_t<int>, 4>& colors, const float& thickness, const float& rounding, const e_corner_flags& flags) {
            if(std::ranges::all_of(colors, [](const color_t<int>& color) { return color.a() <= 0; })) return;

            size_t offset{ vtx_buffer.size() };
            draw_rect(a, b, color_t<int>::palette_t::white, thickness, rounding, flags);

            //@note: i'm sure it can be done differently, but i don't really give a fuck
            vec2_t min{ std::numeric_limits<float>::max() }, max{ std::numeric_limits<float>::min() };
            for(const vertex_t& vertex : vtx_buffer | std::views::drop(offset)) {
                min = math::min(vertex.pos, min);
                max = math::max(vertex.pos, max);
            }

            repaint_rect_vertices_in_multicolor(min, max, offset, colors);
        }

        void c_draw_list::draw_rect_filled(const vec2_t& a, const vec2_t& b, const color_t<int>& color, const float& rounding, const e_corner_flags& flags) {
            if(color.a() <= 0) return;

            if(rounding > 0.0f) {
                path_rect(a, b, rounding, flags);
                path_fill_convex(color);
            } else add_rect(a, b, color);
        }

        //@note: colors = { top left, top right, bottom left, bottom right };
        void c_draw_list::draw_rect_filled_multicolor(const vec2_t& a, const vec2_t& b, const std::array<color_t<int>, 4>& colors, float rounding, const e_corner_flags& flags) {
            if(std::ranges::all_of(colors, [](const color_t<int>& color) { return color.a() <= 0; })) return;

            rounding = std::min(rounding, std::fabsf(b.x - a.x) * ((flags & e_corner_flags::top) == -e_corner_flags::top || (flags & e_corner_flags::bot) == -e_corner_flags::bot ? 0.5f : 1.f) - 1.f);
            rounding = std::min(rounding, std::fabsf(b.y - a.y) * ((flags & e_corner_flags::left) == -e_corner_flags::left || (flags & e_corner_flags::right) == -e_corner_flags::right ? 0.5f : 1.f) - 1.f);
            
            if(rounding > 0.f && flags != e_corner_flags{ }) {
                size_t offset{ vtx_buffer.size() };
                path_rect(a, b, rounding, flags);
                path_fill_convex(color_t<int>::palette_t::white);
                repaint_rect_vertices_in_multicolor(a, b, offset, colors);
            } else {
                add_idx({
                    0, 1, 2,
                    0, 2, 3
                    }, vtx_buffer.size());

                add_vtx({
                    { a, atlas.texture.uv_white_pixel, colors[0] },
                    { { b.x, a.y }, atlas.texture.uv_white_pixel, colors[1] },
                    { b, atlas.texture.uv_white_pixel, colors[3] },
                    { { a.x, b.y }, atlas.texture.uv_white_pixel, colors[2] },
                    });
            }
        }

        void c_draw_list::draw_convex_poly_filled(const std::vector<vec2_t>& points, const color_t<int>& color) {
            if(points.size() < 3 || color.a() <= 0) return;

            if(settings.initialize_flags & e_initialize_flags::anti_aliased_fill) {
                static constexpr float aa_size{ 1.f };
                std::ranges::for_each(std::views::iota(2, (int)points.size()), [=](const std::uint32_t& i) {
                    add_idx({ 0, (i - 1) << 1, i << 1 }, vtx_buffer.size());
                    });

                std::vector<vec2_t> temp_normals(points.size());
                for(int i0{ (int)points.size() - 1 }; const int& i1 : std::views::iota((size_t)0, points.size())) {
                    vec2_t delta{ points[i1] - points[i0] };
                    if(float d2{ std::powf(delta.length(), 2) }; d2 > 0.f) delta *= 1.f / std::sqrtf(d2);

                    temp_normals[i0] = { delta.y, -delta.x };
                    i0 = i1;
                }

                size_t idx{ vtx_buffer.size() };
                for(std::uint32_t i0{ (std::uint32_t)points.size() - 1 }; const std::uint32_t& i1 : std::views::iota((size_t)0, points.size())) {
                    vec2_t delta{ (temp_normals[i0] + temp_normals[i1]) / 2.f };
                    if(float d2{ std::powf(delta.length(), 2) }; d2 > 0.000001f) delta *= 1.f / std::min(d2, 100.f);
                    delta *= aa_size / 2.f;

                    add_idx({
                        i1 << 1,        i0 << 1,        (i0 << 1) + 1,
                        (i0 << 1) + 1,  (i1 << 1) + 1,  i1 << 1
                        }, idx);

                    add_vtx({
                        { points[i1] - delta, atlas.texture.uv_white_pixel, color },
                        { points[i1] + delta, atlas.texture.uv_white_pixel, color_t{ color, 0.f } }
                        });
                    i0 = i1;
                }
            } else {
                for(const std::uint32_t& i : std::views::iota((size_t)2, points.size())) add_idx({ 0, i - 1, i }, vtx_buffer.size());
                std::ranges::for_each(points, [&](const vec2_t& point) { add_vtx({ { point, atlas.texture.uv_white_pixel, color } }); });
            }
        }

        void c_draw_list::draw_poly_line(const std::vector<vec2_t>& points, const color_t<int>& color, const bool& closed, float thickness) {
            if(points.size() < 2 || color.a() <= 0) return;

            const int count{ int(closed ? points.size() : points.size() - 1) };
            const bool thick_line{ thickness > 1.0f };

            if(settings.initialize_flags & e_initialize_flags::anti_aliased_lines) {
                static constexpr float aa_size{ 1.0f };

                thickness = std::max(thickness, 1.0f);

                const bool use_texture{ (settings.initialize_flags & e_initialize_flags::anti_aliased_lines_use_texture) && ((int)thickness < 63) && (thickness - (int)thickness <= 0.00001f) };

                std::vector<vec2_t> temp_normals(points.size() * (use_texture || !thick_line ? 3 : 5));
                std::vector<vec2_t> temp_points(temp_normals.size() + points.size());

                for(const int& i1 : std::views::iota(0, count)) {
                    vec2_t delta{ points[i1 + 1 == points.size() ? 0 : i1 + 1] - points[i1] };
                    if(float d2{ std::powf(delta.length(), 2) }; d2 > 0.f) delta *= 1.f / std::sqrtf(d2);
                    temp_normals[i1] = { delta.y, -delta.x };
                }

                if(!closed) temp_normals[points.size() - 1] = temp_normals[points.size() - 2];

                if(false && (use_texture || !thick_line)) {
                    const float half_draw_size{ use_texture ? thickness * 0.5f + 1 : aa_size };
                    if(!closed) {
                        temp_points[0] = points.front() + temp_normals.front() * half_draw_size;
                        temp_points[1] = points.front() - temp_normals.front() * half_draw_size;
                        temp_points[(points.size() - 1) * 2] = points.back() + temp_normals[points.size() - 1] * half_draw_size;
                        temp_points[(points.size() - 1) * 2 + 1] = points.back() - temp_normals[points.size() - 1] * half_draw_size;
                    }

                    std::uint32_t idx{ (std::uint32_t)vtx_buffer.size() };
                    for(const int& i1 : std::views::iota(0, count)) {
                        const bool last_point{ i1 + 1 == points.size() };
                        const int i2{ last_point ? 0 : (i1 + 1) };
                        const std::uint32_t _idx{ std::uint32_t(last_point ? vtx_buffer.size() : (idx + (use_texture ? 2 : 3))) };

                        vec2_t delta{ (temp_normals[i1] + temp_normals[i2]) / 2.f };
                        if(float d2{ std::powf(delta.length(), 2) }; d2 > 0.000001f) delta *= 1.f / std::min(d2, 100.f);
                        delta *= half_draw_size;

                        temp_points[i2 * 2] = points[i2] + delta;
                        temp_points[i2 * 2 + 1] = points[i2] - delta;

                        if(use_texture) {
                            add_idx({
                                _idx,      idx,       idx + 1,
                                _idx + 1,  idx + 1,   _idx
                                });
                        } else {
                            add_idx({
                                _idx,       idx,        idx + 2,
                                idx + 2,    _idx + 2,   _idx,
                                _idx + 1,   idx + 1,    idx,
                                idx,        _idx,       _idx + 1
                                });
                        }

                        idx = _idx;
                    }

                    if(use_texture) {
                        const rect_t& tex_uvs{ c_font::get_current_font()->container_atlas->texture.uv_lines[(int)thickness] };
                        for(const int& i : std::views::iota((size_t)0, points.size())) {
                            add_vtx({
                                { temp_points[i * 2],       tex_uvs.min, color },
                                { temp_points[i * 2 + 1],   tex_uvs.max, color }
                                });
                        }
                    } else {
                        for(const int& i : std::views::iota((size_t)0, points.size())) {
                            add_vtx({
                                { points[i],                atlas.texture.uv_white_pixel, color },
                                { temp_points[i * 2],       atlas.texture.uv_white_pixel, color_t{ color, 0.f } },
                                { temp_points[i * 2 + 1],   atlas.texture.uv_white_pixel, color_t{ color, 0.f } }
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
                        temp_points[(points.size() - 1) * 4] = points.back() + temp_normals[(points.size() - 1)] * (half_inner_thickness + aa_size);
                        temp_points[(points.size() - 1) * 4 + 1] = points.back() + temp_normals[(points.size() - 1)] * (half_inner_thickness);
                        temp_points[(points.size() - 1) * 4 + 2] = points.back() - temp_normals[(points.size() - 1)] * (half_inner_thickness);
                        temp_points[(points.size() - 1) * 4 + 3] = points.back() - temp_normals[(points.size() - 1)] * (half_inner_thickness + aa_size);
                    }

                    std::uint32_t idx{ (std::uint32_t)vtx_buffer.size() };
                    for(const int& i1 : std::views::iota(0, count)) {
                        const bool last_point{ i1 + 1 == points.size() };
                        const int i2{ last_point ? 0 : (i1 + 1) };
                        const std::uint32_t _idx{ std::uint32_t(last_point ? vtx_buffer.size() : (idx + 4)) };

                        vec2_t delta{ (temp_normals[i1] + temp_normals[i2]) / 2.f };
                        if(float d2{ std::powf(delta.length(), 2) }; d2 > 0.000001f) delta *= 1.f / std::min(d2, 100.f);
                        vec2_t out{ delta * (half_inner_thickness + aa_size) }, in{ delta * half_inner_thickness };
                        temp_points[i2 * 4] = points[i2] + out;
                        temp_points[i2 * 4 + 1] = points[i2] + in;
                        temp_points[i2 * 4 + 2] = points[i2] - in;
                        temp_points[i2 * 4 + 3] = points[i2] - out;

                        add_idx({
                            _idx + 1,   idx + 1,    idx + 2,
                            idx + 2,    _idx + 2,   _idx + 1,
                            _idx + 1,   idx + 1,    idx,
                            idx,        _idx,       _idx + 1,
                            _idx + 2,   idx + 2,    idx + 3,
                            idx + 3,    _idx + 3,   _idx + 2,
                            });

                        idx = _idx;
                    }

                    for(const int& i : std::views::iota((size_t)0, points.size())) {
                        add_vtx({
                            { temp_points[i * 4],       atlas.texture.uv_white_pixel, color_t(color, 0.f) },
                            { temp_points[i * 4 + 1],   atlas.texture.uv_white_pixel, color },
                            { temp_points[i * 4 + 2],   atlas.texture.uv_white_pixel, color },
                            { temp_points[i * 4 + 3],   atlas.texture.uv_white_pixel, color_t(color, 0.f) }
                            });
                    }
                }
            } else {
                for(const int& i1 : std::views::iota(0, count)) {
                    const int i2{ i1 + 1 == points.size() ? 0 : (i1 + 1) };
                    vec2_t delta{ points[i2] - points[i1] };
                    if(float d2{ std::powf(delta.length(), 2) }; d2 > 0.f) delta *= 1.f / std::sqrtf(d2);
                    delta *= thickness / 2.f;

                    add_idx({
                        0, 1, 2,
                        0, 2, 3
                        }, vtx_buffer.size());

                    add_vtx({
                        { points[i1] + vec2_t{ delta.y, -delta.x }, atlas.texture.uv_white_pixel, color },
                        { points[i2] + vec2_t{ delta.y, -delta.x }, atlas.texture.uv_white_pixel, color },
                        { points[i2] + vec2_t{ -delta.y, delta.x }, atlas.texture.uv_white_pixel, color },
                        { points[i1] + vec2_t{ -delta.y, delta.x }, atlas.texture.uv_white_pixel, color }
                        });
                }
            }
        }

        void c_draw_list::draw_circle(const vec2_t& center, const color_t<int>& clr, const float& radius, int num_segments, const float& thickness) {
            if(clr.a() <= 0 || radius <= 0.f) return;

            settings.get_auto_circle_num_segments(num_segments, radius);

            if(num_segments == 12) path_arc_to_fast(center, radius - 0.5f, 0, 11);
            else path_arc_to(center, radius - 0.5f, 0.f, (std::numbers::pi * 2.f) * (num_segments - 1.f) / num_segments, num_segments - 1);
            path_stroke(clr, true, thickness);
        }

        void c_draw_list::draw_circle_filled(const vec2_t& center, const color_t<int>& clr, const float& radius, int num_segments) {
            if(clr.a() <= 0 || radius <= 0.f) return;

            settings.get_auto_circle_num_segments(num_segments, radius);

            if(num_segments == 12) path_arc_to_fast(center, radius, 0, 11);
            else path_arc_to(center, radius, 0.0f, (std::numbers::pi * 2.f) * (num_segments - 1.f) / num_segments, num_segments - 1);
            path_fill_convex(clr);
        }

        void c_draw_list::draw_image(void* texture, const vec2_t& a, const vec2_t& b, const vec2_t& uv_min, const vec2_t& uv_max, const color_t<int>& color) {
            if(color.a() <= 0) return;

            const bool _push_texture{ texture != cmd_buffer.back().texture };
            if(_push_texture) push_texture(texture);
            add_rect_uv(a, b, uv_min, uv_max, color);
            if(_push_texture) pop_texture();
        }

        void c_draw_list::draw_image_quad(void* texture, const std::array<std::pair<vec2_t, vec2_t>, 4>& points_and_uvs, const color_t<int>& color) {
            if(color.a() <= 0) return;

            const bool _push_texture{ texture != cmd_buffer.back().texture };
            if(_push_texture) push_texture(texture);
            add_quad_uv(points_and_uvs, color);
            if(_push_texture) pop_texture();
        }

        void c_draw_list::on_change_clip_rect() {
            if(cmd_buffer.back().element_count && cmd_buffer.back().clip_rect != clips.back()) add_cmd();
            else cmd_buffer.back().clip_rect = clips.back();
        }

        void c_draw_list::on_change_texture() {
            if(cmd_buffer.back().element_count && cmd_buffer.back().texture != textures.back()) add_cmd();
            else cmd_buffer.back().texture = textures.back();
        }
    }

    namespace renderer {
        void draw_data_t::add_draw_list(render::c_draw_list* draw_list) {
            if(draw_list && !draw_list->cmd_buffer.empty() && !draw_list->vtx_buffer.empty() && !draw_list->idx_buffer.empty()) {
                draw_lists.push_back(draw_list);
            }
        }
        void draw_data_t::setup() {
            total_vtx_count = total_idx_count = 0;
            std::ranges::for_each(draw_lists, [&](const render::c_draw_list* draw_list) {
                total_vtx_count += draw_list->vtx_buffer.size();
                total_idx_count += draw_list->idx_buffer.size();
                });
        }

        void setup_default_draw_data() {
            draw_data.draw_lists.clear();
            draw_data.add_draw_list(&render::background);
            std::ranges::for_each(render::custom_lists, [](render::c_draw_list* draw_list) { draw_data.add_draw_list(draw_list); });
            std::ranges::for_each(render::fast_lists, [](render::c_draw_list* draw_list) { draw_data.add_draw_list(draw_list); });
            draw_data.add_draw_list(&render::foreground);
            draw_data.setup();

            render::fast_lists.clear();
        }
    }
}