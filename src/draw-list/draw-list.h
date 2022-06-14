#pragma once
#include <font/font.h>

namespace null {
	enum struct e_draw_list_flags {
		anti_aliased_lines = 1 << 0,
		anti_aliased_lines_use_texture = 1 << 1,
		anti_aliased_fill = 1 << 2,
		allow_vtx_offset = 1 << 3,
	}; enum_create_bit_operators(e_draw_list_flags, true);
	enum_create_cast_operator(e_draw_list_flags, -);

	enum class e_corner_flags {
		top_left = 1 << 0,
		top_right = 1 << 1,
		bot_left = 1 << 2,
		bot_right = 1 << 3,

		top = top_left | top_right,
		bot = bot_left | bot_right,
		left = top_left | bot_left,
		right = top_right | top_right,

		all = top_left | top_right | bot_left | bot_right
	}; enum_create_bit_operators(e_corner_flags, true);
	enum_create_cast_operator(e_corner_flags, -);

	//flags for drawing text
	enum class e_text_flags : std::uint32_t {
		//the default color for the outline is black, maybe later I will add color settings, but personally I don't need it, so you can always change it in your solution
		//you can change offsets in shared data(text_outline_offsets)
		outline = 1 << 0,

		//default aligin is top and left
		aligin_right = 1 << 1, //will be draw at pos - text_size.x
		aligin_bottom = 1 << 2, //will be draw at pos - text_size.y
		//align_center is calculated after the rest, so it can be combined with left and bottom
		aligin_center_x = 1 << 3,
		aligin_center_y = 1 << 4,
		aligin_center = aligin_center_x | aligin_center_y,
		aligin_mask = aligin_center | aligin_bottom | aligin_right
	}; enum_create_bit_operators(e_text_flags, true);
	enum_create_cast_operator(e_text_flags, -);

	enum class e_cmd_callbacks {
		render_draw_data //std::function<bool(cmd*)>, will call setup_render_state if the callback returns true
	};

	namespace render {
		struct shared_data_t {
		public: //used instead of #define
			static const int arc_fast_tessellation_multiplier{ 1 };
			static const int circle_auto_segment_max{ 512 };

		public:
			std::vector<vec2_t> text_outline_offsets{ { -1, 0 }, { 0, -1 }, { 0, 1}, { 1, 0 } };
			e_draw_list_flags initialize_flags{ };
			c_font* font{ };
			rect_t clip_rect_fullscreen{ };

			std::array<vec2_t, 12 * arc_fast_tessellation_multiplier> arc_fast_vtx{ };
			std::array<std::uint8_t, 64> circle_segments{ };
			float circle_segment_max_error{ };
			float curve_tessellation_tol{ };

			shared_data_t(e_draw_list_flags _initialize_flags = e_draw_list_flags{ }) : initialize_flags(_initialize_flags) {
				std::ranges::for_each(std::views::iota(0, (int)arc_fast_vtx.size()), [=](const int& i) {
					float a = (i * 2.f * std::numbers::pi) / arc_fast_vtx.size();
					arc_fast_vtx[i] = vec2_t{ std::cosf(a), std::sinf(a) };
					});
			}

			void set_circle_segment_max_error(float max_error) {
				if(circle_segment_max_error == max_error) return;
				circle_segment_max_error = max_error;

				std::ranges::for_each(std::views::iota(0, (int)circle_segments.size()), [=](const int& i) {
					float radius = i + 1.f;
					circle_segments[i] = std::min(std::clamp((std::numbers::pi * 2.f) / std::acosf((radius - circle_segment_max_error) / radius), 12., 512.), 255.);
					});
			}

