module;
#include <memory>
#include <vector>
#include <ranges>
#include <algorithm>
#include <complex>
export module null.render:graphic.draw_list.stroke;

import null.sdk;

export namespace null::render {
	enum class e_line_join {
		miter,
		bevel,
		none
	};

	enum class e_line_cap {
		butt,
		square,
		joint
	};

	struct stroke_t {
	public:
		struct segment_t {
		public:
			struct edge_t {
			public:
				const vec2_t<float>* point{ };
				size_t outward_begin{ }, outward_end{ };
				size_t inward_begin{ }, inward_end{ };
				size_t join_size{ };

				vec2_t<float> normal{ }, from_previous_direction{ }, to_next_direction{ };
				angle_t<radians_t> miter_angle{ };
				double half_miter_cos{ };
				float max_miter_dist{ };
				bool inversed{ };
			};

		public:
			std::shared_ptr<edge_t> begin_edge{ }, end_edge{ };
			bool is_first{ }, is_last{ };
		};

	public:
		float thickness{ 1.f };
		e_line_join line_join{ };
		e_line_cap line_cap{ };

	public: //@todo: P0847
		auto& set_thickness(const float& _thickness) { thickness = std::max(_thickness, 1.f); return *this; }
		auto& set_join(const e_line_join& _line_join) { line_join = _line_join; return *this; }
		auto& set_cap(const e_line_cap& _line_cap) { line_cap = _line_cap; return *this; }

	public:
		std::vector<segment_t> build_segments(const std::vector<vec2_t<float>>& points) const {
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

				begin_edge->point = &current_point;

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
						}
						else {
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
	};
}