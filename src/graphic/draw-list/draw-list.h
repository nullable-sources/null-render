#pragma once
#include <graphic/draw-list/brushes/brush.h>
#include <graphic/draw-list/text-styles/text-style/text-style.h>
#include <graphic/draw-list/strokes/stroke.h>
#include <graphic/commands/commands.h>

namespace null::render {
	class c_draw_list : public commands::c_command_buffer {
	public:
		void add_rect(const vec2_t<float>& a, const vec2_t<float>& b, const brush_t& brush) { add_convex_shape({ a, { b.x, a.y }, b, { a.x, b.y } }, brush); }

		void add_poly_line(const std::vector<vec2_t<float>>& points, const brush_t& brush, const stroke_t& stroke);
		void add_convex_shape(const std::vector<vec2_t<float>>& points, const brush_t& brush);
		void add_text(const std::string_view& text, const vec2_t<float>& pos, const text_style_t& text_style);
	};

	inline c_draw_list background{ }, foreground{ };
}