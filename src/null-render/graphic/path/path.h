#pragma once
#include <micro-tess/dynamic_array.h>
#include <micro-tess/elliptic_arc_divider.h>
#include <micro-tess/chunker.h>
#include <micro-tess/std_rebind_allocator.h>

#include <null-sdk.h>

namespace null::render {
	enum class e_fill_rule { non_zero, even_odd };
	enum class e_tessellation_quality { constant_memory, fine, better, extra_vertices };

	struct path_t {
	public:
		using chunker_t = microtess::allocator_aware_chunker<vec2_t<float>, microtess::dynamic_array>;

	public:
		chunker_t paths_vertices{ };

		e_fill_rule fill_rule{ e_fill_rule::non_zero };
		e_tessellation_quality quality{ e_tessellation_quality::better };
		e_line_cap line_cap{ e_line_cap::butt };
		e_line_join line_join{ e_line_join::bevel };
		float stroke_width{ 1.f };
		float miter_limit{ 4.f };
		std::array<int, 2> dash_pattern{ };
		float dash_offset{ };

	public:
		path_t() { }

	private:
		vec2_t<float> curr_subpath_last_point() const {
			auto current_path = paths_vertices.back();
			return current_path[current_path.size() - 1];
		}

	public:
		path_t& set_fill_rule(this path_t& self, e_fill_rule fill_rule) { self.fill_rule = fill_rule; return self; }
		path_t& set_quality(this path_t& self, e_tessellation_quality quality) { self.quality = quality; return self; }
		path_t& set_line_cap(this path_t& self, e_line_cap line_cap) { self.line_cap = line_cap; return self; }
		path_t& set_line_join(this path_t& self, e_line_join line_join) { self.line_join = line_join; return self; }
		path_t& set_stroke_width(this path_t& self, float stroke_width) { self.stroke_width = stroke_width; return self; }
		path_t& set_miter_limit(this path_t& self, float miter_limit) { self.miter_limit = miter_limit; return self; }
		path_t& set_dash_pattern(this path_t& self, const std::array<int, 2>& dash_pattern) { self.dash_pattern = dash_pattern; return self; }
		path_t& set_dash_offset(this path_t& self, float dash_offset) { self.dash_offset = dash_offset; return self; }

	public:
		path_t& clear(this path_t& self) {
			self.paths_vertices.clear();
			return self;
		}

		path_t& add_path(this path_t& self, const path_t& path) {
			self.paths_vertices.push_back(path.paths_vertices);
			return self;
		}

		path_t& close(this path_t& self) {
			if(self.paths_vertices.back().size() == 0) return self;

			// @note: move the pen to the first vertex of the sub-path and
			const vec2_t<float> last_point = self.curr_subpath_last_point();

			// @note: if two last points equal the first one, it is a close path signal
			self.paths_vertices.push_back(last_point);
			self.paths_vertices.push_back(last_point);
			self.paths_vertices.cut_chunk_if_current_not_empty();
			return self;
		}

		path_t& move_to(this path_t& self, const vec2_t<float>& point) {
			self.paths_vertices.cut_chunk_if_current_not_empty();
			self.paths_vertices.push_back(point);
			return self;
		}

		path_t& line_to(this path_t& self, const vec2_t<float>& point, float threshold = 1.f) {
			auto current_path = self.paths_vertices.back();

			threshold = std::max(0.f, threshold);
			bool avoid{ };
			if(current_path.size()) {
				const vec2_t<float> vec = current_path[current_path.size() - 1] - point;
				avoid = vec.dot(vec) <= threshold * threshold;
			}

			if(!avoid) {
				if(current_path.size() == 0) {
					const auto paths = self.paths_vertices.size();
					if(paths >= 2) {
						auto last_path = self.paths_vertices[paths - 2];
						self.paths_vertices.push_back(last_path[last_path.size() - 1]);
					}
				}
				self.paths_vertices.push_back(point);
			}
			return self;
		}

		path_t& lines_to(this path_t& self, const std::vector<vec2_t<float>>& lines) {
			for(const vec2_t<float>& point : lines) self.line_to(point);
			return self;
		}

		path_t& add_poly(this path_t& self, const std::vector<vec2_t<float>>& poly) {
			if(poly.empty() == 0) return self;
			self.paths_vertices.cut_chunk_if_current_not_empty();
			return self.lines_to(poly);
		}

		path_t& rect(this path_t& self, const vec2_t<float>& point, const vec2_t<float>& size, math::e_rotation rotation = math::e_rotation::cw) {
			self.move_to(point);
			if(rotation == math::e_rotation::cw) {
				self.line_to({ point.x + size.x, point.y });
				self.line_to({ point.x + size.x, point.y + size.y });
				self.line_to({ point.x, point.y + size.y });
			} else {
				self.line_to({ point.x, point.y + size.y });
				self.line_to({ point.x + size.x, point.y + size.y });
				self.line_to({ point.x + size.x, point.y });
			}
			return self.close();
		}

