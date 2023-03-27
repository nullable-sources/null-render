#pragma once
#include <graphic/commands/commands.h>

namespace null::render::filters {
	class i_filter : public commands::i_command {
	public:
		std::unique_ptr<commands::i_command> child_command{ };
	};
}