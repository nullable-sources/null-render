#pragma once
#include "../wrapper/shaders/pixel.h"
#include "../wrapper/shaders/vertex.h"

namespace null::render::directx11 {
	class c_shader : public virtual backend::i_shader {
	public:
		c_pixel_shader* pixel_shader{ };
		c_vertex_shader* vertex_shader{ };

	public:
		c_shader(c_pixel_shader* _pixel_shader, c_vertex_shader* _vertex_shader) : pixel_shader(_pixel_shader), vertex_shader(_vertex_shader) { }

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