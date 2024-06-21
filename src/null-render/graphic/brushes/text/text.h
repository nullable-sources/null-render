#pragma once
#include "../../../font/font.h"
#include "../../filters/texture/texture.h"
#include "../filter-brush.h"

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

    class i_text_brush : public c_brush {
    public:
        c_font* font{ get_default_font() };
        float size{ font->size };
        e_text_align align{ };

    public:
        void set_font(c_font* _font) { font = _font; }
        void set_size(float _size) { size = _size; }
        void set_align(e_text_align _align) { align = _align; }
    };

    class c_text_brush : public i_text_brush {
    public:
        static std::shared_ptr<c_text_brush> instance() { return std::make_shared<c_text_brush>(); }

    public:
        c_text_brush() { }
        virtual ~c_text_brush() { }

    public:
        virtual std::shared_ptr<i_command> prepare_command(std::shared_ptr<c_geometry_command>&& command) const override;
    };
}