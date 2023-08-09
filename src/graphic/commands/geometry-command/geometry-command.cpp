#include <algorithm>

#include <backend/renderer/renderer.h>
#include <backend/internal/mesh.h>

#include <graphic/commands/geometry-command/geometry-command.h>

namespace null::render {
	c_geometry_command::c_geometry_command()
		: index_offset{ backend::mesh->geometry_buffer.index_buffer.size() }, vertex_offset{ backend::mesh->geometry_buffer.vertex_buffer.size() } { }

	void c_geometry_command::handle() {
		backend::renderer->draw_geometry(vertex_count, index_count, vertex_offset, index_offset);
	}

	void c_geometry_command::get_bounding_box(vec2_t<float>& min, vec2_t<float>& max) const {
		min = std::numeric_limits<float>::max(); max = std::numeric_limits<float>::min();
		for(const backend::vertex_t& vertex : backend::mesh->geometry_buffer.vertex_buffer | std::views::drop(vertex_offset) | std::views::take(vertex_count)) {
			min = math::min(vertex.pos, min);
			max = math::max(vertex.pos, max);
		}
	}

	void c_geometry_command::recalculate_uvs(const vec2_t<float>& min, const vec2_t<float>& max) const {
		const rect_t<float> uvs{ { 0.f }, { 1.f } };
		for(backend::vertex_t& vertex : backend::mesh->geometry_buffer.vertex_buffer | std::views::drop(vertex_offset) | std::views::take(vertex_count)) {
			vertex.uv = std::clamp((vertex.pos - min) / (max - min), uvs.min, uvs.max);
		}
	}

	void c_geometry_command::recalculate_uvs(const vec2_t<float>& min, const vec2_t<float>& max, const rect_t<float>& uvs) const {
		const vec2_t<float> uv_scale{ uvs.size() / (max - min) };
		const std::pair minmax_uv{ std::minmax(uvs.min, uvs.max) };

		for(backend::vertex_t& vertex : backend::mesh->geometry_buffer.vertex_buffer | std::views::drop(vertex_offset) | std::views::take(vertex_count)) {
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
}