#pragma once
#include <graphic/commands/commands.h>

namespace null::render::commands {
	class c_rtt_to_postprocess : public i_command {
	public:
		void handle() override;
	};

	class c_backbuffer_to_postprocess : public i_command {
	public:
		void handle() override;
	};
}