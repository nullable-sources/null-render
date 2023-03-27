#pragma once
#include <backend/shaders/shader.h>

#include <wrapper/constant-buffer/constant-buffer.h>
#include <wrapper/shaders/pixel-shader/pixel-shader.h>
#include <wrapper/shaders/vertex-shader/vertex-shader.h>

namespace null::render::backend::directx11::shaders {
	class i_shader : public virtual backend::shaders::i_shader {
	public:
		wrapper::c_pixel_shader* pixel_shader{ };
		wrapper::c_vertex_shader* vertex_shader{ };

	public:
		i_shader(wrapper::c_pixel_shader* _pixel_shader, wrapper::c_vertex_shader* _vertex_shader) : pixel_shader{ _pixel_shader }, vertex_shader{ _vertex_shader } { }

	public:
		virtual void on_create() override { }
		virtual void on_destroy() override { }

	public:
		virtual void use() override {
			pixel_shader->set();
			vertex_shader->set();
		}

	public:
		virtual bool empty() const override { return (!pixel_shader || pixel_shader->empty()) || (!vertex_shader || vertex_shader->empty()); }
	};
}