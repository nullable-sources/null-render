#pragma once
#include "graphic/draw-list/brushes/text-brush/text-brush.h"
#include "backend/shaders/sdf-shader.h"

namespace null::render {
	struct sdf_text_brush_t : public text_brush_t {
	public:
		backend::i_sdf_shader::constants_t constants{ };

	public:
		auto&& set_outline(this auto&& self, float thickness, const color_t<int>& start, const color_t<int>& end) {
			self.constants.outline_thickness = thickness; self.constants.outline_start = start; self.constants.outline_end = end;
			return self;
		}

	public:
		virtual std::unique_ptr<i_command> prepare_command(std::unique_ptr<c_geometry_command>& command) const override;
	};
}