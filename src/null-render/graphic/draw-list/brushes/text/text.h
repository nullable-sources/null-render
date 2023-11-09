#pragma once
#include "null-render/font/font.h"
#include "../brush.h"

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
		template <typename self_t> self_t& set_font(this self_t& self, c_font* font) { self.font = font; return self; }
		template <typename self_t> self_t& set_size(this self_t& self, float size) { self.size = size; return self; }
		template <typename self_t> self_t& set_align(this self_t& self, e_text_align align) { self.align = align; return self; }

	public:
		virtual std::unique_ptr<i_command> prepare_command(std::unique_ptr<c_geometry_command>& command) const override;
	};
}