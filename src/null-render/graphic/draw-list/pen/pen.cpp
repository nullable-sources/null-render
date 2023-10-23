#include "null-render/backend/internal/mesh.h"
#include "../strokes/stroke.h"
#include "pen.h"

namespace null::render {
	std::unique_ptr<i_command> pen_t::around_convex_shape(const std::unique_ptr<c_geometry_command>& command) const {
		std::unique_ptr<c_geometry_command> pen_command = std::make_unique<c_geometry_command>();

		for(size_t i : std::views::iota(0u, command->vertex_count)) {
			const size_t previous_i = (i + command->vertex_count - 1) % command->vertex_count;
			const size_t next_i = (i + 1) % command->vertex_count;

			make_geometry(pen_command, command, previous_i, i, next_i, math::e_rotation::ccw, i == command->vertex_count - 1, 0);
		}

		return brush->prepare_command(pen_command);
	}

	std::unique_ptr<i_command> pen_t::around_stroke(const std::unique_ptr<c_geometry_command>& command, const std::vector<backend::index_t>& outward_order, const std::vector<backend::index_t>& inward_order, bool closed) const {
		std::unique_ptr<c_geometry_command> pen_command = std::make_unique<c_geometry_command>();

		if(closed) {
			around_order(pen_command, command, outward_order, math::e_rotation::ccw);
			around_order(pen_command, command, inward_order, math::e_rotation::cw, pen_command->vertex_count);
		} else {
			std::vector<backend::index_t> order = outward_order;
			std::ranges::move(inward_order | std::views::reverse, std::back_inserter(order));
			around_order(pen_command, command, order, math::e_rotation::ccw);
		}

		return brush->prepare_command(pen_command);
	}

	void pen_t::around_order(std::unique_ptr<c_geometry_command>& pen_command, const std::unique_ptr<c_geometry_command>& command, const std::vector<backend::index_t>& order, math::e_rotation rotation, backend::index_t first_vertex_offset) const {
		for(size_t i : std::views::iota(0u, order.size())) {
			const size_t previous_i = (i + order.size() - 1) % order.size();
			const size_t next_i = (i + 1) % order.size();

			make_geometry(pen_command, command, order[previous_i], order[i], order[next_i], rotation, i == order.size() - 1, first_vertex_offset);
		}
	}

	void pen_t::make_geometry(std::unique_ptr<c_geometry_command>& pen_command, const std::unique_ptr<c_geometry_command>& command, backend::index_t previous, backend::index_t current, backend::index_t next, math::e_rotation rotation, bool is_last, backend::index_t first_vertex_offset) const {
		const backend::vertex_t& previous_vertex = backend::mesh->geometry_buffer.vertex_buffer[command->vertex_offset + previous];
		const backend::vertex_t& current_vertex = backend::mesh->geometry_buffer.vertex_buffer[command->vertex_offset + current];
		const backend::vertex_t& next_vertex = backend::mesh->geometry_buffer.vertex_buffer[command->vertex_offset + next];

		const size_t next_vertex_offset = is_last ? first_vertex_offset : pen_command->vertex_count + 2;
		pen_command->index_count += 6;
		backend::mesh->geometry_buffer
			.add_index(pen_command->vertex_count).add_index(next_vertex_offset).add_index(next_vertex_offset + 1)
			.add_index(pen_command->vertex_count).add_index(next_vertex_offset + 1).add_index(pen_command->vertex_count + 1);

		stroke_t::segment_t::i_edge current_edge{ };
		current_edge.from_points(previous_vertex.pos, current_vertex.pos, next_vertex.pos, rotation);
		if(edge == e_pen_edge::miter) current_edge.miter_from_points(previous_vertex.pos, current_vertex.pos, next_vertex.pos);

		const float half_thickness = thickness / 2.f, thickness_offset = origin * 2.f;
		const float outward_thickness = half_thickness * thickness_offset, inward_thickness = half_thickness * (2.f - thickness_offset);
		const vec2_t<float> outward_tessellation = edge == e_pen_edge::miter ? current_edge.get_mitter_offset(outward_thickness) : current_edge.normal * outward_thickness;
		const vec2_t<float> inward_tessellation = edge == e_pen_edge::miter ? current_edge.get_mitter_offset(inward_thickness) : current_edge.normal * inward_thickness;

		pen_command->vertex_count += 2;
		backend::mesh->geometry_buffer
			.add_vertex(current_vertex.pos + outward_tessellation, { }, brush->color)
			.add_vertex(current_vertex.pos - inward_tessellation, { }, brush->color);
	}
}
