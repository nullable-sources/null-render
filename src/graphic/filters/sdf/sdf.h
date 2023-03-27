#pragma once
#include <graphic/filters/texture/texture.h>

namespace null::render::filters {
	class c_sdf : public c_texture {
	public:
		float size{ }, thickness{ };
		color_t<int> outline_start{ }, outline_end{ };

	public:
		c_sdf(void* _texture, const float& _thickness, const float& _size, const color_t<int>& _outline_start, const color_t<int>& _outline_end)
			: c_texture{ _texture }, thickness{ _thickness }, size{ _size }, outline_start{ _outline_start }, outline_end{ _outline_end } { }

	public:
		virtual void handle() override;
	};
}