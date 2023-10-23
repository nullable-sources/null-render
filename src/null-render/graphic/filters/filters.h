#pragma once
#include "../../commands/geometry/geometry.h"

namespace null::render {
	class i_filter : public i_command {
	public:
		std::unique_ptr<i_command> child_command{ };

	public:
		i_filter(std::unique_ptr<i_command>&& _child_command) : child_command(std::move(_child_command)) { }
	};

	class i_filter_instancer {
	public:
		virtual std::unique_ptr<i_filter> instance_filter(std::unique_ptr<c_geometry_command>&& child_command) const = 0;
	};
}