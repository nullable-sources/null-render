#pragma once
#include <wrapper/vertex/vertex.h>
#include <wrapper/pixel/pixel.h>
#include <backend/shaders/shader.h>

namespace null::render::backend::directx9::shaders {
	class i_shader : public virtual backend::shaders::i_shader {
	public:
		wrapper::c_pixel_shader* pixel_shader{ };
		wrapper::c_vertex_shader* vertex_shader{ };

	public:
		i_shader(wrapper::c_pixel_shader* _pixel_shader, wrapper::c_vertex_shader* _vertex_shader) : pixel_shader{ _pixel_shader }, vertex_shader{ _vertex_shader } { }

	public:
		void on_create() override { }
		void on_destroy() override { }

	public:
		virtual void use() override {
			pixel_shader->set();
			vertex_shader->set();
		}

	public:
		virtual bool empty() const override { return (!pixel_shader || pixel_shader->empty()) || (!vertex_shader || vertex_shader->empty()); }
	};
}