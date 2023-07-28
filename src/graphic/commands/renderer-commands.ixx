export module null.render:graphic.commands.renderer;

import :graphic.command;
import :backend.renderer;

export namespace null::render {
	class c_texture_command : public i_command {
	public:
		void* texture{ };

	public:
		c_texture_command(void* _texture) : texture{ _texture } { }

	public:
		void handle() override {
			backend::renderer->set_texture(texture);
		}
	};

	class c_setup_state_command : public i_command {
	public:
		void handle() override {
			backend::renderer->setup_state();
		}
	};

	class c_set_matrix_command : public i_command {
	public:
		matrix4x4_t matrix{ };

	public:
		c_set_matrix_command(const matrix4x4_t& _matrix) : matrix{ _matrix } { }

	public:
		void handle() override {
			backend::renderer->set_matrix(matrix);
		}
	};

	class c_clip_command : public i_command {
	public:
		rect_t<float> rect{ };

	public:
		c_clip_command(const rect_t<float>& _rect) : rect{ _rect } { }

	public:
		void handle() override {
			backend::renderer->set_clip(rect);
		}
	};
}