module;
#include <memory>

#include <utils/fast_operators.h>
export module null.render:graphic.draw_list.brushes.text;

import :font;
import :font.atlas;
import :graphic.draw_list.brush;

import :graphic.filters.texture;

export namespace null::render {
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

	public: //@todo: P0847
		auto& set_font(c_font* _font) { font = _font; return *this; }
		auto& set_size(const float& _size) { size = _size; return *this; }
		auto& set_align(const e_text_align& _align) { align = _align; return *this; }

	public:
		virtual std::unique_ptr<i_command> prepare_command(std::unique_ptr<c_geometry_command>& command) const override {
			return std::make_unique<c_texture_filter>(std::move(command), font->container_atlas->texture.data);
		}
	};
}