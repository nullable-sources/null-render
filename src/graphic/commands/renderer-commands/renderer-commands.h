#pragma once
#include "graphic/commands/commands.h"

namespace null::render {
	class c_setup_state_command : public i_command {
	public:
		void handle() override;
	};

	class c_texture_command : public i_command {
	public:
		void* texture{ };

	public:
		c_texture_command(void* _texture) : texture{ _texture } { }

	public:
		void handle() override;
	};

	class c_matrix_command : public i_command {
	public:
		matrix4x4_t matrix{ };

	public:
		c_matrix_command(const matrix4x4_t& _matrix) : matrix{ _matrix } { }

	public:
		void handle() override;
	};

	class c_clip_command : public i_command {
	public:
		rect_t<float> rect{ };

	public:
		c_clip_command(const rect_t<float>& _rect) : rect{ _rect } { }

	public:
		void handle() override;
	};
}