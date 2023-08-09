#pragma once
#include <wrapper/vertex/vertex.h>
#include <wrapper/pixel/pixel.h>
#include <backend/shaders/shader.h>

namespace null::render::directx9 {
	class c_shader : public virtual backend::i_shader {
	public:
		c_pixel_shader* pixel_shader{ };
		c_vertex_shader* vertex_shader{ };

	public:
		c_shader(c_pixel_shader* _pixel_shader, c_vertex_shader* _vertex_shader) : pixel_shader{ _pixel_shader }, vertex_shader{ _vertex_shader } { }

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