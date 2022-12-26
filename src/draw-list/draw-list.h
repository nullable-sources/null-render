#pragma once
#include <font/font.h>

namespace null {
	namespace render {
		enum struct e_initialize_flags {
			anti_aliased_lines = 1 << 0,
			anti_aliased_lines_use_texture = 1 << 1,
			anti_aliased_fill = 1 << 2,
			allow_vtx_offset = 1 << 3
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

		enum class e_cmd_callbacks {
			on_draw_data
		};

		struct vertex_t {
			vec2_t pos{ }, uv{ };
			color_t<int> color{ };
		};

		class c_draw_list {
		public:
			struct settings_t {
			public:
				static constexpr int arc_fast_tessellation_multiplier{ 1 };
				static constexpr int circle_auto_segment_max{ 512 };

			public:
				e_initialize_flags initialize_flags{ };

				std::vector<vec2_t> text_outline_offsets{ { -1, 0 }, { 0, -1 }, { 0, 1 }, { 1, 0 } };
				std::array<vec2_t, 12 * arc_fast_tessellation_multiplier> arc_fast_vtx{ };
				std::array<std::uint8_t, 64> circle_segments{ };
				float circle_segment_max_error{ };
				float curve_tessellation_tol{ 1.25f };

				settings_t(e_initialize_flags _initialize_flags = e_initialize_flags{ }) : initialize_flags{ _initialize_flags } {
					std::ranges::for_each(std::views::iota(0, (int)arc_fast_vtx.size()), [=](const int& i) {
						float a{ float(i * 2.f * std::numbers::pi) / arc_fast_vtx.size() };
						arc_fast_vtx[i] = vec2_t{ std::cosf(a), std::sinf(a) };
						});
				}

				void set_circle_segment_max_error(float max_error) {
					if(circle_segment_max_error == max_error) return;
					circle_segment_max_error = max_error;

					std::ranges::for_each(std::views::iota(0, (int)circle_segments.size()), [=](const int& i) {
						float radius{ i + 1.f };
						circle_segments[i] = std::min(std::clamp((std::numbers::pi * 2.f) / std::acosf((radius - circle_segment_max_error) / radius), 12., 512.), 255.);
						});
				}

				void get_auto_circle_num_segments(int& num_segments, float radius) {
					if(num_segments <= 0) {
						const int radius_idx{ (int)radius - 1 };
						if(radius_idx < circle_segments.size()) num_segments = circle_segments[radius_idx];
						else num_segments = std::clamp((int)((std::numbers::pi * 2.0f) / acosf((radius - circle_segment_max_error) / radius)), 12, circle_auto_segment_max);
					} else num_segments = std::clamp(num_segments, 3, circle_auto_segment_max);
				}
			} settings{ e_initialize_flags{ -e_initialize_flags::allow_vtx_offset | -e_initialize_flags::anti_aliased_lines | -e_initialize_flags::anti_aliased_lines_use_texture | -e_initialize_flags::anti_aliased_fill } };

			struct cmd_t {
				rect_t clip_rect{ };
				void* texture{ };
				std::uint32_t vtx_offset{ }, idx_offset{ }, element_count{ };

				utils::callbacks_tuple_t<utils::callback_t<e_cmd_callbacks::on_draw_data, bool(c_draw_list::cmd_t&)>> callbacks{ };
			};

		public:
			std::vector<void*> textures{ };
			std::vector<vec2_t> pathes{ };
			std::vector<rect_t> clips{ };

			std::vector<cmd_t> cmd_buffer{ };

			std::vector<vertex_t> vtx_buffer{ };
			std::vector<std::uint16_t> idx_buffer{ };

		public:
			void add_cmd() { cmd_buffer.push_back({ clips.back(), textures.back(), 0, (std::uint32_t)idx_buffer.size() }); }
			void clear() { textures.clear(); pathes.clear(); clips.clear(); cmd_buffer.clear(); idx_buffer.clear(); vtx_buffer.clear(); }
			void reset();

