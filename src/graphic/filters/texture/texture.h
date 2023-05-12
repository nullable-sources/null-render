#pragma once
#include <graphic/filters/filters.h>

namespace null::render::filters {
	class c_texture : public i_filter {
	public:
		void* texture{ };

	public:
		c_texture(std::unique_ptr<commands::i_command>&& _child_command, void* _texture)
			: i_filter{ std::move(_child_command) }, texture { _texture } { }

	public:
		virtual void handle() override;
	};
}