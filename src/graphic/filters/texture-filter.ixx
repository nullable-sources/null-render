module;
#include <limits>
#include <memory>
export module null.render:graphic.filters.texture;

import null.sdk;

import :graphic.filter;
import :graphic.command;
import :graphic.commands.geometry;

namespace null::render {
	class c_texture_filter : public i_filter {
	public:
		void* texture{ };

	public:
		c_texture_filter(std::unique_ptr<i_command>&& _child_command, void* _texture)
			: i_filter{ std::move(_child_command) }, texture{ _texture } { }

	public:
		virtual void handle() override {
			backend::renderer->set_texture(texture);
			backend::shaders::passthrough_texture->use();

			child_command->handle();

			backend::shaders::passthrough_color->use();
			backend::renderer->set_texture(nullptr);
		}
	};

	struct texture_filter_t : public i_filter_instancer {
	public:
		rect_t<float> uvs{ std::numeric_limits<float>::max() };
		void* texture{ };

	public: //@todo: P0847
		auto& set_texture(void* _texture) { texture = _texture; return *this; }
		auto& set_uvs(const rect_t<float>& _uvs) { uvs = _uvs; return *this; }

	public:
		std::unique_ptr<i_filter> instance_filter(std::unique_ptr<c_geometry_command>&& child_command) const override {
			if(uvs != std::numeric_limits<float>::max()) child_command->set_uvs(uvs);
			return std::make_unique<c_texture_filter>(std::move(child_command), texture);
		}
	};
}