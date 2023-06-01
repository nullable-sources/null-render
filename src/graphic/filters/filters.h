#pragma once
#include <graphic/commands/geometry-command/geometry-command.h>

namespace null::render::filters {
	class i_filter : public commands::i_command {
	public:
		std::unique_ptr<commands::i_command> child_command{ };

	public:
		i_filter(std::unique_ptr<commands::i_command>&& _child_command) : child_command{ std::move(_child_command) } { }
	};

	class i_filter_instancer {
	public:
		virtual std::unique_ptr<i_filter> instance_filter(std::unique_ptr<commands::c_geometry>&& child_command) const = 0;

	public:
		virtual i_filter_instancer* clone() const = 0;
	};
}