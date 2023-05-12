#include <graphic/filters/linear-gradient/linear-gradient.h>
#include <graphic/draw-list/brushes/linear-gradient-brush/linear-gradient-brush.h>

namespace null::render {
	std::unique_ptr<commands::i_command> linear_gradient_brush_t::prepare_command(std::unique_ptr<commands::c_geometry>& command) const {
		if(stops.size() < 2 || stops.size() > 16) {
			utils::logger.log(utils::e_log_type::error, "the number of stops should be in the range from 2 to 16.");
			return std::move(command);
		}

		vec2_t min{ }, max{ };
		command->get_bounding_box(min, max);
		command->recalculate_uvs(min, max);

		return std::make_unique<filters::c_linear_gradient>(std::move(command), angle, stops);
	}
}