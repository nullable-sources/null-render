#pragma once
#include <backend/shaders/sdf/sdf.h>
#include <graphic/filters/texture/texture.h>

namespace null::render {
	class c_sdf_filter : public c_texture_filter {
	public:
		backend::i_sdf_shader::constants_t constants{ };

	public:
		c_sdf_filter(std::unique_ptr<i_command>&& _child_command, void* _texture, float outline_thickness, float _size, const color_t<int>& outline_start, const color_t<int>& outline_end)
			: c_texture_filter{ std::move(_child_command), _texture }, constants{ outline_start, outline_end, _size, outline_thickness } { }

	public:
		virtual void handle() override;
	};
}
