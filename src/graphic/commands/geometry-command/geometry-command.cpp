#include <backend/renderer/renderer.h>
#include <backend/internal/mesh.h>

#include <graphic/commands/geometry-command/geometry-command.h>

namespace null::render::commands {
	c_geometry::c_geometry()
		: index_offset{ backend::mesh->geometry_buffer.index_buffer.size() }, vertex_offset{ backend::mesh->geometry_buffer.vertex_buffer.size() } { }

	void c_geometry::handle() {
		backend::renderer->draw_geometry(vertex_count, index_count, vertex_offset, index_offset);
	}

	void c_geometry::get_bounding_box(vec2_t<float>& min, vec2_t<float>& max) const {
		min = std::numeric_limits<float>::max(); max = std::numeric_limits<float>::min();
		for(const backend::vertex_t& vertex : backend::mesh->geometry_buffer.vertex_buffer | std::views::drop(vertex_offset) | std::views::take(vertex_count)) {
			min = math::min(vertex.pos, min);
			max = math::max(vertex.pos, max);
		}
	}

	void c_geometry::recalculate_uvs(const vec2_t<float>& min, const vec2_t<float>& max) const {
		for(backend::vertex_t& vertex : backend::mesh->geometry_buffer.vertex_buffer | std::views::drop(vertex_offset) | std::views::take(vertex_count)) {
			vertex.uv = std::clamp((vertex.pos - min) / (max - min), { 0.f }, { 1.f });
		}
	}
}