			void get_auto_circle_num_segments(int& num_segments, float radius) {
				if(num_segments <= 0) {
					const int radius_idx = (int)radius - 1;
					if(radius_idx < circle_segments.size()) num_segments = circle_segments[radius_idx];
					else num_segments = std::clamp((int)((std::numbers::pi * 2.0f) / acosf((radius - circle_segment_max_error) / radius)), 12, circle_auto_segment_max);
				} else num_segments = std::clamp(num_segments, 3, circle_auto_segment_max);
			}
		} inline shared_data{ e_draw_list_flags::allow_vtx_offset };

		class c_draw_list {
		public:
			struct vertex_t {
				vec2_t pos{ }, uv{ };
				color_t color{ };
			};

			struct draw_data_t {
				std::vector<c_draw_list*> layers{ };

				bool valid{ };
				std::vector<c_draw_list*> cmd_lists{ };
				int total_idx_count{ }, total_vtx_count{ };

				vec2_t window_pos{ }, window_size{ };

				void deindex_all_buffers();
				void add_draw_list(c_draw_list* draw_list);
				void setup();

				void scale_clip_rects(const vec2_t& scale) {
					for(c_draw_list* cmd_list : cmd_lists)
						for(cmd_t& cmd : cmd_list->cmd_buffer) cmd.clip_rect *= scale;
				}
			};

			struct cmd_header_t {
				rect_t clip_rect{ };
				void* texture_id{ };
				std::uint32_t vtx_offset{ };

				bool operator==(const cmd_header_t&) const = default;
			};

			struct cmd_t : cmd_header_t {
				std::uint32_t idx_offset{ }, element_count{ };

				single_callbacks_t<e_cmd_callbacks> callbacks{ };
			};

		public:
			shared_data_t* parent_shared_data{ &shared_data };

			std::vector<void*> texture_id_stack{ };
			std::vector<vec2_t> path{ };
			std::vector<rect_t> clip_rect_stack{ };

			std::vector<cmd_t> cmd_buffer{ };
			cmd_header_t cmd_header{ };

			using idx_buffer_t = std::vector<std::uint16_t>; idx_buffer_t idx_buffer{ };
			using vtx_buffer_t = std::vector<vertex_t>; vtx_buffer_t vtx_buffer{ };

		public:
			void add_draw_cmd() { cmd_buffer.push_back({ cmd_header.clip_rect, cmd_header.texture_id, cmd_header.vtx_offset, (std::uint32_t)idx_buffer.size() }); }
			void pop_unused_draw_cmd() { if(cmd_buffer.empty()) return; if(!cmd_buffer.back().element_count && !cmd_buffer.back().callbacks.empty()) cmd_buffer.pop_back(); }

			void reset_for_begin_frame() {
				cmd_buffer = { cmd_t{ } };
				idx_buffer.clear();
				vtx_buffer.clear();
				clip_rect_stack.clear();
				texture_id_stack.clear();
				path.clear();

				cmd_header = cmd_header_t{ };
			}
			void shade_verts_linear_uv(const vtx_buffer_t::iterator& vtx_start, const vtx_buffer_t::iterator& vtx_end, const rect_t& rect, const rect_t& uv, bool clamp);

			void on_changed_clip_rect();
			void push_clip_rect(rect_t rect, bool intersect_with_current_rect = false);
			void push_clip_rect_fullscreen() { push_clip_rect(parent_shared_data->clip_rect_fullscreen); }
			void pop_clip_rect();

			void on_changed_texture_id();
			void push_texture_id(void* texture_id);
			void pop_texture_id();

			void on_changed_vtx_offset();
			void vtx_check(int vtx_count);
			void prim_add_vtx(const vertex_t& vtx) { vtx_check(1); vtx_buffer.push_back(vtx); }
			void prim_insert_vtx(const vtx_buffer_t::iterator& place, const vtx_buffer_t& vtx_list) { vtx_check(vtx_list.size()); vtx_buffer.insert(place, vtx_list.begin(), vtx_list.end()); }
			void prim_insert_vtx(const vtx_buffer_t& vtx_list) { prim_insert_vtx(vtx_buffer.end(), vtx_list); }
			void prim_write_vtx(vtx_buffer_t::iterator& place, const vtx_buffer_t& vtx_list) { std::move(vtx_list.begin(), vtx_list.end(), place); place += vtx_list.size(); }

