#include "graphic/filters/texture/texture.h"
#include "graphic/draw-list/brushes/text-brush/text-brush.h"

namespace null::render {
	std::unique_ptr<i_command> text_brush_t::prepare_command(std::unique_ptr<c_geometry_command>& command) const {
		return std::make_unique<c_texture_filter>(std::move(command), font->container_atlas->texture.data);
	};
}