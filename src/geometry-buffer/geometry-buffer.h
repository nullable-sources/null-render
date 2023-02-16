#pragma once
#include <font/font.h>

namespace null::render {
	enum struct e_initialize_flags {
		anti_aliased_lines = 1 << 0,
		anti_aliased_lines_use_texture = 1 << 1,
		anti_aliased_fill = 1 << 2
	}; enum_create_bit_operators(e_initialize_flags, true);
	enum_create_cast_operator(e_initialize_flags, -);

	//@note: flags for drawing text
	enum class e_text_flags {
		//@note:	the default color for the outline is black, maybe later I will add color settings, but personally I don't need it, so you can always change it in your solution
		//			you can change offsets in shared data(text_outline_offsets)
		outline = 1 << 0,

		//@note: default aligin is top and left
		aligin_right = 1 << 1, //@note: will be draw at pos - text_size.x
		aligin_bottom = 1 << 2, //@note: will be draw at pos - text_size.y
		//@note: align_center is calculated after the rest, so it can be combined with left and bottom
		aligin_center_x = 1 << 3,
		aligin_center_y = 1 << 4,
		aligin_center = aligin_center_x | aligin_center_y,
		aligin_mask = aligin_center | aligin_bottom | aligin_right
	}; enum_create_bit_operators(e_text_flags, true);
	enum_create_cast_operator(e_text_flags, -);

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
	public:
		vec2_t<float> pos{ }, uv{ };
		color_t<int> color{ };