		public:
			void restore_clip_rect();
			void push_clip_rect(rect_t rect, const bool& intersect_with_current_rect = false);
			void push_clip_rect(const vec2_t& a, const vec2_t& b, const bool& intersect_with_current_rect = false) { push_clip_rect(rect_t{ a, b }, intersect_with_current_rect); }
			void pop_clip_rect() { clips.pop_back(); on_change_clip_rect(); }

			void restore_texture() { push_texture(atlas.texture.data); }
			void push_texture(void* texture) { textures.push_back(texture); on_change_texture(); }
			void pop_texture() { textures.pop_back(); on_change_texture(); }

		public:
			void add_vtx(const std::vector<vertex_t>::iterator& place, const std::vector<vertex_t>& buffer) { on_change_vtx(buffer.size()); vtx_buffer.insert(place, buffer.begin(), buffer.end()); }
			void add_vtx(const std::vector<vertex_t>& buffer) { add_vtx(vtx_buffer.end(), buffer); }
			void add_idx(const std::vector<std::uint16_t>::iterator& place, const std::vector<std::uint16_t>& buffer) { cmd_buffer.back().element_count += buffer.size(); idx_buffer.insert(place, buffer.begin(), buffer.end()); }
			void add_idx(const std::vector<std::uint16_t>& buffer) { add_idx(idx_buffer.end(), buffer); }

			void add_rect(const vec2_t& a, const vec2_t& b, const color_t<int>& color) { add_rect_uv(a, b, atlas.texture.uv_white_pixel, atlas.texture.uv_white_pixel, color); }
			void add_rect_uv(const vec2_t& a, const vec2_t& b, const vec2_t& uv_a, const vec2_t& uv_b, const color_t<int>& color) { add_quad_uv({ std::pair{ a, uv_a }, std::pair{ vec2_t{ b.x, a.y }, vec2_t{ uv_b.x, uv_a.y } }, std::pair{ b, uv_b }, std::pair{ vec2_t{ a.x, b.y }, vec2_t{ uv_a.x, uv_b.y } } }, color); }
			void add_quad_uv(const std::array<std::pair<vec2_t, vec2_t>, 4>& points, const color_t<int>& color);

			void path_rect(const vec2_t& a, const vec2_t& b, float rounding = 0.0f, const e_corner_flags& flags = e_corner_flags::all);
			void path_arc_to_fast(const vec2_t& center, const float& radius, const int& a_min_of_12, const int& a_max_of_12);
			void path_arc_to(const vec2_t& center, const float& radius, const float& a_min, const float& a_max, const int& num_segments);
			void path_fill_convex(const color_t<int>& clr) { draw_convex_poly_filled(pathes, clr); pathes.clear(); }
			void path_stroke(const color_t<int>& color, const bool& closed, const float& thickness) { draw_poly_line(pathes, color, closed, thickness); pathes.clear(); }

			void repaint_rect_vertices_in_multicolor(const vec2_t& min, const vec2_t& max, const size_t& vtx_offset, const std::array<color_t<int>, 4>& colors);

