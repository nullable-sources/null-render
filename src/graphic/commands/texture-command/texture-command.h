#pragma once
#include <graphic/commands/commands.h>

namespace null::render::commands {
	class c_texture : public i_command {
	public:
		void* texture{ };

	public:
		c_texture(void* _texture) : texture{ _texture } { }

	public:
		void handle() override;
	};
}