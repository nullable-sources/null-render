#pragma once
#include <graphic/commands/commands.h>

namespace null::render::commands {
	class c_matrix : public i_command {
	public:
		matrix4x4_t matrix{ };

	public:
		c_matrix(const matrix4x4_t& _matrix) : matrix{ _matrix } { }

	public:
		void handle() override;
	};
}