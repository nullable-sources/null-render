module;
#include <memory>
#include <vector>
#include <ranges>
#include <algorithm>
export module null.render:graphic.draw_list.pen;

import null.sdk;

import :graphic.command;
import :graphic.commands.geometry;
import :graphic.draw_list.brush;
import :backend.renderer.geometry_buffer;
import :backend.renderer.mesh;

export namespace null::render {
	enum class e_pen_layer {
		foreground, //@note: drawing over original geometry
		background //@note: drawing under original geometry
	};

	struct pen_t {
	public:
		std::shared_ptr<brush_t> brush{ };
		float thickness{ 1.f };
		float origin{ 1.f }; //@note: offset from geometry start, 0.f -> inside the geometry, 1.f -> outside the geometry
		e_pen_layer layer{ };

	public: //@todo: P0847
		template <typename brush_type_t> auto& set_brush(const brush_type_t& _brush) { brush = std::make_shared<brush_type_t>(_brush); return *this; }
		auto& set_thickness(float _thickness) { thickness = _thickness; return *this; }
		auto& set_origin(float _origin) { origin = _origin; return *this; }
		auto& set_layer(e_pen_layer _layer) { layer = _layer; return *this; }

	public:
		std::unique_ptr<i_command> around_convex_shape(const std::unique_ptr<c_geometry_command>& command) const {
			std::unique_ptr<c_geometry_command> pen_command{ std::make_unique<c_geometry_command>() };

			for(const size_t& i : std::views::iota(0u, command->vertex_count)) {
				const size_t previous_i{ (i + command->vertex_count - 1) % command->vertex_count };
				const size_t next_i{ (i + 1) % command->vertex_count };

				make_geometry(pen_command, command, previous_i, i, next_i, math::e_rotation::ccw, i == command->vertex_count - 1, 0);
			}

			return brush->prepare_command(pen_command);
		}

		std::unique_ptr<i_command> around_stroke(const std::unique_ptr<c_geometry_command>& command, const std::vector<backend::index_t>& outward_order, const std::vector<backend::index_t>& inward_order, bool closed) const {
			std::unique_ptr<c_geometry_command> pen_command{ std::make_unique<c_geometry_command>() };

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

	private:
		void around_order(std::unique_ptr<c_geometry_command>& pen_command, const std::unique_ptr<c_geometry_command>& command, const std::vector<backend::index_t>& order, math::e_rotation rotation, backend::index_t first_vertex_offset = { }) const {
			for(const size_t& i : std::views::iota(0u, order.size())) {
				const size_t previous_i{ (i + order.size() - 1) % order.size() };
				const size_t next_i{ (i + 1) % order.size() };

				make_geometry(pen_command, command, order[previous_i], order[i], order[next_i], rotation, i == order.size() - 1, first_vertex_offset);
			}
		}

		void make_geometry(std::unique_ptr<c_geometry_command>& pen_command, const std::unique_ptr<c_geometry_command>& command, backend::index_t previous, backend::index_t current, backend::index_t next, math::e_rotation rotation, bool is_last, backend::index_t first_vertex_offset) const {
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

			pen_command->vertex_count += 2;
			backend::mesh->geometry_buffer
				.add_vertex({ current_vertex.pos + outward_tesselation, { }, brush->color })
				.add_vertex({ current_vertex.pos - inward_tesselation, { }, brush->color });
		}
	};
}