	public:
		vertex_t() { }
		vertex_t(const vec2_t<float>& _pos, const color_t<int>& _color) : pos{ _pos }, color{ _color } { }
		vertex_t(const vec2_t<float>& _pos, const vec2_t<float>& _uv, const color_t<int>& _color) : pos{ _pos }, color{ _color }, uv{ _uv } { }
	};

	enum class e_cmd_callbacks {
		on_draw
	};

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

		struct cmd_t {
		public:
			rect_t<float> clip_rect{ };
			void* texture{ };
			std::uint32_t element_count{ };

			utils::callbacks_tuple_t<utils::callback_t<e_cmd_callbacks::on_draw, bool(c_geometry_buffer::cmd_t&)>> callbacks{ };
		};

	public:
		settings_t settings{ e_initialize_flags{ -e_initialize_flags::anti_aliased_lines | -e_initialize_flags::anti_aliased_lines_use_texture | -e_initialize_flags::anti_aliased_fill } };

		std::vector<void*> textures{ };
		std::vector<rect_t<float>> clips{ };

		std::vector<vertex_t> vtx_buffer{ };
		std::vector<std::uint32_t> idx_buffer{ };

		std::vector<cmd_t> cmd_buffer{ };

	public:
		c_geometry_buffer() { reset(); }

	public:
		void add_cmd() { cmd_buffer.push_back({ clips.back(), textures.back() }); }
		void clear() { vtx_buffer.clear(); idx_buffer.clear(); textures.clear(); clips.clear(); cmd_buffer.clear(); }
		void reset();

	public:
		void add_vtx(const std::vector<vertex_t>::iterator& place, const std::vector<vertex_t>& buffer) { std::ranges::move(buffer, std::inserter(vtx_buffer, place)); }
		void add_vtx(const std::vector<vertex_t>& buffer) { add_vtx(vtx_buffer.end(), buffer); }
		void add_idx(const std::vector<std::uint32_t>::iterator& place, const std::vector<std::uint32_t>& buffer) { cmd_buffer.back().element_count += buffer.size(); std::ranges::move(buffer, std::inserter(idx_buffer, place)); }
		void add_idx(const std::vector<std::uint32_t>::iterator& place, const std::vector<std::uint32_t>& buffer, const std::uint32_t& offset) { cmd_buffer.back().element_count += buffer.size(); std::ranges::move(buffer | std::views::transform([&](const std::uint32_t& idx) { return idx + offset; }), std::inserter(idx_buffer, place)); }
		void add_idx(const std::vector<std::uint32_t>& buffer) { add_idx(idx_buffer.end(), buffer); }
		void add_idx(const std::vector<std::uint32_t>& buffer, const std::uint32_t& offset) { add_idx(idx_buffer.end(), buffer, offset); }

	public:;
		void repaint_rect_vertices_in_multicolor(const vec2_t<float>& min, const vec2_t<float>& max, const size_t& vtx_offset, const std::array<color_t<int>, 4>& colors);

	public:
		void restore_clip_rect();
		void push_clip_rect(rect_t<float> rect, const bool& intersect_with_current_rect = false);
		void push_clip_rect(const vec2_t<float>& a, const vec2_t<float>& b, const bool& intersect_with_current_rect = false) { push_clip_rect(rect_t{ a, b }, intersect_with_current_rect); }
		void pop_clip_rect() { clips.pop_back(); on_change_clip_rect(); }

		void push_texture(void* texture) { textures.push_back(texture); on_change_texture(); }
		void pop_texture() { textures.pop_back(); on_change_texture(); }

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

		void add_circle(const vec2_t<float>& center, const color_t<int>& color, const float& radius, int num_segments = 0, const float& thickness = 1.f);
		void add_circle_filled(const vec2_t<float>& center, const color_t<int>& color, const float& radius, int num_segments = 0);

		void add_convex_poly_filled(const std::vector<vec2_t<float>>& points, const color_t<int>& color);

		void add_image(void* texture, const vec2_t<float>& a, const vec2_t<float>& b, const vec2_t<float>& uv_min, const vec2_t<float>& uv_max, const color_t<int>& color);
		void add_image(void* texture, const rect_t<float>& rect, const rect_t<float>& uvs, const color_t<int>& color) { add_image(texture, rect.min, rect.max, uvs.min, uvs.max, color); }
		void add_image_quad(void* texture, const std::array<std::pair<vec2_t<float>, vec2_t<float>>, 4>& points_and_uvs, const color_t<int>& color);

		template <typename char_t> void add_text(const std::basic_string_view<char_t>& str, const color_t<int>& color, vec2_t<float>& pos, float& new_line_pos, c_font* font, const float& size, int& vtx_offset, e_text_flags flags);
		template <typename string_view_t> void add_text(const string_view_t& str, vec2_t<float> pos, const color_t<int>& color, e_text_flags flags = e_text_flags{ }, c_font* font = c_font::get_current_font(), float size = 0.f);
		template <typename string_t> void add_text(const multicolor_text_t<string_t>& str, vec2_t<float> pos, e_text_flags flags = e_text_flags{ }, c_font* font = c_font::get_current_font(), float size = 0.f);

	public:
		virtual void on_change_clip_rect();
		virtual void on_change_texture();
	};

	inline c_geometry_buffer background{ }, foreground{ };
	inline std::vector<c_geometry_buffer*> custom_buffers{ }, fast_buffers{ };

	namespace geometry_utils {
		static const std::vector<std::uint32_t> quad_indexes{
			0, 1, 2,
			0, 2, 3
		};

		std::vector<vertex_t> build_rect_vertex(const vec2_t<float>& a, const vec2_t<float>& b, const vec2_t<float>& uv_a, const vec2_t<float>& uv_b, const std::array<color_t<int>, 4>& colors);
		static std::vector<vertex_t> build_rect_vertex(const vec2_t<float>& a, const vec2_t<float>& b, const vec2_t<float>& uv_a, const vec2_t<float>& uv_b, const color_t<int>& color) { return build_rect_vertex(a, b, uv_a, uv_b, std::array<color_t<int>, 4>{ color, color, color, color }); }

		std::vector<vec2_t<float>> build_arc_to_fast_path(const vec2_t<float>& center, const float& radius, const int& a_min_of_12, const int& a_max_of_12, const c_geometry_buffer::settings_t& settings);
		std::vector<vec2_t<float>> build_arc_to_path(const vec2_t<float>& center, const float& radius, const float& a_min, const float& a_max, const int& num_segments);
		std::vector<vec2_t<float>> build_rect_path(const c_geometry_buffer::settings_t& settings, const vec2_t<float>& a, const vec2_t<float>& b, float rounding = 0.0f, const e_corner_flags& flags = e_corner_flags::all);
	}
}

#include <geometry-buffer/geometry-buffer.inl>