		path_t& ellipse(this path_t& self, const vec2_t<float>& point, float radius_x, float radius_y, radians_t rotation_angle, radians_t start_angle, radians_t end_angle, math::e_rotation rotation = math::e_rotation::cw, unsigned divisions_count = 32) {
			microtess::dynamic_array<vec2_t<float>> output{ divisions_count, vec2_t<float>() };
			output.clear();

			microtess::elliptic_arc_divider<float, decltype(output)>::compute(
				output, point.x, point.y,
				radius_x, radius_y, rotation_angle, start_angle, end_angle, divisions_count, rotation == math::e_rotation::ccw
			);

			for(const vec2_t<float> point : output) self.line_to(point);
			return self;
		}

		path_t& circle(this path_t& self, const vec2_t<float> point, float radius, math::e_rotation rotation = math::e_rotation::cw, unsigned divisions_count = 32) {
			constexpr radians_t end_angle = angle_t<degrees_t>(360.f);
			return self.ellipse(point, radius, radius, 0, 0, end_angle, rotation, divisions_count);
		}

		path_t& arc(this path_t& self, const vec2_t<float>& point, float radius, radians_t start_angle, radians_t end_angle, math::e_rotation rotation = math::e_rotation::cw, unsigned divisions_count = 32) { return self.ellipse(point, radius, radius, 0, start_angle, end_angle, rotation, divisions_count); }
	};

	namespace path {
		enum class e_corner_sides {
			top_left,
			top_right,
			bottom_left,
			bottom_right
		}; enum_create_cast_operator(e_corner_sides, -);

		enum class e_corner_flags {
			top_left = 1 << 0,
			top_right = 1 << 1,
			bottom_left = 1 << 2,
			bottom_right = 1 << 3,

			top = top_left | top_right,
			bottom = bottom_left | bottom_right,
			left = top_left | bottom_left,
			right = top_right | bottom_right,

			left_diagonal = top_left | bottom_right,
			right_diagonal = top_right | bottom_left,

			all = top_left | top_right | bottom_left | bottom_right
		}; enum_create_bit_operators(e_corner_flags, true);
		enum_create_cast_operator(e_corner_flags, -);

		struct rounding_t {
		public:
			std::array<float, 4> corners{ };

		public:
			rounding_t() { }
			rounding_t(const std::array<float, 4>& _corners) : corners(_corners) { }
			rounding_t(float top_left, float top_right, float bottom_left, float bottom_right) : corners(top_left, top_right, bottom_left, bottom_right) { }
			rounding_t(float rounding, e_corner_flags corner_flags = e_corner_flags::all) {
				if(rounding > 0) {
					if(corner_flags & e_corner_flags::top_left) corner(e_corner_sides::top_left) = rounding;
					if(corner_flags & e_corner_flags::top_right) corner(e_corner_sides::top_right) = rounding;
					if(corner_flags & e_corner_flags::bottom_left) corner(e_corner_sides::bottom_left) = rounding;
					if(corner_flags & e_corner_flags::bottom_right) corner(e_corner_sides::bottom_right) = rounding;
				}
			}

			rounding_t(float rounding_first, float rounding_second, e_corner_flags corner_flags) {
				if(rounding_first > 0 || rounding_second > 0) {
					switch(corner_flags & e_corner_flags::all) {
						case -e_corner_flags::top: { corner(e_corner_sides::top_left) = rounding_first; corner(e_corner_sides::top_right) = rounding_second; } break;
						case -e_corner_flags::bottom: { corner(e_corner_sides::bottom_left) = rounding_first; corner(e_corner_sides::bottom_right) = rounding_second; } break;
						case -e_corner_flags::left: { corner(e_corner_sides::top_left) = rounding_first; corner(e_corner_sides::bottom_left) = rounding_second; } break;
						case -e_corner_flags::right: { corner(e_corner_sides::top_right) = rounding_first; corner(e_corner_sides::bottom_right) = rounding_second; } break;
						case -e_corner_flags::left_diagonal: { corner(e_corner_sides::top_left) = rounding_first; corner(e_corner_sides::bottom_right) = rounding_second; } break;
						case -e_corner_flags::right_diagonal: { corner(e_corner_sides::top_right) = rounding_first; corner(e_corner_sides::bottom_left) = rounding_second; } break;
					}
				}
			}

		public:
			auto&& corner(this auto&& self, e_corner_sides corner) { return self[corner]; }
			float sum() const { return std::accumulate(corners.begin(), corners.end(), 0.f); }

		public:
			auto&& operator[](this auto&& self, e_corner_sides idx) { return self.corners[-idx]; }
		};

		std::vector<vec2_t<float>> make_arc_fast(const vec2_t<float>& center, float radius, int a_min_of_12, int a_max_of_12);
		std::vector<vec2_t<float>> make_arc(const vec2_t<float>& center, float radius, float a_min, float a_max, int num_segments);

		std::vector<vec2_t<float>> make_circle(const vec2_t<float>& center, float radius, int num_segments = 0);
		std::vector<vec2_t<float>> make_rect(const vec2_t<float>& a, const vec2_t<float>& b, const rounding_t& rounding = { });
		static std::vector<vec2_t<float>> make_rect(const rect_t<float>& rect, const rounding_t& rounding = { }) { return make_rect(rect.min, rect.max, rounding); }
	}
}