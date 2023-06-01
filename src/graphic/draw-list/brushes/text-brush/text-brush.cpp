#include <graphic/filters/texture/texture.h>
#include <graphic/draw-list/brushes/text-brush/text-brush.h>

namespace null::render {
	std::unique_ptr<commands::i_command> text_brush_t::prepare_command(std::unique_ptr<commands::c_geometry>& command) const {
		return std::make_unique<filters::c_texture>(std::move(command), font->container_atlas->texture.data);
	};
}