#include <graphic/filters/quad-gradient/quad-gradient.h>
#include <backend/internal/mesh.h>

#include <graphic/draw-list/brushes/quad-gradient-brush/quad-gradient-brush.h>

namespace null::render {
	std::unique_ptr<commands::i_command> quad_gradient_brush_t::prepare_command(std::unique_ptr<commands::c_geometry>& command) const {
		vec2_t min{ std::numeric_limits<float>::max() }, max{ std::numeric_limits<float>::min() };
		for(const backend::vertex_t& vertex : backend::mesh->geometry_buffer.vertex_buffer | std::views::drop(command->vertex_offset) | std::views::take(command->vertex_count)) {
			min = math::min(vertex.pos, min);
			max = math::max(vertex.pos, max);
		}

		for(backend::vertex_t& vertex : backend::mesh->geometry_buffer.vertex_buffer | std::views::drop(command->vertex_offset) | std::views::take(command->vertex_count)) {
			vertex.uv = std::clamp((vertex.pos - min) / (max - min), { 0.f }, { 1.f });
		}

		std::unique_ptr<filters::c_quad_gradient> filter{ std::make_unique<filters::c_quad_gradient>(colors) };
		filter->child_command = std::move(command);
		return std::move(filter);
	}
}