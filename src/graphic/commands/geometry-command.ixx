module;
#include <limits>
#include <algorithm>
#include <ranges>
export module null.render:graphic.commands.geometry;

import null.sdk;

import :graphic.command;
import :backend.renderer;
import :backend.renderer.mesh;

export namespace null::render {
	class c_geometry_command : public i_command {
	public:
		size_t index_offset{ }, vertex_offset{ };
		size_t index_count{ }, vertex_count{ };

	public:
		c_geometry_command() : index_offset{ backend::mesh->geometry_buffer.index_buffer.size() }, vertex_offset{ backend::mesh->geometry_buffer.vertex_buffer.size() } { }
		virtual ~c_geometry_command() { }

	public:
		virtual void handle() override {
			backend::renderer->draw_geometry(vertex_count, index_count, vertex_offset, index_offset);
		}

	public:
		void get_bounding_box(vec2_t<float>& min, vec2_t<float>& max) const {
			min = std::numeric_limits<float>::max(); max = std::numeric_limits<float>::min();
			for(const backend::vertex_t& vertex : backend::mesh->geometry_buffer.vertex_buffer | std::views::drop(vertex_offset) | std::views::take(vertex_count)) {
				min = math::min(vertex.pos, min);
				max = math::max(vertex.pos, max);
			}
		}

		void recalculate_uvs(const vec2_t<float>& min, const vec2_t<float>& max) const {
			const rect_t<float> uvs{ { 0.f }, { 1.f } };
			for(backend::vertex_t& vertex : backend::mesh->geometry_buffer.vertex_buffer | std::views::drop(vertex_offset) | std::views::take(vertex_count)) {
				vertex.uv = std::clamp((vertex.pos - min) / (max - min), uvs.min, uvs.max);
			}
		}

		void recalculate_uvs(const vec2_t<float>& min, const vec2_t<float>& max, const rect_t<float>& uvs) const {
			const vec2_t<float> uv_scale{ uvs.size() / (max - min) };
			const std::pair minmax_uv{ std::minmax(uvs.min, uvs.max) };

			for(backend::vertex_t& vertex : backend::mesh->geometry_buffer.vertex_buffer | std::views::drop(vertex_offset) | std::views::take(vertex_count)) {
				vertex.uv = std::clamp(uvs.min + (vertex.pos - min) * uv_scale, minmax_uv.first, minmax_uv.second);
			}
		}

		void set_uvs(const rect_t<float>& uvs) const {
			vec2_t<float> min{ }, max{ };
			get_bounding_box(min, max);
			recalculate_uvs(min, max, uvs);
		}

		void set_default_uvs() const {
			vec2_t<float> min{ }, max{ };
			get_bounding_box(min, max);
			recalculate_uvs(min, max);
		}
	};
}
