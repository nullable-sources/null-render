#include "../../../backend/renderer/renderer.h"
#include "../../../backend/internal/mesh.h"

#include "geometry.h"

namespace null::render {
    void i_geometry_command::handle() {
        backend::renderer->draw_geometry(topology, vertex_count, index_count, vertex_offset, index_offset);
    }

    c_geometry_command::c_geometry_command(backend::c_geometry_buffer* _geometry_buffer)
        : geometry_buffer(_geometry_buffer), i_geometry_command(_geometry_buffer->index_buffer.size(), _geometry_buffer->vertex_buffer.size()) { }

    void c_geometry_command::get_bounding_box(vec2_t<float>& min, vec2_t<float>& max) const {
        min = std::numeric_limits<float>::max(); max = std::numeric_limits<float>::min();
        for(const backend::vertex_t& vertex : geometry_buffer->vertex_buffer | std::views::drop(vertex_offset) | std::views::take(vertex_count)) {
            min = math::min(vertex.pos, min);
            max = math::max(vertex.pos, max);
        }
    }

    void c_geometry_command::recalculate_uvs(const vec2_t<float>& min, const vec2_t<float>& max) const {
        constexpr rect_t<float> uvs(vec2_t(0.f), vec2_t(1.f));
        for(backend::vertex_t& vertex : geometry_buffer->vertex_buffer | std::views::drop(vertex_offset) | std::views::take(vertex_count)) {
            vertex.uv = std::clamp((vertex.pos - min) / (max - min), uvs.min, uvs.max);
        }
    }

    void c_geometry_command::recalculate_uvs_unbound(const vec2_t<float>& min, const vec2_t<float>& max) const {
        constexpr rect_t<float> uvs(vec2_t(0.f), vec2_t(1.f));
        for(backend::vertex_t& vertex : geometry_buffer->vertex_buffer | std::views::drop(vertex_offset) | std::views::take(vertex_count)) {
            vertex.uv = (vertex.pos - min) / (max - min);
        }
    }

    void c_geometry_command::recalculate_uvs(const vec2_t<float>& min, const vec2_t<float>& max, const rect_t<float>& uvs) const {
        const vec2_t<float> uv_scale = uvs.size() / (max - min);
        const std::pair minmax_uv = std::minmax(uvs.min, uvs.max);

        for(backend::vertex_t& vertex : geometry_buffer->vertex_buffer | std::views::drop(vertex_offset) | std::views::take(vertex_count)) {
            vertex.uv = std::clamp(uvs.min + (vertex.pos - min) * uv_scale, minmax_uv.first, minmax_uv.second);
        }
    }

    void c_geometry_command::set_uvs(const rect_t<float>& uvs) const {
        vec2_t<float> min{ }, max{ };
        get_bounding_box(min, max);
        recalculate_uvs(min, max, uvs);
    }

    void c_geometry_command::set_default_uvs() const {
        vec2_t<float> min{ }, max{ };
        get_bounding_box(min, max);
        recalculate_uvs(min, max);
    }

    void c_geometry_command::clear_geometry() {
        if(geometry_buffer) {
            geometry_buffer->index_buffer.erase(
                std::next(geometry_buffer->index_buffer.begin(), index_offset),
                std::next(geometry_buffer->index_buffer.begin(), index_offset + index_count)
            );

            geometry_buffer->vertex_buffer.erase(
                std::next(geometry_buffer->vertex_buffer.begin(), vertex_offset),
                std::next(geometry_buffer->vertex_buffer.begin(), vertex_offset + vertex_count)
            );
        }

        index_offset = index_count = 0;
        vertex_offset = vertex_count = 0;
    }
}