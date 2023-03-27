#pragma once
#include <graphic/commands/commands.h>

namespace null::render::commands {
	class c_clip : public i_command {
	public:
		rect_t<float> rect{ };

	public:
		c_clip(const rect_t<float>& _rect) : rect{ _rect } { }

	public:
		void handle() override;
	};
}