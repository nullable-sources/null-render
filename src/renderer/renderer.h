#pragma once
#include <geometry-buffer/geometry-buffer.h>

namespace null::renderer {
	struct compiled_geometry_data_t {
	public:
		std::vector<render::c_geometry_buffer*> geometry_buffers{ };
		int total_idx_count{ }, total_vtx_count{ };

	public:
		void add_geometry_buffer(render::c_geometry_buffer* geometry_buffer);
		void setup();
	} inline compiled_geometry_data{ };

	void compile_default_geometry_data();

	class i_renderer {
	public:
		virtual void initialize() = 0;
		virtual void shutdown() = 0;

		virtual void begin_frame() = 0;
		virtual void end_frame() = 0;

		virtual void render(const compiled_geometry_data_t& _compiled_geometry_data = compiled_geometry_data) = 0;
		virtual void setup_state() = 0;

		virtual void create_objects() = 0;
		virtual void destroy_objects() = 0;
	} inline *renderer{ }; //note: perhaps need to unify work with shaders and replace the type with std::unique_ptr
}