#include <graphic/filters/sdf/sdf.h>
#include <graphic/draw-list/brushes/sdf-text-brush/sdf-text-brush.h>

namespace null::render {
	std::unique_ptr<commands::i_command> sdf_text_brush_t::prepare_command(std::unique_ptr<commands::c_geometry>& command) const {
		return std::make_unique<filters::c_sdf>(std::move(command), font->container_atlas->texture.data, outline_thickness, size, outline_start, outline_end);
	};
}