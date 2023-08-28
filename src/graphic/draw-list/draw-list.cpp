#include "backend/internal/mesh.h"
#include "graphic/commands/geometry-command/geometry-command.h"

#include "graphic/draw-list/draw-list.h"

namespace null::render {
	void c_draw_list::add_poly_line(const std::vector<vec2_t<float>>& points, const stroke_t& stroke, const brush_t& brush, const pen_t& pen) {
		if(points.size() < 2) return;
		std::unique_ptr<c_geometry_command> command{ std::make_unique<c_geometry_command>() };

		const bool have_pen{ stroke.line_join != e_line_join::none && pen.brush };
		std::vector<backend::index_t> outward_order{ }, inward_order{ };

		const float half_thickness{ stroke.thickness / 2.f };
		const float outward_thickness{ stroke.thickness * stroke.origin };
		const float inward_thickness{ stroke.thickness * (1.f - stroke.origin) };

		std::vector<stroke_t::segment_t> segments{ };
		stroke.build_segments(segments, points);
		for(const stroke_t::segment_t& segment : segments) {
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
				const vec2_t<float> outward_delta{ math::invert_vector_axis(direction, math::e_rotation::ccw) * outward_thickness };
				const vec2_t<float> inward_delta{ math::invert_vector_axis(direction, math::e_rotation::cw) * inward_thickness };

				const vec2_t<float> outward_vertex{ *segment.begin_edge->point + outward_delta + cap_direction * (stroke.line_cap == e_line_cap::square ? outward_thickness : 0.f) };
				const vec2_t<float> inward_vertex{ *segment.begin_edge->point + inward_delta + cap_direction * (stroke.line_cap == e_line_cap::square ? outward_thickness : 0.f) };

				command->vertex_count += 2;
				backend::mesh->geometry_buffer
					.add_vertex({ outward_vertex, { }, brush.color })
					.add_vertex({ inward_vertex, { }, brush.color });
			} else {
				if(stroke.line_join == e_line_join::bevel) {
					vec2_t<float> distance{ segment.begin_edge->get_mitter_offset(half_thickness) };
					const math::e_rotation rotation{ segment.begin_edge->inversed ? math::e_rotation::cw : math::e_rotation::ccw };
					if(!segment.begin_edge->inversed)
						distance *= -1.f;

					vec2_t<float> origin_offset{ };
					if(stroke.origin != 0.5f)
						origin_offset = segment.begin_edge->get_mitter_offset(inward_thickness > outward_thickness ? (outward_thickness - half_thickness) : (half_thickness - inward_thickness));

					command->index_count += 3;
					backend::mesh->geometry_buffer.add_index(command->vertex_count).add_index(command->vertex_count + 1).add_index(command->vertex_count + 2);

					const vec2_t<float> point{ *segment.begin_edge->point + origin_offset };
					const vec2_t<float> miter_vertex{ point + distance };
					const vec2_t<float> to_previous_vertex{ point + math::invert_vector_axis(segment.begin_edge->from_previous_direction, rotation) * half_thickness };
					const vec2_t<float> to_next_vertex{ point + math::invert_vector_axis(segment.begin_edge->to_next_direction, rotation) * half_thickness };

					command->vertex_count += 3;
					backend::mesh->geometry_buffer
						.add_vertex({ miter_vertex, { }, brush.color })
						.add_vertex({ to_previous_vertex, { }, brush.color })
						.add_vertex({ to_next_vertex, { }, brush.color });
				} else if(stroke.line_join == e_line_join::miter) {
					const vec2_t<float> outward_vertex{ *segment.begin_edge->point + segment.begin_edge->get_mitter_offset(outward_thickness) };
					const vec2_t<float> inward_vertex{ *segment.begin_edge->point - segment.begin_edge->get_mitter_offset(inward_thickness) };

					command->vertex_count += 2;
					backend::mesh->geometry_buffer
						.add_vertex({ outward_vertex, { }, brush.color })
						.add_vertex({ inward_vertex, { }, brush.color });
				} else {
					const vec2_t<float> outward_vertex{ *segment.begin_edge->point + math::invert_vector_axis(segment.begin_edge->to_next_direction, math::e_rotation::ccw) * outward_thickness };
					const vec2_t<float> inward_vertex{ *segment.begin_edge->point + math::invert_vector_axis(segment.begin_edge->to_next_direction, math::e_rotation::cw) * inward_thickness };

					command->vertex_count += 2;
					backend::mesh->geometry_buffer
						.add_vertex({ outward_vertex, { }, brush.color })
						.add_vertex({ inward_vertex, { }, brush.color });
				}
			}

			//@note: in e_line_join::none each segment itself creates an end in addition to the beginning, the check is needed so that e_line_cap is displayed correctly
			if(stroke.line_join == e_line_join::none && ((!segment.is_last && *segment.end_edge->point != *std::prev(points.end())) || stroke.line_cap == e_line_cap::joint)) {
				const vec2_t<float> outward_vertex{ *segment.end_edge->point + math::invert_vector_axis(segment.begin_edge->to_next_direction, math::e_rotation::ccw) * outward_thickness };
				const vec2_t<float> inward_vertex{ *segment.end_edge->point + math::invert_vector_axis(segment.begin_edge->to_next_direction, math::e_rotation::cw) * inward_thickness };

				command->vertex_count += 2;
				backend::mesh->geometry_buffer
					.add_vertex({ outward_vertex, { }, brush.color })
					.add_vertex({ inward_vertex, { }, brush.color });
			}
		}

		std::unique_ptr<i_command> pen_command{ };
		if(have_pen) pen_command = std::move(pen.around_stroke(command, outward_order, inward_order, stroke.line_cap == e_line_cap::joint));
		if(have_pen && pen.layer == e_pen_layer::background) add_command(std::move(pen_command));
		add_command(brush.prepare_command(command));
		if(have_pen && pen.layer == e_pen_layer::foreground) add_command(std::move(pen_command));
	}

	void c_draw_list::add_convex_shape(const std::vector<vec2_t<float>>& points, const brush_t& brush, const pen_t& pen) {
		if(points.size() < 3) return;
		std::unique_ptr<c_geometry_command> command{ std::make_unique<c_geometry_command>() };

		command->index_count += (points.size() - 2) * 3;
		for(const int& i : std::views::iota(2u, points.size()))
			backend::mesh->geometry_buffer.add_index(0).add_index(i - 1).add_index(i);

		command->vertex_count += points.size();
		for(const vec2_t<float>& current_point : points) {
			backend::mesh->geometry_buffer.add_vertex({ current_point, { }, brush.color });
		}

		std::unique_ptr<i_command> pen_command{ };
		if(pen.brush) pen_command = std::move(pen.around_convex_shape(command));
		if(pen.brush && pen.layer == e_pen_layer::background) add_command(std::move(pen_command));
		add_command(brush.prepare_command(command));
		if(pen.brush && pen.layer == e_pen_layer::foreground) add_command(std::move(pen_command));
	}
}