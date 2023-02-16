#include <geometry-buffer/geometry-buffer.h>

namespace null::render {
	template <typename char_t>
	void c_geometry_buffer::add_text(const std::basic_string_view<char_t>& str, const color_t<int>& color, vec2_t<float>& pos, float& new_line_pos, c_font* font, const float& size, int& vtx_offset, e_text_flags flags) {
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
						for(const vec2_t<float>& offset : settings.text_outline_offsets) {
							add_idx(geometry_utils::quad_indexes, vtx_buffer.size());

							rect_t pos{ corners + offset };
							add_vtx(std::prev(vtx_buffer.end(), vtx_offset), geometry_utils::build_rect_vertex(pos.min, pos.max, uvs.min, uvs.max, color_t<int>::palette_t::black));
						}
					}

					//@note: main text
					add_idx(geometry_utils::quad_indexes, vtx_buffer.size());
					add_vtx(geometry_utils::build_rect_vertex(corners.min, corners.max, uvs.min, uvs.max, color));

					//@note:	necessary for the correct drawing order of the outline and body text.
					//			because the outline draw call runs parallel to the body text draw calls, glyphs of new letters can overlap past ones.
					vtx_offset += 4;
				}
			}
			pos.x += glyph->advance_x * (size / font->size);
		}
	}

	template <typename string_view_t>
	void c_geometry_buffer::add_text(const string_view_t& str, vec2_t<float> pos, const color_t<int>& color, e_text_flags flags, c_font* font, float size) {
		if(!font) return;
		if(size <= 0) size = font->size;

		bool _push_texture{ cmd_buffer.back().texture != font->container_atlas->texture.data };
		if(_push_texture) push_texture(font->container_atlas->texture.data);

		int vtx_offset{ }; //@note: offset for outline
		float new_line_pos{ pos.x };
		add_text(std::basic_string_view{ str }, color, pos, new_line_pos, font, size, vtx_offset, flags);

		if(_push_texture) pop_texture();
	}

	template <typename string_t>
	void c_geometry_buffer::add_text(const multicolor_text_t<string_t>& str, vec2_t<float> pos, e_text_flags flags, c_font* font, float size) {
		if(!font) return;
		if(size <= 0) size = font->size;

		bool _push_texture{ cmd_buffer.back().texture != font->container_atlas->texture.data };
		if(_push_texture) push_texture(font->container_atlas->texture.data);

		int vtx_offset{ }; //@note: offset for outline
		float new_line_pos{ pos.x };
		std::ranges::for_each(str.data, [&](const auto& data) {
			add_text<string_t::value_type>(data.first, data.second, pos, new_line_pos, font, size, vtx_offset, flags);
			});

		if(_push_texture) pop_texture();
	}
}