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

	struct texture_t : public i_filter_instancer {
	public:
		rect_t<float> uvs{ std::numeric_limits<float>::max() };
		void* texture{ };

	public:
		template <typename self_t> auto&& set_texture(this self_t&& self, void* texture) { self.texture = texture; return self; }
		template <typename self_t> auto&& set_uvs(this self_t&& self, const rect_t<float>& uvs) { self.uvs = uvs; return self; }

	public:
		std::unique_ptr<i_filter> instance_filter(std::unique_ptr<commands::c_geometry>&& child_command) const override {
			if(uvs != std::numeric_limits<float>::max()) child_command->set_uvs(uvs);
			return std::make_unique<c_texture>(std::move(child_command), texture);
		}
	};
}