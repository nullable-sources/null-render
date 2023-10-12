#pragma once
#include <null-sdk.h>

namespace null::render::path {
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
		rounding_t(const std::array<float, 4>& _corners) : corners{ _corners } { }
		rounding_t(float top_left, float top_right, float bottom_left, float bottom_right) : corners{ top_left, top_right, bottom_left, bottom_right } { }
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