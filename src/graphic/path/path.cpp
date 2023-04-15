#include <graphic/path/path.h>
#include <null-render.h>

namespace null::render::path {
    std::vector<vec2_t<float>> make_arc_fast(const vec2_t<float>& center, const float& radius, const int& a_min_of_12, const int& a_max_of_12) {
        if(radius == 0.f || a_min_of_12 > a_max_of_12) return { center };
        else {
            return std::views::iota(a_min_of_12 * shared::arc_fast_tessellation_multiplier, a_max_of_12 * shared::arc_fast_tessellation_multiplier + 1)
                | std::views::transform([&](const int& a) { return vec2_t<float>{ center + shared::arc_fast_vertex[a % shared::arc_fast_vertex.size()] * radius }; })
                | std::ranges::to<std::vector>();
        }
    }

    std::vector<vec2_t<float>> make_arc(const vec2_t<float>& center, const float& radius, const float& a_min, const float& a_max, const int& num_segments) {
        if(radius == 0.f) return { center };
        else {
            return std::views::iota(0, num_segments + 1)
                | std::views::transform([&](const int& i) { const float a{ a_min + ((float)i / (float)num_segments) * (a_max - a_min) }; return vec2_t<float>{ center + vec2_t{ std::cosf(a), std::sinf(a) } *radius }; })
                | std::ranges::to<std::vector>();
        }
    }

	std::vector<vec2_t<float>> make_rect(const vec2_t<float>& a, const vec2_t<float>& b, const rounding_t& rounding) {
        if(rounding.sum() <= 0.0f) return { a, { b.x, a.y }, b, { a.x, b.y } };
        
        const vec2_t size{ math::abs(b - a) - 1.f };
        float scale_factor{ std::numeric_limits<float>::max() };
        scale_factor = std::min(scale_factor, size.x / (rounding[e_corner_sides::top_left] + rounding[e_corner_sides::top_right]));
        scale_factor = std::min(scale_factor, size.y / (rounding[e_corner_sides::top_right] + rounding[e_corner_sides::bottom_right]));
        scale_factor = std::min(scale_factor, size.x / (rounding[e_corner_sides::bottom_right] + rounding[e_corner_sides::bottom_left]));
        scale_factor = std::min(scale_factor, size.y / (rounding[e_corner_sides::bottom_left] + rounding[e_corner_sides::top_left]));
        scale_factor = std::min(1.0f, scale_factor);

        rounding_t result_rounding{ };
        for(std::tuple<const float&, float&> tuple : std::views::zip(rounding.corners, result_rounding.corners))
            std::get<float&>(tuple) = std::round(std::get<const float&>(tuple) * scale_factor);

        return std::vector{
            make_arc_fast(a + result_rounding[e_corner_sides::top_left], result_rounding[e_corner_sides::top_left], 6, 9),
            make_arc_fast(vec2_t{ b.x, a.y } + vec2_t{ -result_rounding[e_corner_sides::top_right], result_rounding[e_corner_sides::top_right] }, result_rounding[e_corner_sides::top_right], 9, 12),
            make_arc_fast(b - result_rounding[e_corner_sides::bottom_right], result_rounding[e_corner_sides::bottom_right], 0, 3),
            make_arc_fast(vec2_t{ a.x, b.y } + vec2_t{ result_rounding[e_corner_sides::bottom_left], -result_rounding[e_corner_sides::bottom_left] }, result_rounding[e_corner_sides::bottom_left], 3, 6)
        } | std::views::join | std::ranges::to<std::vector>();
	}
}