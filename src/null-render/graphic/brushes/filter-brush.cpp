#include "filter-brush.h"

namespace null::render {
	std::shared_ptr<i_command> c_filter_brush::prepare_command(std::shared_ptr<c_geometry_command>&& command) const {
		std::shared_ptr<i_filter> new_filter = filter->clone();
		new_filter->set_child_command(command);
		return new_filter;
	}
}