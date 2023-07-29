#include <memory>
#include <graphic/draw-list/strokes/stroke.h>

namespace null::render {
	std::vector<stroke_t::segment_t> stroke_t::build_segments(const std::vector<vec2_t<float>>& points) const {
		std::vector<segment_t> segments{ };
		const size_t segments_count{ points.size() };

		for(const size_t& i : std::views::iota(0u, segments_count)) {
			const size_t previous_i{ (i + points.size() - 1) % points.size() };
			const size_t next_i{ (i + 1) % points.size() };

			const bool is_first{ i == 0u }, is_last{ i == segments_count - 1 };

			const vec2_t<float>& previous_point{ points[previous_i] };
			const vec2_t<float>& current_point{ points[i] };
			const vec2_t<float>& next_point{ points[next_i] };

			std::shared_ptr<segment_t::edge_t> begin_edge{ }, end_edge{ };
			if(!segments.empty()) begin_edge = segments.back().end_edge;
			else begin_edge = std::make_shared<segment_t::edge_t>();

			if(is_last) end_edge = segments.front().begin_edge;
			else end_edge = std::make_shared<segment_t::edge_t>();

			begin_edge->point = &points[i];

			begin_edge->to_next_direction = current_point.direction(next_point);
			begin_edge->from_previous_direction = previous_point.direction(current_point);
			begin_edge->normal = math::vectors_bisector(begin_edge->to_next_direction, begin_edge->from_previous_direction);

			begin_edge->miter_angle = angle_t<radians_t>{ math::angle_between(
				math::invert_vector_axis(begin_edge->to_next_direction, math::e_rotation::cw),
				math::invert_vector_axis(begin_edge->from_previous_direction, math::e_rotation::cw)
			) }.normalized() / 2.;
			begin_edge->max_miter_dist = std::abs(std::max(vec2_t{ current_point - next_point }.length(), vec2_t{ current_point - previous_point }.length()) / begin_edge->miter_angle.sin());

			begin_edge->inversed = angle_t<radians_t>{ math::angle_between<float>(begin_edge->normal, begin_edge->to_next_direction) }.normalized() < angle_t<radians_t>{ 90.f };

			switch(line_join) {
				case e_line_join::none: {
					begin_edge->join_size = 0;
					begin_edge->outward_begin = 0;
					begin_edge->inward_begin = 1;

					begin_edge->outward_end = 2;
					begin_edge->inward_end = 3;
				} break;
				case e_line_join::miter: {
					begin_edge->join_size = 2;
					begin_edge->outward_begin = begin_edge->outward_end = 0;
					begin_edge->inward_begin = begin_edge->inward_end = 1;
				} break;
				case e_line_join::bevel: {
					begin_edge->join_size = 3;
					if(begin_edge->inversed) {
						begin_edge->outward_begin = begin_edge->outward_end = 0;
						begin_edge->inward_begin = 1;
						begin_edge->inward_end = 2;
					} else {
						begin_edge->inward_begin = begin_edge->inward_end = 0;
						begin_edge->outward_begin = 1;
						begin_edge->outward_end = 2;
					}
				} break;
			}

			if(line_cap != e_line_cap::joint && (is_first || is_last)) {
				begin_edge->join_size = 2;
				begin_edge->outward_begin = begin_edge->outward_end = 0;
				begin_edge->inward_begin = begin_edge->inward_end = 1;
			}

			segments.push_back({ begin_edge, end_edge, is_first, is_last });
		}

		return segments;
	}
}