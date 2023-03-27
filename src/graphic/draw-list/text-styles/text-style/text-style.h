#pragma once
#include <font/font.h>
#include <graphic/draw-list/brushes/brush.h>

namespace null::render {
	struct text_style_t : public brush_t {
	public:
		enum class e_align {
			none,
			aligin_right = 1 << 0, //@note: will be draw at pos - text_size.x
			aligin_bottom = 1 << 1, //@note: will be draw at pos - text_size.y
			//@note: align_center is calculated after the rest, so it can be combined with left and bottom
			aligin_center_x = 1 << 2,
			aligin_center_y = 1 << 3,
			aligin_center = aligin_center_x | aligin_center_y,
		};

	public:
		c_font* font{ get_default_font() };
		float size{ font->size };
		e_align align{ };

	public:
		template <typename self_t> auto&& set_font(this self_t&& self, c_font* font) { self.font = font; return self; }
		template <typename self_t> auto&& set_size(this self_t&& self, const float& size) { self.size = size; return self; }
		template <typename self_t> auto&& set_align(this self_t&& self, const e_align& align) { self.align = align; return self; }

	public:
		virtual std::unique_ptr<commands::i_command> prepare_command(std::unique_ptr<commands::c_geometry>& command) const override;
	};

	enum_create_bit_operators(text_style_t::e_align, true);
	enum_create_cast_operator(text_style_t::e_align, -);
}