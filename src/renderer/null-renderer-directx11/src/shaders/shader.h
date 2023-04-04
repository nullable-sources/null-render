#pragma once
#include <null-renderer-directx11.h>
#include <renderer/shaders/shader.h>

#include <wrapper/constant-buffer/constant-buffer.h>
#include <wrapper/shaders/pixel-shader/pixel-shader.h>
#include <wrapper/shaders/vertex-shader/vertex-shader.h>

namespace null::renderer::shaders {
	/*namespace sources {
		namespace pixel_sampler {
			#include "pixel-sampler/compiled/pixel-sampler.h"
			static std::vector<std::uint8_t> data() {
				return std::vector<std::uint8_t>{ shader_data, shader_data + sizeof(shader_data) };
			}
		}

		namespace pixel_without_sampler {
			#include "pixel-without-sampler/compiled/pixel-without-sampler.h"
			static std::vector<std::uint8_t> data() {
				return std::vector<std::uint8_t>{ shader_data, shader_data + sizeof(shader_data) };
			}
		}

		namespace vertex {
			#include "vertex/compiled/vertex.h"
			static std::vector<std::uint8_t> data() {
				return std::vector<std::uint8_t>{ shader_data, shader_data + sizeof(shader_data) };
			}
		}
	}*/

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