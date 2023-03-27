#pragma once
#include <graphic/commands/commands.h>

namespace null::render::commands {
	class c_setup_state : public i_command {
	public:
		void handle() override;
	};
}