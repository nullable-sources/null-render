#pragma once
#include <graphic/commands/commands.h>

namespace null::render::filters {
	class i_filter : public commands::i_command {
	public:
		std::unique_ptr<commands::i_command> child_command{ };

	public:
		i_filter(std::unique_ptr<commands::i_command>&& _child_command) : child_command{ std::move(_child_command) } { }
	};
}