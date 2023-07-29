#pragma once
#include <font/font.h>
#include <graphic/draw-list/brushes/brush.h>

namespace null::render {
	enum class e_text_align {
		none,
		right = 1 << 0, //@note: will be draw at pos - text_size.x
		bottom = 1 << 1, //@note: will be draw at pos - text_size.y
		//@note: align_center is calculated after the rest, so it can be combined with left and bottom
		center_x = 1 << 2,
		center_y = 1 << 3,
		center = center_x | center_y,
	}; enum_create_bit_operators(e_text_align, true);
	enum_create_cast_operator(e_text_align, -);

	struct text_brush_t : public brush_t {
	public:
		c_font* font{ get_default_font() };
		float size{ font->size };
		e_text_align align{ };

	public:
		auto&& set_font(this auto&& self, c_font* font) { self.font = font; return self; }
		auto&& set_size(this auto&& self, float size) { self.size = size; return self; }
		auto&& set_align(this auto&& self, e_text_align align) { self.align = align; return self; }

	public:
		virtual std::unique_ptr<commands::i_command> prepare_command(std::unique_ptr<commands::c_geometry>& command) const override;
	};
}