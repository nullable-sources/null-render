#pragma once
#include "graphic/draw-list/brushes/text-brush/text-brush.h"

namespace null::render {
	struct sdf_text_brush_t : public text_brush_t {
	public:
		float outline_thickness{ };
		color_t<int> outline_start{ }, outline_end{ };

	public:
		auto&& set_outline(this auto&& self, float thickness, const color_t<int>& start, const color_t<int>& end) {
			self.outline_thickness = thickness; self.outline_start = start; self.outline_end = end;
			return self;
		}

	public:
		virtual std::unique_ptr<i_command> prepare_command(std::unique_ptr<c_geometry_command>& command) const override;
	};
}