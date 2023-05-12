#pragma once
#include <graphic/draw-list/brushes/brush.h>
#include <graphic/draw-list/pen/pen.h>
#include <graphic/draw-list/text-styles/text-style/text-style.h>
#include <graphic/draw-list/strokes/stroke.h>

#include <graphic/draw-list/brushes/linear-gradient-brush/linear-gradient-brush.h>
#include <graphic/draw-list/brushes/quad-gradient-brush/quad-gradient-brush.h>
#include <graphic/draw-list/brushes/texture-brush/texture-brush.h>
#include <graphic/draw-list/text-styles/sdf-text-style/sdf-text-style.h>

namespace null::render {
	class c_draw_list : public commands::c_command_buffer {
	public:
		void add_poly_line(const std::vector<vec2_t<float>>& points, const stroke_t& stroke, const brush_t& brush, const pen_t& pen = { });
		void add_convex_shape(const std::vector<vec2_t<float>>& points, const brush_t& brush, const pen_t& pen = { });

		template <typename string_t> void add_text(const string_t& text, vec2_t<float> pos, const text_style_t& text_style) { add_text(std::basic_string_view{ text }, pos, text_style); }
		template <typename char_t> void add_text(const std::basic_string_view<char_t>& text, vec2_t<float> pos, const text_style_t& text_style);
	};

	inline c_draw_list background{ }, foreground{ };
}

#include <graphic/draw-list/draw-list.inl>