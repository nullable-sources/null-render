#include "graphic/filters/sdf-filter/sdf-filter.h"
#include "graphic/draw-list/brushes/sdf-text-brush/sdf-text-brush.h"

namespace null::render {
	std::unique_ptr<i_command> sdf_text_brush_t::prepare_command(std::unique_ptr<c_geometry_command>& command) const {
		return std::make_unique<c_sdf_filter>(std::move(command), font->container_atlas->texture.data, constants, size);
	};
}