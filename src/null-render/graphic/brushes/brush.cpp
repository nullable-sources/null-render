#include "../commands/geometry/geometry.h"
#include "brush.h"

namespace null::render {
	std::shared_ptr<i_command>&& c_brush::prepare_command(std::shared_ptr<c_geometry_command>&& command) const {
		return std::move(command);
	}
}