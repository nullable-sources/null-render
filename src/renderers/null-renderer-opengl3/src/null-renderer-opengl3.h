#pragma once
#include <opengl/opengl3.h>

#include <null-render.h>

namespace null::renderer::opengl3 {
	struct vertex_t {
		vec2_t pos{ }, uv{ };
		std::uint32_t color{ };
	};

	inline std::uint32_t shader_program{ }, vertex_shader{ }, frag_shader{ };
	inline int attribute_texture{ }, attribute_proj_mtx{ };
	inline std::uint32_t attribute_position{ }, attribute_uv{ }, attribute_color{ };
	inline std::uint32_t vbo_handle{ }, elements_handle{ };
	inline std::uint32_t font_texture{ };

	void render_draw_data(render::c_draw_list::draw_data_t* draw_data = &render::draw_data);
	void setup_render_state(render::c_draw_list::draw_data_t* draw_data, std::uint32_t vertex_array_object);
	bool create_fonts_texture();
	void destroy_fonts_texture();
	bool create_device_objects();
	void destroy_device_objects();

	static void shutdown() { destroy_device_objects(); }
	static void begin_frame() { if(!shader_program) create_device_objects(); }
}