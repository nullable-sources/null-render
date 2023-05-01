#include <graphic/draw-list/pen/pen.h>
#include <backend/internal/mesh.h>

namespace null::render {
	std::unique_ptr<commands::i_command> pen_t::around_convex_shape(const std::unique_ptr<commands::c_geometry>& command) const {
		std::unique_ptr<commands::c_geometry> pen_command{ std::make_unique<commands::c_geometry>() };

		for(const size_t& i : std::views::iota(0u, command->vertex_count)) {
			const size_t previous_i{ (i + command->vertex_count - 1) % command->vertex_count };
			const size_t next_i{ (i + 1) % command->vertex_count };

			make_geometry(pen_command, command, previous_i, i, next_i, math::e_rotation::ccw, i == command->vertex_count - 1, 0);
		}

		return brush->prepare_command(pen_command);
	}

	std::unique_ptr<commands::i_command> pen_t::around_stroke(const std::unique_ptr<commands::c_geometry>& command, const std::vector<backend::index_t>& outward_order, const std::vector<backend::index_t>& inward_order, const bool& closed) const {
		std::unique_ptr<commands::c_geometry> pen_command{ std::make_unique<commands::c_geometry>() };

		if(closed) {
			around_order(pen_command, command, outward_order, math::e_rotation::ccw);
			around_order(pen_command, command, inward_order, math::e_rotation::cw, pen_command->vertex_count);
		} else {
			std::vector<backend::index_t> order{ outward_order };
			std::ranges::move(inward_order | std::views::reverse, std::back_inserter(order));
			around_order(pen_command, command, order, math::e_rotation::ccw);
		}

		return brush->prepare_command(pen_command);
	}

	void pen_t::around_order(std::unique_ptr<commands::c_geometry>& pen_command, const std::unique_ptr<commands::c_geometry>& command, const std::vector<backend::index_t>& order, const math::e_rotation& rotation, backend::index_t first_vertex_offset) const {
		for(const size_t& i : std::views::iota(0u, order.size())) {
			const size_t previous_i{ (i + order.size() - 1) % order.size() };
			const size_t next_i{ (i + 1) % order.size() };

			make_geometry(pen_command, command, order[previous_i], order[i], order[next_i], rotation, i == order.size() - 1, first_vertex_offset);
		}
	}

	void pen_t::make_geometry(std::unique_ptr<commands::c_geometry>& pen_command, const std::unique_ptr<commands::c_geometry>& command, const backend::index_t& previous, const backend::index_t& current, const backend::index_t& next, const math::e_rotation& rotation, const bool& is_last, const backend::index_t& first_vertex_offset) const {
		const backend::vertex_t& previous_vertex{ backend::mesh->geometry_buffer.vertex_buffer[command->vertex_offset + previous] };
		const backend::vertex_t& current_vertex{ backend::mesh->geometry_buffer.vertex_buffer[command->vertex_offset + current] };
		const backend::vertex_t& next_vertex{ backend::mesh->geometry_buffer.vertex_buffer[command->vertex_offset + next] };

		const vec2_t<float> to_next_direction{ current_vertex.pos.direction(next_vertex.pos) };
		const vec2_t<float> from_previous_direction{ previous_vertex.pos.direction(current_vertex.pos) };
		const vec2_t<float> normal{ math::vectors_bisector(to_next_direction, from_previous_direction, rotation) };

		const size_t next_vertex_offset{ is_last ? first_vertex_offset : pen_command->vertex_count + 2 };
		pen_command->index_count += 6;
		backend::mesh->geometry_buffer
			.add_index(pen_command->vertex_count).add_index(next_vertex_offset).add_index(next_vertex_offset + 1)
			.add_index(pen_command->vertex_count).add_index(next_vertex_offset + 1).add_index(pen_command->vertex_count + 1);

		const float half_thickness{ thickness / 2.f }, thickness_offset{ origin * 2.f };
		const vec2_t<float> outward_tesselation{ normal * (half_thickness * thickness_offset) };
		const vec2_t<float> inward_tesselation{ normal * (half_thickness * (2.f - thickness_offset)) };

		const bool to_next_round{ std::abs(to_next_direction.x) == 1.f || std::abs(to_next_direction.y) == 1.f };
		const bool from_previous_round{ std::abs(from_previous_direction.x) == 1.f || std::abs(from_previous_direction.y) == 1.f };
		pen_command->vertex_count += 2;
		backend::mesh->geometry_buffer
			.add_vertex({ current_vertex.pos + (to_next_round || from_previous_round ? math::round(outward_tesselation) : outward_tesselation), { }, brush->color })
			.add_vertex({ current_vertex.pos - (to_next_round || from_previous_round ? math::round(inward_tesselation) : inward_tesselation), { }, brush->color });
	}
}