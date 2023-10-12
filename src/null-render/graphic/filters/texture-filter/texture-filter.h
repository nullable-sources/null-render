#pragma once
#include "../filters.h"

namespace null::render {
	class c_texture_filter : public i_filter {
	public:
		void* texture{ };

	public:
		c_texture_filter(std::unique_ptr<i_command>&& _child_command, void* _texture)
			: i_filter{ std::move(_child_command) }, texture { _texture } { }

	public:
		virtual void handle() override;
	};

	struct texture_filter_t : public i_filter_instancer {
	public:
		rect_t<float> uvs{ std::numeric_limits<float>::max() };
		void* texture{ };

	public:
		auto&& set_texture(this auto&& self, void* texture) { self.texture = texture; return self; }
		auto&& set_uvs(this auto&& self, const rect_t<float>& uvs) { self.uvs = uvs; return self; }

	public:
		std::unique_ptr<i_filter> instance_filter(std::unique_ptr<c_geometry_command>&& child_command) const override {
			if(uvs != std::numeric_limits<float>::max()) child_command->set_uvs(uvs);
			return std::make_unique<c_texture_filter>(std::move(child_command), texture);
		}
	};
}