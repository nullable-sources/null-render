#pragma once
#include <font/font.h>

namespace null::render {
	enum struct e_initialize_flags {
		anti_aliased_lines = 1 << 0,
		anti_aliased_lines_use_texture = 1 << 1,
		anti_aliased_fill = 1 << 2
	}; enum_create_bit_operators(e_initialize_flags, true);
	enum_create_cast_operator(e_initialize_flags, -);

	enum class e_corner_flags {
		top_left = 1 << 0,
		top_right = 1 << 1,
		bot_left = 1 << 2,
		bot_right = 1 << 3,

		top = top_left | top_right,
		bot = bot_left | bot_right,
		left = top_left | bot_left,
		right = top_right | bot_right,

		all = top_left | top_right | bot_left | bot_right
	}; enum_create_bit_operators(e_corner_flags, true);
	enum_create_cast_operator(e_corner_flags, -);

	struct vertex_t {
		vec2_t<float> pos{ }, uv{ };
		color_t<int> color{ };
	};

	namespace geometry_utils {
		static const std::vector<std::uint32_t> quad_indicies{
			0, 1, 2,
			0, 2, 3
		};

		static std::vector<vertex_t> build_rect_vertex(const vec2_t<float>& a, const vec2_t<float>& b, const vec2_t<float>& uv_a, const vec2_t<float>& uv_b, const std::array<color_t<float>, 4>& colors) {
			return {
				{ a, uv_a, colors[0] },
				{ { b.x, a.y }, { uv_b.x, uv_a.y }, colors[1] },
				{ b, uv_b, colors[2] },
				{ { a.x, b.y }, { uv_a.x, uv_b.y }, colors[3] }
			};
		}
		static std::vector<vertex_t> build_rect_vertex(const vec2_t<float>& a, const vec2_t<float>& b, const vec2_t<float>& uv_a, const vec2_t<float>& uv_b, const color_t<float>& color) { return build_rect_vertex(a, b, uv_a, uv_b, std::array<color_t<float>, 4>{ color, color, color, color }); }
	}

	class c_geometry_buffer {
	public:
		struct settings_t {
		public:
			static constexpr int arc_fast_tessellation_multiplier{ 1 };
			static constexpr int circle_auto_segment_max{ 512 };

		public:
			e_initialize_flags initialize_flags{ };

			std::vector<vec2_t<float>> text_outline_offsets{ { -1, 0 }, { 0, -1 }, { 0, 1 }, { 1, 0 } };
			std::array<vec2_t<float>, 12 * arc_fast_tessellation_multiplier> arc_fast_vtx{ };
			std::array<std::uint8_t, 64> circle_segments{ };
			float circle_segment_max_error{ };
			float curve_tessellation_tol{ 1.25f };

			settings_t(const e_initialize_flags& _initialize_flags = e_initialize_flags{ }) : initialize_flags{ _initialize_flags } {
				std::ranges::for_each(std::views::iota(0, (int)arc_fast_vtx.size()), [=](const int& i) {
					float a{ float(i * 2.f * std::numbers::pi) / arc_fast_vtx.size() };
					arc_fast_vtx[i] = vec2_t{ std::cosf(a), std::sinf(a) };
					});
			}

			void set_circle_segment_max_error(const float& max_error) {
				if(circle_segment_max_error == max_error) return;
				circle_segment_max_error = max_error;

				std::ranges::for_each(std::views::iota(0, (int)circle_segments.size()), [=](const int& i) {
					float radius{ i + 1.f };
					circle_segments[i] = std::min(std::clamp((std::numbers::pi * 2.f) / std::acosf((radius - circle_segment_max_error) / radius), 12., 512.), 255.);
					});
			}

			void get_auto_circle_num_segments(int& num_segments, const float& radius) {
				if(num_segments <= 0) {
					const int radius_idx{ (int)radius - 1 };
					if(radius_idx < circle_segments.size()) num_segments = circle_segments[radius_idx];
					else num_segments = std::clamp((int)((std::numbers::pi * 2.0f) / acosf((radius - circle_segment_max_error) / radius)), 12, circle_auto_segment_max);
				} else num_segments = std::clamp(num_segments, 3, circle_auto_segment_max);
			}
		};

	public:
		settings_t settings{ e_initialize_flags{ -e_initialize_flags::anti_aliased_lines | -e_initialize_flags::anti_aliased_lines_use_texture | -e_initialize_flags::anti_aliased_fill } };

		std::vector<vec2_t<float>> pathes{ };

		std::vector<vertex_t> vtx_buffer{ };
		std::vector<std::uint32_t> idx_buffer{ };

	public:
		virtual void clear() { pathes.clear(); vtx_buffer.clear(); idx_buffer.clear(); }

