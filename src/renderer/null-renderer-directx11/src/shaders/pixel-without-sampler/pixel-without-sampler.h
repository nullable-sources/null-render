#pragma once
#include <shaders/shaders.h>

namespace null::render::shaders::pixel_without_sampler {
	class c_shader : public c_pixel_shader {
	public:
		c_native_wrapper native_wrapper{ };

	public:
		void create() override { native_wrapper = c_native_wrapper{ sources::pixel_without_sampler::data() }; }
		void destroy() override { native_wrapper.destroy(); }

		void setup_state() override { set(); }

		void set() override { native_wrapper.set(); }

	public:
		virtual operator bool() const override { return native_wrapper; }
	} inline shader{ };
}