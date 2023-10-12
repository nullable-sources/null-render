#include <memory>
#include "stroke.h"

namespace null::render {
	void stroke_t::segment_t::i_edge::from_points(const vec2_t<float>& previous_point, const vec2_t<float>& current_point, const vec2_t<float>& next_point, math::e_rotation normal_rotation) {
		to_next_direction = current_point.direction(next_point);
		from_previous_direction = previous_point.direction(current_point);
		normal = vectors_bisector(to_next_direction, from_previous_direction, normal_rotation);
	}

	void stroke_t::segment_t::i_edge::miter_from_points(const vec2_t<float>& previous_point, const vec2_t<float>& current_point, const vec2_t<float>& next_point) {
		miter_angle = angle_t<radians_t>{ math::angle_between(
			invert_vector_axis(to_next_direction, math::e_rotation::cw),
			invert_vector_axis(from_previous_direction, math::e_rotation::cw)
			) }.normalized() / 2.;
		max_miter_dist = std::abs(std::max(vec2_t<float>{ current_point - next_point }.length(), vec2_t<float>{ current_point - previous_point }.length()) / miter_angle.sin());
	}

	void stroke_t::build_segments(std::vector<segment_t>& segments, const std::vector<vec2_t<float>>& points) const {
		const size_t segments_count{ points.size() };

		for(size_t i : std::views::iota(0u, segments_count)) {
			const size_t previous_i{ (i + segments_count - 1) % segments_count };
			const size_t next_i{ (i + 1) % segments_count };

			const bool is_first{ i == 0u }, is_last{ i == segments_count - 1 };

			const vec2_t<float>& previous_point{ points[previous_i] };
			const vec2_t<float>& current_point{ points[i] };
			const vec2_t<float>& next_point{ points[next_i] };

			std::shared_ptr<segment_t::edge_t> begin_edge{ }, end_edge{ };
			if(!segments.empty()) begin_edge = segments.back().end_edge;
			else begin_edge = std::make_shared<segment_t::edge_t>();

			if(is_last) end_edge = segments.front().begin_edge;
			else end_edge = std::make_shared<segment_t::edge_t>();

			begin_edge->from_points(previous_point, current_point, next_point);
			if(line_join != e_line_join::none) begin_edge->miter_from_points(previous_point, current_point, next_point);

			begin_edge->point = &points[i];
			if(line_join == e_line_join::bevel)
				begin_edge->inversed = angle_t<radians_t>{ math::angle_between<float>(begin_edge->normal, begin_edge->to_next_direction) }.normalized() < angle_t<radians_t>{ 90.f };

			switch(line_join) {
				case e_line_join::none: {
					begin_edge->join_size = 0;
					begin_edge->outward_begin = 0;
					begin_edge->inward_begin = 1;

					begin_edge->outward_end = 2;
					begin_edge->inward_end = 3;
				}
				break;
				case e_line_join::miter: {
					begin_edge->join_size = 2;
					begin_edge->outward_begin = begin_edge->outward_end = 0;
					begin_edge->inward_begin = begin_edge->inward_end = 1;
				}
				break;
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
				}
				break;
			}

			if(line_cap != e_line_cap::joint && (is_first || is_last)) {
				begin_edge->join_size = 2;
				begin_edge->outward_begin = begin_edge->outward_end = 0;
				begin_edge->inward_begin = begin_edge->inward_end = 1;
			}

			segments.push_back({ begin_edge, end_edge, is_first, is_last });
		}
	}
}
