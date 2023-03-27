#include <graphic/filters/quad-gradient/quad-gradient.h>
#include <graphic/draw-list/brushes/quad-gradient-brush/quad-gradient-brush.h>

namespace null::render {
	std::unique_ptr<commands::i_command> quad_gradient_brush_t::prepare_command(std::unique_ptr<commands::c_geometry>& command) const {
		vec2_t min{ }, max{ };
		command->get_bounding_box(min, max);
		command->recalculate_uvs(min, max);

		std::unique_ptr<filters::c_quad_gradient> filter{ std::make_unique<filters::c_quad_gradient>(colors) };
		filter->child_command = std::move(command);
		return std::move(filter);
	}
}