		public:
			void draw_line(const vec2_t& a, const vec2_t& b, const color_t<int>& color, const float& thickness = 1.f);
			void draw_rect(const vec2_t& a, const vec2_t& b, const color_t<int>& color, const float& thickness = 1.f, const float& rounding = 0.f, const e_corner_flags& flags = e_corner_flags::all);
			void draw_rect(const rect_t& rect, const color_t<int>& color, const float& thickness = 1.f, const float& rounding = 0.f, const e_corner_flags& flags = e_corner_flags::all) { draw_rect(rect.min, rect.max, color, thickness, rounding, flags); }
			void draw_rect_multicolor(const vec2_t& a, const vec2_t& b, const std::array<color_t<int>, 4>& colors, const float& thickness = 1.f, const float& rounding = 0.f, const e_corner_flags& flags = e_corner_flags::all);
			void draw_rect_multicolor(const rect_t& rect, const std::array<color_t<int>, 4>& colors, const float& thickness = 1.f, const float& rounding = 0.f, const e_corner_flags& flags = e_corner_flags::all) { draw_rect_multicolor(rect.min, rect.max, colors, thickness, rounding, flags); }
			void draw_rect_filled(const vec2_t& a, const vec2_t& b, const color_t<int>& color, const float& rounding = 0.f, const e_corner_flags& flags = e_corner_flags::all);
			void draw_rect_filled(const rect_t& rect, const color_t<int>& color, const float& rounding = 0.f, const e_corner_flags& flags = e_corner_flags::all) { draw_rect_filled(rect.min, rect.max, color, rounding, flags); }
			void draw_rect_filled_multicolor(const vec2_t& a, const vec2_t& b, const std::array<color_t<int>, 4>& colors, float rounding = 0.f, const e_corner_flags& flags = e_corner_flags::all);
			void draw_rect_filled_multicolor(const rect_t& rect, const std::array<color_t<int>, 4>& colors, const float& rounding = 0.f, const e_corner_flags& flags = e_corner_flags::all) { draw_rect_filled_multicolor(rect.min, rect.max, colors, rounding, flags); }
			void draw_convex_poly_filled(const std::vector<vec2_t>& points, const color_t<int>& color);
			void draw_poly_line(const std::vector<vec2_t>& points, const color_t<int>& color, const bool& closed, float thickness = 1.f);
			void draw_circle(const vec2_t& center, const color_t<int>& clr, const float& radius, int num_segments = 0, const float& thickness = 1.f);
			void draw_circle_filled(const vec2_t& center, const color_t<int>& clr, const float& radius, int num_segments = 0);
			void draw_image(void* texture, const vec2_t& a, const vec2_t& b, const vec2_t& uv_min, const vec2_t& uv_max, const color_t<int>& color);
			void draw_image(void* texture, const rect_t& rect, const rect_t& uvs, const color_t<int>& color) { draw_image(texture, rect.min, rect.max, uvs.min, uvs.max, color); }
			void draw_image_quad(void* texture, const std::array<std::pair<vec2_t, vec2_t>, 4>& points_and_uvs, const color_t<int>& color);