	public:
		void add_vtx(const std::vector<vertex_t>::iterator& place, const std::vector<vertex_t>& buffer) { on_add_vtx(place, buffer); vtx_buffer.insert(place, buffer.begin(), buffer.end()); }
		void add_vtx(const std::vector<vertex_t>& buffer) { add_vtx(vtx_buffer.end(), buffer); }
		void add_idx(const std::vector<std::uint32_t>::iterator& place, const std::vector<std::uint32_t>& buffer) { on_add_idx(place, buffer); idx_buffer.insert(place, buffer.begin(), buffer.end()); }
		void add_idx(const std::vector<std::uint32_t>::iterator& place, const std::vector<std::uint32_t>& buffer, const std::uint32_t& offset) { add_idx(place, std::views::transform(buffer, [&](const std::uint32_t& idx) { return idx + offset; }) | std::ranges::to<std::vector>()); }
		void add_idx(const std::vector<std::uint32_t>& buffer) { add_idx(idx_buffer.end(), buffer); }
		void add_idx(const std::vector<std::uint32_t>& buffer, const std::uint32_t& offset) { add_idx(idx_buffer.end(), buffer, offset); }

	public:
		void path_rect(const vec2_t<float>& a, const vec2_t<float>& b, float rounding = 0.0f, const e_corner_flags& flags = e_corner_flags::all);
		void path_arc_to_fast(const vec2_t<float>& center, const float& radius, const int& a_min_of_12, const int& a_max_of_12);
		void path_arc_to(const vec2_t<float>& center, const float& radius, const float& a_min, const float& a_max, const int& num_segments);
		void path_fill_convex(const color_t<int>& clr) { add_convex_poly_filled(pathes, clr); pathes.clear(); }
		void path_stroke(const color_t<int>& color, const bool& closed, const float& thickness) { add_poly_line(pathes, color, closed, thickness); pathes.clear(); }

		void repaint_rect_vertices_in_multicolor(const vec2_t<float>& min, const vec2_t<float>& max, const size_t& vtx_offset, const std::array<color_t<int>, 4>& colors);

	public:
		void add_rect(const vec2_t<float>& a, const vec2_t<float>& b, const color_t<int>& color, const float& thickness = 1.f, const float& rounding = 0.f, const e_corner_flags& flags = e_corner_flags::all);
		void add_rect(const rect_t<float>& rect, const color_t<int>& color, const float& thickness = 1.f, const float& rounding = 0.f, const e_corner_flags& flags = e_corner_flags::all) { add_rect(rect.min, rect.max, color, thickness, rounding, flags); }
		void add_rect_filled(const vec2_t<float>& a, const vec2_t<float>& b, const color_t<int>& color, const float& rounding = 0.f, const e_corner_flags& flags = e_corner_flags::all);
		void add_rect_filled(const rect_t<float>& rect, const color_t<int>& color, const float& rounding = 0.f, const e_corner_flags& flags = e_corner_flags::all) { add_rect_filled(rect.min, rect.max, color, rounding, flags); }

		void add_rect_multicolor(const vec2_t<float>& a, const vec2_t<float>& b, const std::array<color_t<int>, 4>& colors, const float& thickness = 1.f, const float& rounding = 0.f, const e_corner_flags& flags = e_corner_flags::all);
		void add_rect_multicolor(const rect_t<float>& rect, const std::array<color_t<int>, 4>& colors, const float& thickness = 1.f, const float& rounding = 0.f, const e_corner_flags& flags = e_corner_flags::all) { add_rect_multicolor(rect.min, rect.max, colors, thickness, rounding, flags); }
		void add_rect_filled_multicolor(const vec2_t<float>& a, const vec2_t<float>& b, const std::array<color_t<int>, 4>& colors, float rounding = 0.f, const e_corner_flags& flags = e_corner_flags::all);
		void add_rect_filled_multicolor(const rect_t<float>& rect, const std::array<color_t<int>, 4>& colors, const float& rounding = 0.f, const e_corner_flags& flags = e_corner_flags::all) { add_rect_filled_multicolor(rect.min, rect.max, colors, rounding, flags); }
		
		void add_line(const vec2_t<float>& a, const vec2_t<float>& b, const color_t<int>& color, const float& thickness = 1.f);
		void add_poly_line(const std::vector<vec2_t<float>>& points, const color_t<int>& color, const bool& closed, float thickness = 1.f);
		
		void add_circle(const vec2_t<float>& center, const color_t<int>& clr, const float& radius, int num_segments = 0, const float& thickness = 1.f);
		void add_circle_filled(const vec2_t<float>& center, const color_t<int>& clr, const float& radius, int num_segments = 0);
		
		void add_convex_poly_filled(const std::vector<vec2_t<float>>& points, const color_t<int>& color);

	public:
		virtual void on_add_vtx(const std::vector<vertex_t>::iterator& place, const std::vector<vertex_t>& buffer) { }
		virtual void on_add_idx(const std::vector<std::uint32_t>::iterator& place, const std::vector<std::uint32_t>& buffer) { }
	};
}