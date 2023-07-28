module;
#include <memory>
#include <vector>
#include <ranges>
#include <future>

#include <utils/fast_operators.h>
export module null.render:graphic.draw_list.brushes.sdf_text;

import null.sdk;

import :font;
import :font.atlas;
import :graphic.draw_list.brushes.text;
import :graphic.filters.sdf;

export namespace null::render {
	struct sdf_text_brush_t : public text_brush_t {
	public:
		float outline_thickness{ };
		color_t<int> outline_start{ }, outline_end{ };

	public: //@todo: P0847
		auto& set_outline(const float& thickness, const color_t<int>& start, const color_t<int>& end) {
			outline_thickness = thickness; outline_start = start; outline_end = end;
			return *this;
		}

	public:
		virtual std::unique_ptr<i_command> prepare_command(std::unique_ptr<c_geometry_command>& command) const override {
			return std::make_unique<c_sdf_filter>(std::move(command), font->container_atlas->texture.data, outline_thickness, size, outline_start, outline_end);
		}
	};
}