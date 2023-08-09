#pragma once
#include "backend/shaders/sdf-shader.h"
#include "graphic/filters/texture-filter/texture-filter.h"

namespace null::render {
	class c_sdf_filter : public c_texture_filter {
	public:
		backend::i_sdf_shader::constants_t constants{ };

	public:
		c_sdf_filter(std::unique_ptr<i_command>&& _child_command, void* _texture, const backend::i_sdf_shader::constants_t& _constants)
			: c_texture_filter{ std::move(_child_command), _texture }, constants{ _constants } { }

		c_sdf_filter(std::unique_ptr<i_command>&& _child_command, void* _texture, const backend::i_sdf_shader::constants_t& _constants, float font_size)
			: c_texture_filter{ std::move(_child_command), _texture }, constants{ _constants } { constants.aa = font_size; }

	public:
		virtual void handle() override;
	};
}
