#pragma once
#include "null-render/backend/shaders/sdf.h"
#include "../text/text.h"

namespace null::render {
	struct sdf_brush_t : public text_brush_t {
	public:
		backend::i_sdf_shader::constants_t constants{ };

	public:
		template <typename self_t>
		auto&& set_outline(this self_t&& self, float thickness, const color_t<int>& start, const color_t<int>& end) {
			self.constants.outline_thickness = thickness; self.constants.outline_start = start; self.constants.outline_end = end;
			return self;
		}

	public:
		virtual std::unique_ptr<i_command> prepare_command(std::unique_ptr<c_geometry_command>& command) const override;
	};
}