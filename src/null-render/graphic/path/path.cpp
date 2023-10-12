#include "null-render.h"

namespace null::render::path {
	std::vector<vec2_t<float>> make_arc_fast(const vec2_t<float>& center, float radius, int a_min_of_12, int a_max_of_12) {
		if(radius == 0.f || a_min_of_12 > a_max_of_12) return { center };
		else {
			return std::views::iota(a_min_of_12 * shared::arc_fast_tessellation_multiplier, a_max_of_12 * shared::arc_fast_tessellation_multiplier + 1)
					| std::views::transform([&](const int& a) { return vec2_t<float>{ center + shared::arc_fast_vertex[a % shared::arc_fast_vertex.size()] * radius }; })
					| std::ranges::to<std::vector>();
		}
	}

	std::vector<vec2_t<float>> make_arc(const vec2_t<float>& center, float radius, float a_min, float a_max, int num_segments) {
		if(radius == 0.f) return { center };
		else {
			return std::views::iota(0, num_segments + 1)
				| std::views::transform([&](const int& i) { const float a{ a_min + ((float)i / (float)num_segments) * (a_max - a_min) }; return vec2_t<float>{ center + vec2_t{ std::cosf(a), std::sinf(a) } *radius }; })
				| std::ranges::to<std::vector>();
		}
	}

	std::vector<vec2_t<float>> make_circle(const vec2_t<float>& center, float radius, int num_segments) {
		if(num_segments <= 0) {
			const int radius_idx{ (int)radius - 1 };
			if(radius_idx < shared::circle_segments.size()) num_segments = shared::circle_segments[radius_idx];
			else num_segments = std::clamp((int)((std::numbers::pi * 2.0f) / std::acosf((radius - shared::circle_segment_max_error) / radius)), 12, shared::circle_auto_segment_max);
		} else num_segments = std::clamp(num_segments, 3, shared::circle_auto_segment_max);

		return num_segments == 12 ? make_arc_fast(center, radius, 0, 11) : make_arc(center, radius, 0.0f, (std::numbers::pi * 2.f) * (num_segments - 1.f) / num_segments, num_segments - 1);
	}

	std::vector<vec2_t<float>> make_rect(const vec2_t<float>& a, const vec2_t<float>& b, const rounding_t& rounding) {
		if(rounding.sum() <= 0.0f) return { a, { b.x, a.y }, b, { a.x, b.y } };

		const vec2_t<float> size{ math::abs(b - a) - 1.f };
		float scale_factor{ std::numeric_limits<float>::max() };
		scale_factor = std::min(scale_factor, size.x / (rounding[e_corner_sides::top_left] + rounding[e_corner_sides::top_right]));
		scale_factor = std::min(scale_factor, size.y / (rounding[e_corner_sides::top_right] + rounding[e_corner_sides::bottom_right]));
		scale_factor = std::min(scale_factor, size.x / (rounding[e_corner_sides::bottom_right] + rounding[e_corner_sides::bottom_left]));
		scale_factor = std::min(scale_factor, size.y / (rounding[e_corner_sides::bottom_left] + rounding[e_corner_sides::top_left]));
		scale_factor = std::min(1.0f, scale_factor);

		rounding_t result_rounding{ };
		for(auto [corner, result] : std::views::zip(rounding.corners, result_rounding.corners))
			result = std::round(corner * scale_factor);

		return std::vector{
			make_arc_fast(a + result_rounding[e_corner_sides::top_left], result_rounding[e_corner_sides::top_left], 6, 9),
			make_arc_fast(vec2_t{ b.x, a.y } + vec2_t{ -result_rounding[e_corner_sides::top_right], result_rounding[e_corner_sides::top_right] }, result_rounding[e_corner_sides::top_right], 9, 12),
			make_arc_fast(b - result_rounding[e_corner_sides::bottom_right], result_rounding[e_corner_sides::bottom_right], 0, 3),
			make_arc_fast(vec2_t{ a.x, b.y } + vec2_t{ result_rounding[e_corner_sides::bottom_left], -result_rounding[e_corner_sides::bottom_left] }, result_rounding[e_corner_sides::bottom_left], 3, 6)
		} | std::views::join | std::ranges::to<std::vector>();
	}
}