#pragma once
#include <wrapper/vertex/vertex.h>
#include <wrapper/pixel/pixel.h>
#include <renderer/shaders/shader.h>

namespace null::renderer::shaders {
	class i_shader : public virtual render::impl::shaders::i_shader {
	public:
		std::unique_ptr<wrapper::c_pixel_shader> pixel_shader{ };
		std::unique_ptr<wrapper::c_vertex_shader> vertex_shader{ };

	public:
		virtual void use() override {
			if(empty()) return;
			pixel_shader->set();
			vertex_shader->set();
		}

	public:
		virtual bool empty() const override { return (!pixel_shader || pixel_shader->empty()) || (!vertex_shader || vertex_shader->empty()); }
	};
}