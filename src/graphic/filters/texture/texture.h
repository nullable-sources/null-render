#pragma once
#include <graphic/filters/filters.h>

namespace null::render::filters {
	class c_texture : public i_filter {
	public:
		void* texture{ };

	public:
		c_texture(void* _texture) : texture{ _texture } { }

	public:
		virtual void handle() override;
	};
}