#pragma once
#include <shaders/shaders.h>

namespace null::render::shaders::vertex {
	class c_shader : public c_vertex_shader {
	public:
		struct constant_t {
		public:
			float matrix[4][4]{ };

		public:
			constant_t() { }
			constant_t(const matrix4x4_t& _matrix) { std::ranges::move(_matrix.linear_array, &matrix[0][0]); }
		};

	public:
		c_native_wrapper native_wrapper{ };
		c_constant_wrapper<constant_t> constant_buffer{ };

	public:
		void edit_matrix(const matrix4x4_t& _matrix) { constant_buffer.edit_constant({ _matrix }); }

	public:
		void create() override {
			native_wrapper = c_native_wrapper{ sources::vertex::data() };
			constant_buffer = c_constant_wrapper<constant_t>{ };
			constant_buffer.create();
		}

		void destroy() override {
			native_wrapper.destroy();
			constant_buffer.destroy();
		}

		void setup_state() override {
			edit_matrix(matrix4x4_t::project_ortho(0.f, render::shared::viewport.x, render::shared::viewport.y, 0.f, -10000.f, 10000.f));
			set();
		}

		void set() override {
			native_wrapper.set();
			constant_buffer.set();
		}

	public:
		virtual operator bool() const override { return native_wrapper && constant_buffer; }
	} inline shader{ };
}