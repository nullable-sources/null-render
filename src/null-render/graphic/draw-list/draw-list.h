#pragma once
#include "brushes/sdf-text/sdf-text.h"
#include "pen/pen.h"
#include "strokes/stroke.h"

#include "../filters/linear-gradient/linear-gradient.h"
#include "../filters/quad-gradient/quad-gradient.h"
#include "../filters/sdf/sdf.h"
#include "../filters/texture/texture.h"

#include "../path/path.h"

namespace null::render {
	class c_draw_list : public c_command_buffer {
	public:
		void stroke_path(const path_t& path, const brush_t& brush);
		void fill_path(const path_t& path, const brush_t& brush);

		void add_poly_line(const std::vector<vec2_t<float>>& points, const stroke_t& stroke, const brush_t& brush, const pen_t& pen = { });
		void add_convex_shape(const std::vector<vec2_t<float>>& points, const brush_t& brush, const pen_t& pen = { });

		template <typename string_t> void add_text(const string_t& text, vec2_t<float> pos, const text_brush_t& text_brush) { add_text(std::basic_string_view(text), pos, text_brush); }
		template <typename char_t> void add_text(std::basic_string_view<char_t> text, vec2_t<float> pos, const text_brush_t& text_brush);
	};

	inline c_draw_list background{ }, foreground{ };
}

#include "draw-list.inl"