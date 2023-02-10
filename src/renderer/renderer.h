#pragma once
#include <draw-list/draw-list.h>

namespace null::renderer {
	struct draw_data_t {
	public:
		static inline vec2_t screen_size{ };

	public:
		std::vector<render::c_draw_list*> draw_lists{ };
		int total_idx_count{ }, total_vtx_count{ };

	public:
		void add_draw_list(render::c_draw_list* draw_list);
		void setup();
	} inline draw_data{ };

	void setup_default_draw_data();

	class i_renderer {
	public:
		virtual void initialize() = 0;
		virtual void shutdown() = 0;

		virtual void begin_frame() = 0;
		virtual void end_frame() = 0;

		virtual void render(const draw_data_t& draw_data = draw_data) = 0;
		virtual void setup_state() = 0;

		virtual void create_objects() = 0;
		virtual void destroy_objects() = 0;
	} inline *renderer{ }; //note: perhaps need to unify work with shaders and replace the type with std::unique_ptr
}