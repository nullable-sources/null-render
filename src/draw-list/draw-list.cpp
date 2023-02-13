#include <renderer/renderer.h>

namespace null::render {
    void c_draw_list::reset() {
        settings.set_circle_segment_max_error(1.60f);

        clear();
        textures = { atlas.texture.data };
        clips = { { vec2_t{ 0.f }, renderer::draw_data_t::screen_size } };
        add_cmd();
    }

    void c_draw_list::restore_clip_rect() { push_clip_rect(0.f, renderer::draw_data_t::screen_size); }
    void c_draw_list::push_clip_rect(rect_t<float> rect, const bool& intersect_with_current_rect) {
        if(intersect_with_current_rect) rect = { math::max(rect.min, clips.back().min), math::min(rect.max, clips.back().max) };

        clips.push_back(rect_t{ rect.min, std::max(rect.min, rect.max) });
        on_change_clip_rect();
    }

    void c_draw_list::add_image(void* texture, const vec2_t<float>& a, const vec2_t<float>& b, const vec2_t<float>& uv_min, const vec2_t<float>& uv_max, const color_t<int>& color) {
        if(color.a() <= 0) return;

        const bool _push_texture{ texture != cmd_buffer.back().texture };
        if(_push_texture) push_texture(texture);

        add_idx(geometry_utils::quad_indicies, vtx_buffer.size());
        add_vtx(geometry_utils::build_rect_vertex(a, b, uv_min, uv_max, color));

        if(_push_texture) pop_texture();
    }

    void c_draw_list::add_image_quad(void* texture, const std::array<std::pair<vec2_t<float>, vec2_t<float>>, 4>& points_and_uvs, const color_t<int>& color) {
        if(color.a() <= 0) return;

        const bool _push_texture{ texture != cmd_buffer.back().texture };
        if(_push_texture) push_texture(texture);

        add_idx(geometry_utils::quad_indicies, vtx_buffer.size());
        std::ranges::transform(points_and_uvs, std::back_inserter(vtx_buffer), [&](const auto& pos_and_uv) { return vertex_t{ pos_and_uv.first, pos_and_uv.second, color }; });

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