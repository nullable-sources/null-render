#pragma once
#include <geometry-buffer/geometry-buffer.h>

namespace null::render {
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

	class c_draw_list : public c_geometry_buffer {
	public:
		struct cmd_t {
			rect_t<float> clip_rect{ };
			void* texture{ };
			std::uint32_t vtx_offset{ }, idx_offset{ }, element_count{ };

			utils::callbacks_tuple_t<utils::callback_t<e_cmd_callbacks::on_draw_data, bool(c_draw_list::cmd_t&)>> callbacks{ };
		};

	public:
		std::vector<void*> textures{ };
		std::vector<rect_t<float>> clips{ };

		std::vector<cmd_t> cmd_buffer{ };

	public:
		void add_cmd() { cmd_buffer.push_back({ clips.back(), textures.back(), 0, (std::uint32_t)idx_buffer.size() }); }
		void clear() override { textures.clear(); clips.clear(); cmd_buffer.clear(); c_geometry_buffer::clear(); }
		void reset();

	public:
		void restore_clip_rect();
		void push_clip_rect(rect_t<float> rect, const bool& intersect_with_current_rect = false);
		void push_clip_rect(const vec2_t<float>& a, const vec2_t<float>& b, const bool& intersect_with_current_rect = false) { push_clip_rect(rect_t{ a, b }, intersect_with_current_rect); }
		void pop_clip_rect() { clips.pop_back(); on_change_clip_rect(); }

		void restore_texture() { push_texture(atlas.texture.data); }
		void push_texture(void* texture) { textures.push_back(texture); on_change_texture(); }
		void pop_texture() { textures.pop_back(); on_change_texture(); }

	public:
		void add_image(void* texture, const vec2_t<float>& a, const vec2_t<float>& b, const vec2_t<float>& uv_min, const vec2_t<float>& uv_max, const color_t<int>& color);
		void add_image(void* texture, const rect_t<float>& rect, const rect_t<float>& uvs, const color_t<int>& color) { add_image(texture, rect.min, rect.max, uvs.min, uvs.max, color); }
		void add_image_quad(void* texture, const std::array<std::pair<vec2_t<float>, vec2_t<float>>, 4>& points_and_uvs, const color_t<int>& color);

		template <typename char_t> void add_text(const std::basic_string_view<char_t>& str, const color_t<int>& color, vec2_t<float>& pos, float& new_line_pos, c_font* font, const float& size, int& vtx_offset, e_text_flags flags);
		template <typename string_view_t> void add_text(const string_view_t& str, vec2_t<float> pos, const color_t<int>& color, e_text_flags flags = e_text_flags{ }, c_font* font = c_font::get_current_font(), float size = 0.f);
		template <typename string_t> void add_text(const multicolor_text_t<string_t>& str, vec2_t<float> pos, e_text_flags flags = e_text_flags{ }, c_font* font = c_font::get_current_font(), float size = 0.f);

	public:
		void on_add_idx(const std::vector<std::uint32_t>::iterator& place, const std::vector<std::uint32_t>& buffer) override { cmd_buffer.back().element_count += buffer.size(); }

	public:
		virtual void on_change_clip_rect();
		virtual void on_change_texture();
	};

	inline c_draw_list background{ }, foreground{ };
	inline std::vector<c_draw_list*> custom_lists{ }, fast_lists{ };
}

#include <draw-list/draw-list.inl>