			void prim_add_idx(std::uint16_t idx) { idx_buffer.push_back(idx); cmd_buffer.back().element_count++; }
			void prim_insert_idx(const idx_buffer_t::iterator& place, const idx_buffer_t& idx_list) { idx_buffer.insert(place, idx_list.begin(), idx_list.end()); cmd_buffer.back().element_count += idx_list.size(); }
			void prim_insert_idx(const idx_buffer_t& idx_list) { prim_insert_idx(idx_buffer.end(), idx_list); }
			void prim_write_idx(idx_buffer_t::iterator& place, const idx_buffer_t& idx_list) { std::move(idx_list.begin(), idx_list.end(), place); place += idx_list.size(); cmd_buffer.back().element_count += idx_list.size(); }

			void prim_rect(const vec2_t& a, const vec2_t& c, const color_t& color);
			void prim_rect_uv(const vec2_t& a, const vec2_t& c, const vec2_t& uv_a, const vec2_t& uv_c, const color_t& color);
			void prim_quad_uv(const std::array<std::pair<vec2_t, vec2_t>, 4>& points, const color_t& color);

			void path_rect(const vec2_t& a, const vec2_t& b, float rounding = 0.0f, e_corner_flags flags = e_corner_flags::all);
			void path_arc_to_fast(const vec2_t& center, float radius, int a_min_of_12, int a_max_of_12);
			void path_arc_to(const vec2_t& center, float radius, float a_min, float a_max, int num_segments);
			void path_fill_convex(const color_t& clr) { draw_convex_poly_filled(path, clr); path.clear(); }
			void path_stroke(const color_t& color, bool closed, float thickness) { draw_poly_line(path, color, closed, thickness); path.clear(); }

			void draw_text(std::string_view str, vec2_t pos, const color_t& color, e_text_flags flags = e_text_flags{ }, c_font* font = nullptr, float size = 0.f);
			void draw_text(multicolor_text_t str, vec2_t pos, e_text_flags flags = e_text_flags{ }, c_font* font = nullptr, float size = 0.f);
			void draw_text(std::string_view str, const color_t& color, const vec2_t& pos, vec2_t& draw_pos, c_font* font, const float& line_height, const float& scale, int& vtx_offset, bool outline);
			void draw_line(const vec2_t& a, const vec2_t& b, const color_t& color, float thickness = 1.f);
			void draw_rect(const vec2_t& a, const vec2_t& b, const color_t& color, float thickness = 1.f, float rounding = 0.f, e_corner_flags flags = e_corner_flags::all); //@todo: add rect multicolor
			void draw_rect_filled(const vec2_t& a, const vec2_t& b, const color_t& color, float rounding = 0.f, e_corner_flags flags = e_corner_flags::all); //@todo: add rect filled multicolor
			void draw_convex_poly_filled(const std::vector<vec2_t>& points, const color_t& color);
			void draw_poly_line(const std::vector<vec2_t>& points, const color_t& color, bool closed, float thickness = 1.f);
			void draw_circle(const vec2_t& center, const color_t& clr, float radius, int num_segments = 0, float thickness = 1.f);
			void draw_circle_filled(const vec2_t& center, const color_t& clr, float radius, int num_segments = 0);
		};

		inline c_draw_list::draw_data_t draw_data{ };
		inline c_draw_list background_layer{ }, foreground_layer{ };

		//custom_layers does not change automatically, needed in order to create your own c_draw_list without having to change the code in begin_frame and setup_draw_data
		//fast_layers is cleared after each call to begin_frame and setup_draw_data. Will be drawn between background_layer and custom_layers
		inline std::vector<c_draw_list*> custom_layers{ }, fast_layers{ };
	}
}