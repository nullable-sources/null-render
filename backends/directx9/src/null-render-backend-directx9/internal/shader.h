#pragma once
#include "../wrapper/vertex/vertex.h"
#include "../wrapper/pixel/pixel.h"

namespace null::render::directx9 {
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

			const vec2_t<float> texel_offset = vec2_t<float>(-1.f, 1.f) / shared::viewport;
			vertex_shader->set_constant(0, texel_offset.coordinates.data(), 1);
		}

	public:
		virtual bool empty() const override { return (!pixel_shader || pixel_shader->empty()) || (!vertex_shader || vertex_shader->empty()); }
	};

	class c_default_shader : public c_shader {
	public: using c_shader::c_shader;
		virtual void use() override {
			if(empty()) return;
			c_shader::use();

			vertex_shader->set_constant(1, backend::renderer->get_matrix().linear_array.data(), 4);
			vertex_shader->set_constant(5, backend::renderer->get_translation().coordinates.data());
		}
	};
}