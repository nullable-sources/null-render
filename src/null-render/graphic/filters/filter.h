#pragma once
#include "../commands/geometry/geometry.h"

namespace null::render {
	class i_filter : public i_command {
	public:
		std::shared_ptr<i_command> child_command{ };

	public:
		i_filter() { }
		virtual ~i_filter() { }

	public:
		virtual void set_child_command(std::shared_ptr<c_geometry_command>& _child_command) {
			child_command = std::move(_child_command);
		}
	};
}