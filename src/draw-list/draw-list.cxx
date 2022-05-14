module null.render;
import :draw_list;
import :font;

namespace null::render {
    void c_draw_list::draw_data_t::de_index_all_buffers() {
        std::vector<vertex_t> new_vtx_buffer{ };
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
        if(draw_list->vtx_write != draw_list->vtx_buffer.end()) throw std::runtime_error("vtx_write != vtx_buffer.back()");

        if(draw_list->idx_buffer.empty()) throw std::runtime_error("idx_buffer empty");
        if(draw_list->idx_write != draw_list->idx_buffer.end()) throw std::runtime_error("idx_write != idx_buffer.back()");

        if(!(draw_list->flags & e_draw_list_flags::allow_vtx_offset) && draw_list->vtx_current_idx != draw_list->vtx_buffer.size())
            throw std::runtime_error("vtx_current_idx != vtx_buffer.size()");

        //https://github.com/ocornut/imgui/blob/v1.80/imgui.cpp#L4145
        if(sizeof(std::uint16_t) == 2 && draw_list->vtx_current_idx >= (1 << 16))
            throw std::runtime_error("Too many vertices in c_draw_list using 16-bit indices. Read comment above");

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

    void c_draw_list::shade_verts_linear_uv(std::vector<vertex_t>::iterator vtx_start, std::vector<vertex_t>::iterator vtx_end, rect_t rect, rect_t uv, bool clamp) {
        const vec2_t scale = vec2_t{
            rect.size().x != 0.0f ? (uv.size().x / rect.size().x) : 0.0f,
            rect.size().y != 0.0f ? (uv.size().y / rect.size().y) : 0.0f };

        for(std::vector<vertex_t>::iterator vertex = vtx_start; vertex < vtx_end; ++vertex) {
            if(clamp) vertex->uv = std::clamp(uv.min + (vertex->pos - rect.min) * scale, std::min(uv.min, uv.max), std::max(uv.min, uv.max));
            else vertex->uv = uv.min + (vertex->pos - rect.min) * scale;
        }
    }

    void c_draw_list::on_changed_vtx_offset() {
        vtx_current_idx = 0;
        if(cmd_buffer.back().element_count) {
            add_draw_cmd();
            return;
        }

        cmd_buffer.back().vtx_offset = cmd_header.vtx_offset;
    }

    void c_draw_list::prim_reserve(int idx_count, int vtx_count) {
        if(sizeof(std::uint16_t) == 2 && (vtx_current_idx + vtx_count >= (1 << 16)) && (flags & e_draw_list_flags::allow_vtx_offset)) {
            cmd_header.vtx_offset = vtx_buffer.size();
            on_changed_vtx_offset();
        }

        cmd_buffer.back().element_count += idx_count;

        vtx_buffer.insert(vtx_buffer.end(), vtx_count, { });
        vtx_write = std::prev(vtx_buffer.end(), vtx_count);

        idx_buffer.insert(idx_buffer.end(), idx_count, { });
        idx_write = std::prev(idx_buffer.end(), idx_count);
    }

    void c_draw_list::prim_unreserve(int idx_count, int vtx_count) {
        cmd_buffer.back().element_count -= idx_count;
        vtx_buffer.erase(vtx_buffer.end() - vtx_count, vtx_buffer.end());
        idx_buffer.erase(idx_buffer.end() - idx_count, idx_buffer.end());
    }

    void c_draw_list::prim_rect(vec2_t a, vec2_t c, color_t color) {
        std::vector<std::uint16_t> idx_list {
            (std::uint16_t)vtx_current_idx, (std::uint16_t)(vtx_current_idx + 1), (std::uint16_t)(vtx_current_idx + 2),
            (std::uint16_t)vtx_current_idx, (std::uint16_t)(vtx_current_idx + 2), (std::uint16_t)(vtx_current_idx + 3)
        };
        prim_write_idx(idx_list);

        std::vector<vertex_t> vtx_list {
            { a, shared_data->font->container_atlas->texture.uv_white_pixel, color },
            { { c.x, a.y }, shared_data->font->container_atlas->texture.uv_white_pixel, color },
            { c, shared_data->font->container_atlas->texture.uv_white_pixel, color },
            { { a.x, c.y }, shared_data->font->container_atlas->texture.uv_white_pixel, color },
        };
        prim_write_vtx(vtx_list);
    }

    void c_draw_list::prim_rect_uv(vec2_t a, vec2_t c, vec2_t uv_a, vec2_t uv_c, color_t color) {
        std::vector<std::uint16_t> idx_list {
            (std::uint16_t)vtx_current_idx, (std::uint16_t)(vtx_current_idx + 1), (std::uint16_t)(vtx_current_idx + 2),
            (std::uint16_t)vtx_current_idx, (std::uint16_t)(vtx_current_idx + 2), (std::uint16_t)(vtx_current_idx + 3)
        };
        prim_write_idx(idx_list);

        std::vector<vertex_t> vtx_list {
            { a, uv_a, color },
            { { c.x, a.y }, { uv_c.x, uv_a.y }, color },
            { c, uv_c, color },
            { { a.x, c.y }, { uv_a.x, uv_c.y }, color }
        };
        prim_write_vtx(vtx_list);
    }

    void c_draw_list::prim_quad_uv(std::array<vec2_t, 4> points, std::array<vec2_t, 4> uvs, color_t color) {
        std::vector<std::uint16_t> idx_list{
            (std::uint16_t)vtx_current_idx, (std::uint16_t)(vtx_current_idx + 1), (std::uint16_t)(vtx_current_idx + 2),
            (std::uint16_t)vtx_current_idx, (std::uint16_t)(vtx_current_idx + 2), (std::uint16_t)(vtx_current_idx + 3)
        };
        prim_write_idx(idx_list);

        std::transform(points.begin(), points.end(), uvs.begin(), points.begin(), [&](vec2_t& point, vec2_t& uv) { prim_write_vtx({ point, uv, color }); return point; });
    }
}