			template <typename char_t>
			void draw_text(const std::basic_string_view<char_t>& str, const color_t<int>& color, vec2_t& pos, float& new_line_pos, c_font* font, const float& size, int& vtx_offset, e_text_flags flags) {
				if(color.a() <= 0) return;
				if(flags & e_text_flags::aligin_mask) {
					vec2_t str_size{ font->calc_text_size(str, size) };
					if(str_size <= 0.f) return;

					if(flags & e_text_flags::aligin_right) pos.x -= str_size.x;
					if(flags & e_text_flags::aligin_bottom) pos.y -= str_size.y;
					if(flags & e_text_flags::aligin_center_x) pos.x -= str_size.x / 2.f;
					if(flags & e_text_flags::aligin_center_y) pos.y -= str_size.y / 2.f;
					flags &= ~e_text_flags::aligin_mask;

					new_line_pos = pos.x;
				}

				pos = math::floor(pos);
				for(auto iterator{ str.begin() }; iterator != str.end();) {
					std::uint32_t symbol{ (std::uint32_t)*iterator };
					iterator += impl::char_converters::converter<char_t>::convert(symbol, iterator, str.end());
					if(!symbol) break;

					if(symbol == '\r') continue;
					if(symbol == '\n') {
						pos.x = new_line_pos;
						pos.y += size;
						if(pos.y > cmd_buffer.back().clip_rect.max.y)
							break;
						continue;
					}

					const c_font::glyph_t* glyph{ font->find_glyph((std::uint16_t)symbol) };
					if(!glyph) continue;

					if(glyph->visible) {
						rect_t corners{ rect_t{ pos } + glyph->corners * (size / font->size) };
						if(cmd_buffer.back().clip_rect.intersects(corners)) {
							rect_t uvs{ glyph->texture_coordinates };

							if(flags & e_text_flags::outline && !settings.text_outline_offsets.empty()) {
								for(const vec2_t& offset : settings.text_outline_offsets) {
									add_idx({
										(std::uint16_t)vtx_buffer.size(), (std::uint16_t)(vtx_buffer.size() + 1), (std::uint16_t)(vtx_buffer.size() + 2),
										(std::uint16_t)vtx_buffer.size(), (std::uint16_t)(vtx_buffer.size() + 2), (std::uint16_t)(vtx_buffer.size() + 3)
										});

									rect_t pos{ corners + offset };
									add_vtx(std::prev(vtx_buffer.end(), vtx_offset),
										{
											{ pos.min,                  uvs.min,                    { 0, 0, 0 } },
											{ { pos.max.x, pos.min.y }, { uvs.max.x, uvs.min.y },   { 0, 0, 0 } },
											{ pos.max,                  uvs.max,                    { 0, 0, 0 } },
											{ { pos.min.x, pos.max.y }, { uvs.min.x, uvs.max.y },   { 0, 0, 0 } }
										});
								}
							}
							add_rect_uv(corners.min, corners.max, uvs.min, uvs.max, color); //@note: main text

							//@note:	necessary for the correct drawing order of the outline and body text.
							//			because the outline draw call runs parallel to the body text draw calls, glyphs of new letters can overlap past ones.
							vtx_offset += 4;
						}
					}
					pos.x += glyph->advance_x * (size / font->size);
				}
			}

			template <typename string_view_t>
			void draw_text(const string_view_t& str, vec2_t pos, const color_t<int>& color, e_text_flags flags = e_text_flags{ }, c_font* font = c_font::get_current_font(), float size = 0.f) {
				if(!font) return;
				if(size <= 0) size = font->size;

				if(cmd_buffer.back().texture != font->container_atlas->texture.data)
					throw std::runtime_error{ "cmd_buffer.back().texture != font->container_atlas->texture.data" };

				int vtx_offset{ }; //@note: offset for outline
				draw_text(std::basic_string_view{ str }, color, pos, pos.x, font, size, vtx_offset, flags);
			}

			template <typename string_t>
			void draw_text(const multicolor_text_t<string_t>& str, vec2_t pos, e_text_flags flags = e_text_flags{ }, c_font* font = c_font::get_current_font(), float size = 0.f) {
				if(!font) return;
				if(size <= 0) size = font->size;

				if(cmd_buffer.back().texture != font->container_atlas->texture.data)
					throw std::runtime_error{ "cmd_buffer.back().texture != font->container_atlas->texture.data" };

				int vtx_offset{ }; //@note: offset for outline
				float new_line_pos{ pos.x };
				std::ranges::for_each(str.data, [&](const auto& data) {
					draw_text<string_t::value_type>(data.first, data.second, pos, new_line_pos, font, size, vtx_offset, flags);
					});
			}

		public:
			virtual void on_change_clip_rect();
			virtual void on_change_texture();
			virtual void on_change_vtx(const int& new_vtx_count);
		};

		inline c_draw_list background{ }, foreground{ };
		inline std::vector<c_draw_list*> custom_lists{ }, fast_lists{ };
	}

	namespace renderer {
		struct draw_data_t {
		public:
			static inline vec2_t screen_size{ };

		public:
			std::vector<render::c_draw_list*> draw_lists{ };
			int total_idx_count{ }, total_vtx_count{ };

		public:
			void add_draw_list(render::c_draw_list* draw_list);
			void setup();
		} inline draw_data;

		void setup_default_draw_data();
	}
}