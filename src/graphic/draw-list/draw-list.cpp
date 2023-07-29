#include <backend/internal/mesh.h>
#include <graphic/commands/geometry-command/geometry-command.h>

#include <graphic/draw-list/draw-list.h>

namespace null::render {
	void c_draw_list::add_poly_line(const std::vector<vec2_t<float>>& points, const stroke_t& stroke, const brush_t& brush, const pen_t& pen) {
		if(points.size() < 2) return;
		std::unique_ptr<commands::c_geometry> command{ std::make_unique<commands::c_geometry>() };

		const bool have_pen{ stroke.line_join != e_line_join::none && pen.brush };
		std::vector<backend::index_t> outward_order{ }, inward_order{ };

		const float half_thickness{ stroke.thickness / 2.f };
		for(const stroke_t::segment_t& segment : stroke.build_segments(points)) {
			if(have_pen) {
				outward_order.push_back(command->vertex_count + segment.begin_edge->outward_begin);
				if(segment.begin_edge->outward_begin != segment.begin_edge->outward_end)
					outward_order.push_back(command->vertex_count + segment.begin_edge->outward_end);

				inward_order.push_back(command->vertex_count + segment.begin_edge->inward_begin);
				if(segment.begin_edge->inward_begin != segment.begin_edge->inward_end)
					inward_order.push_back(command->vertex_count + segment.begin_edge->inward_end);
			}

			if(!(stroke.line_cap != e_line_cap::joint && segment.is_last)) {
				const size_t next_edge_offset{ segment.is_last && stroke.line_join != e_line_join::none ? 0u : command->vertex_count + segment.begin_edge->join_size };

				command->index_count += 6;
				backend::mesh->geometry_buffer
					.add_index(command->vertex_count + segment.begin_edge->outward_end).add_index(next_edge_offset + segment.end_edge->outward_begin).add_index(next_edge_offset + segment.end_edge->inward_begin)
					.add_index(command->vertex_count + segment.begin_edge->outward_end).add_index(next_edge_offset + segment.end_edge->inward_begin).add_index(command->vertex_count + segment.begin_edge->inward_end);
			}

			if(stroke.line_cap != e_line_cap::joint && (segment.is_first || segment.is_last)) {
				const vec2_t<float>& cap_direction{ segment.is_first ? -segment.begin_edge->to_next_direction : segment.begin_edge->from_previous_direction };
				
				const vec2_t<float>& direction{ segment.is_first ? segment.begin_edge->to_next_direction : segment.begin_edge->from_previous_direction };
				const vec2_t<float> outward_delta{ math::invert_vector_axis(direction, math::e_rotation::ccw) };
				const vec2_t<float> inward_delta{ math::invert_vector_axis(direction, math::e_rotation::cw) };

				const vec2_t<float> outward_vertex{ *segment.begin_edge->point + outward_delta * half_thickness + cap_direction * (stroke.line_cap == e_line_cap::square ? half_thickness : 0.f) };
				const vec2_t<float> inward_vertex{ *segment.begin_edge->point + inward_delta * half_thickness + cap_direction * (stroke.line_cap == e_line_cap::square ? half_thickness : 0.f) };

				command->vertex_count += 2;
				backend::mesh->geometry_buffer
					.add_vertex({ outward_vertex, { }, brush.color })
					.add_vertex({ inward_vertex, { }, brush.color });
			} else {
				vec2_t<float> distance{ segment.begin_edge->normal * std::min(half_thickness / segment.begin_edge->miter_angle.cos(), (double)segment.begin_edge->max_miter_dist) };
				if(stroke.line_join == e_line_join::bevel) {
					const math::e_rotation rotation{ segment.begin_edge->inversed ? math::e_rotation::cw : math::e_rotation::ccw };
					if(!segment.begin_edge->inversed)
						distance *= -1.f;

					command->index_count += 3;
					backend::mesh->geometry_buffer.add_index(command->vertex_count).add_index(command->vertex_count + 1).add_index(command->vertex_count + 2);

					const vec2_t<float> miter_vertex{ *segment.begin_edge->point + distance };
					const vec2_t<float> to_previous_vertex{ *segment.begin_edge->point + math::invert_vector_axis(segment.begin_edge->from_previous_direction, rotation) * half_thickness };
					const vec2_t<float> to_next_vertex{ *segment.begin_edge->point + math::invert_vector_axis(segment.begin_edge->to_next_direction, rotation) * half_thickness };

					command->vertex_count += 3;
					backend::mesh->geometry_buffer
						.add_vertex({ miter_vertex, { }, brush.color })
						.add_vertex({ to_previous_vertex, { }, brush.color })
						.add_vertex({ to_next_vertex, { }, brush.color });
				} else if(stroke.line_join == e_line_join::miter) {
					const vec2_t<float> outward_vertex{ *segment.begin_edge->point + distance };
					const vec2_t<float> inward_vertex{ *segment.begin_edge->point - distance };

					command->vertex_count += 2;
					backend::mesh->geometry_buffer
						.add_vertex({ outward_vertex, { }, brush.color })
						.add_vertex({ inward_vertex, { }, brush.color });
				} else {
					vec2_t<float> direction{ segment.begin_edge->to_next_direction * half_thickness };

					const vec2_t<float> outward_vertex{ *segment.begin_edge->point + math::invert_vector_axis(segment.begin_edge->to_next_direction, math::e_rotation::ccw) * half_thickness };
					const vec2_t<float> inward_vertex{ *segment.begin_edge->point + math::invert_vector_axis(segment.begin_edge->to_next_direction, math::e_rotation::cw) * half_thickness };

					command->vertex_count += 2;
					backend::mesh->geometry_buffer
						.add_vertex({ outward_vertex, { }, brush.color })
						.add_vertex({ inward_vertex, { }, brush.color });
				}
			}

			if(stroke.line_join == e_line_join::none) {
				const vec2_t<float> outward_vertex{ *segment.end_edge->point + math::invert_vector_axis(segment.begin_edge->to_next_direction, math::e_rotation::ccw) * half_thickness };
				const vec2_t<float> inward_vertex{ *segment.end_edge->point + math::invert_vector_axis(segment.begin_edge->to_next_direction, math::e_rotation::cw) * half_thickness };

				command->vertex_count += 2;
				backend::mesh->geometry_buffer
					.add_vertex({ outward_vertex, { }, brush.color })
					.add_vertex({ inward_vertex, { }, brush.color });
			}
		}

		std::unique_ptr<commands::i_command> pen_command{ };
		if(have_pen) pen_command = std::move(pen.around_stroke(command, outward_order, inward_order, stroke.line_cap == e_line_cap::joint));
		if(have_pen && pen.layer == e_pen_layer::background) add_command(std::move(pen_command));
		add_command(brush.prepare_command(command));
		if(have_pen && pen.layer == e_pen_layer::foreground) add_command(std::move(pen_command));
	}

	void c_draw_list::add_convex_shape(const std::vector<vec2_t<float>>& points, const brush_t& brush, const pen_t& pen) {
		if(points.size() < 3) return;
		std::unique_ptr<commands::c_geometry> command{ std::make_unique<commands::c_geometry>() };

		command->index_count += (points.size() - 2) * 3;
		for(const int& i : std::views::iota(2u, points.size()))
			backend::mesh->geometry_buffer.add_index(0).add_index(i - 1).add_index(i);

		command->vertex_count += points.size();
		for(const vec2_t<float>& current_point : points) {
			backend::mesh->geometry_buffer.add_vertex({ current_point, { }, brush.color });
		}

		std::unique_ptr<commands::i_command> pen_command{ };
		if(pen.brush) pen_command = std::move(pen.around_convex_shape(command));
		if(pen.brush && pen.layer == e_pen_layer::background) add_command(std::move(pen_command));
		add_command(brush.prepare_command(command));
		if(pen.brush && pen.layer == e_pen_layer::foreground) add_command(std::move(pen_command));
	}
}