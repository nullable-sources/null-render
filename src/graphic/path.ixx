module;
#include <numeric>
#include <vector>
#include <array>
#include <ranges>
#include <cmath>
#include <numbers>
#include <algorithm>

#include <utils/fast_operators.h>
export module null.render:graphic.path;

import null.sdk;

export namespace null::render {
	namespace shared {
		int arc_fast_tessellation_multiplier{ 1 };
		float circle_segment_max_error{ 1.6f };
		int circle_auto_segment_max{ 512 };

		std::array<std::uint8_t, 64> circle_segments{ };
		std::vector<vec2_t<float>> arc_fast_vertex{ };
	}

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
		rounding_t(const float& top_left, const float& top_right, const float& bottom_left, const float& bottom_right) : corners{ top_left, top_right, bottom_left, bottom_right } { }
		rounding_t(const float& rounding, const e_corner_flags& corner_flags = e_corner_flags::all) {
			if (rounding > 0) {
				if (corner_flags & e_corner_flags::top_left) corner(e_corner_sides::top_left) = rounding;
				if (corner_flags & e_corner_flags::top_right) corner(e_corner_sides::top_right) = rounding;
				if (corner_flags & e_corner_flags::bottom_left) corner(e_corner_sides::bottom_left) = rounding;
				if (corner_flags & e_corner_flags::bottom_right) corner(e_corner_sides::bottom_right) = rounding;
			}
		}

		rounding_t(const float& rounding_first, const float& rounding_second, const e_corner_flags& corner_flags) {
			if (rounding_first > 0 || rounding_second > 0) {
				switch (corner_flags & e_corner_flags::all) {
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
		float sum() const { return std::accumulate(corners.begin(), corners.end(), 0.f); }
		float& corner(const e_corner_sides& corner) { return corners[-corner]; }

	public:
		auto& operator[](const e_corner_sides& corner) const { return corners[-corner]; }
		auto& operator[](const e_corner_sides& corner) { return corners[-corner]; }
	};

	std::vector<vec2_t<float>> make_arc_fast(const vec2_t<float>& center, const float& radius, const int& a_min_of_12, const int& a_max_of_12) {
		if (radius == 0.f || a_min_of_12 > a_max_of_12) return { center };
		else {
			return std::views::iota(a_min_of_12 * shared::arc_fast_tessellation_multiplier, a_max_of_12 * shared::arc_fast_tessellation_multiplier + 1)
				| std::views::transform([&](const int& a) { return vec2_t<float>{ center + shared::arc_fast_vertex[a % shared::arc_fast_vertex.size()] * radius }; })
				| std::ranges::to<std::vector>();
		}
	}

	std::vector<vec2_t<float>> make_arc(const vec2_t<float>& center, const float& radius, const float& a_min, const float& a_max, const int& num_segments) {
		if (radius == 0.f) return { center };
		else {
			return std::views::iota(0, num_segments + 1)
				| std::views::transform([&](const int& i) { const float a{ a_min + ((float)i / (float)num_segments) * (a_max - a_min) }; return vec2_t<float>{ center + vec2_t{ std::cosf(a), std::sinf(a) } *radius }; })
				| std::ranges::to<std::vector>();
		}
	}

	std::vector<vec2_t<float>> make_circle(const vec2_t<float>& center, const float& radius, int num_segments = 0) {
		if (num_segments <= 0) {
			const int radius_idx{ (int)radius - 1 };
			if (radius_idx < shared::circle_segments.size()) num_segments = shared::circle_segments[radius_idx];
			else num_segments = std::clamp((int)((std::numbers::pi * 2.0f) / std::acosf((radius - shared::circle_segment_max_error) / radius)), 12, shared::circle_auto_segment_max);
		}
		else num_segments = std::clamp(num_segments, 3, shared::circle_auto_segment_max);

		return num_segments == 12 ? make_arc_fast(center, radius, 0, 11) : make_arc(center, radius, 0.0f, (std::numbers::pi * 2.f) * (num_segments - 1.f) / num_segments, num_segments - 1);
	}

	std::vector<vec2_t<float>> make_rect(const vec2_t<float>& a, const vec2_t<float>& b, const rounding_t& rounding = { }) {
		if (rounding.sum() <= 0.0f) return { a, { b.x, a.y }, b, { a.x, b.y } };

		const vec2_t size{ math::abs(b - a) - 1.f };
		float scale_factor{ std::numeric_limits<float>::max() };
		scale_factor = std::min(scale_factor, size.x / (rounding[e_corner_sides::top_left] + rounding[e_corner_sides::top_right]));
		scale_factor = std::min(scale_factor, size.y / (rounding[e_corner_sides::top_right] + rounding[e_corner_sides::bottom_right]));
		scale_factor = std::min(scale_factor, size.x / (rounding[e_corner_sides::bottom_right] + rounding[e_corner_sides::bottom_left]));
		scale_factor = std::min(scale_factor, size.y / (rounding[e_corner_sides::bottom_left] + rounding[e_corner_sides::top_left]));
		scale_factor = std::min(1.0f, scale_factor);

		rounding_t result_rounding{ };
		for (std::tuple<const float&, float&> tuple : std::views::zip(rounding.corners, result_rounding.corners))
			std::get<float&>(tuple) = std::round(std::get<const float&>(tuple) * scale_factor);

		return std::vector{
			make_arc_fast(a + result_rounding[e_corner_sides::top_left], result_rounding[e_corner_sides::top_left], 6, 9),
			make_arc_fast(vec2_t{ b.x, a.y } + vec2_t{ -result_rounding[e_corner_sides::top_right], result_rounding[e_corner_sides::top_right] }, result_rounding[e_corner_sides::top_right], 9, 12),
			make_arc_fast(b - result_rounding[e_corner_sides::bottom_right], result_rounding[e_corner_sides::bottom_right], 0, 3),
			make_arc_fast(vec2_t{ a.x, b.y } + vec2_t{ result_rounding[e_corner_sides::bottom_left], -result_rounding[e_corner_sides::bottom_left] }, result_rounding[e_corner_sides::bottom_left], 3, 6)
		} | std::views::join | std::ranges::to<std::vector>();
	}

	std::vector<vec2_t<float>> make_rect(const rect_t<float>& rect, const rounding_t& rounding = { }) {
		return make_rect(rect.min, rect.max, rounding);
	}
}