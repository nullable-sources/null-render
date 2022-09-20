#pragma once
#include <Windows.h>
#include <GL/gl3w.h>

#include <null-render.h>

namespace null::render::opengl3 {
	struct vertex_t {
		vec2_t pos{ }, uv{ };
		std::uint32_t color{ };
	};

	inline GLuint shader_program{ }, vertex_shader{ }, frag_shader{ };

	inline GLint attribute_texture{ }, attribute_proj_mtx{ };
	inline GLuint attribute_position{ }, attribute_uv{ }, attribute_color{ };

	inline GLuint vbo_handle{ }, elements_handle{ };

	inline GLuint font_texture{ };

	static void initialize() { gl3w::init(); }
	void shutdown();
	void begin_frame();

	void render_draw_data(c_draw_list::draw_data_t* _draw_data = &draw_data);
	void setup_render_state(c_draw_list::draw_data_t* _draw_data, GLuint vertex_array_object);
	bool create_fonts_texture();
	void destroy_fonts_texture();
	bool create_device_objects();
	void destroy_device_objects();
}