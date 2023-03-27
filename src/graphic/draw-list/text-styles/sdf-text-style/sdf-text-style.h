#pragma once
#include <graphic/draw-list/text-styles/text-style/text-style.h>

namespace null::render {
	struct sdf_text_style_t : public text_style_t {
	public:
		float outline_thickness{ };
		color_t<int> outline_start{ }, outline_end{ };

	public:
		template <typename self_t> auto&& set_outline(this self_t&& self, const float& thickness, const color_t<int>& start, const color_t<int>& end) {
			self.outline_thickness = thickness; self.outline_start = start; self.outline_end = end;
			return self;
		}

	public:
		virtual std::unique_ptr<commands::i_command> prepare_command(std::unique_ptr<commands::c_geometry>& command) const override;
	};
}