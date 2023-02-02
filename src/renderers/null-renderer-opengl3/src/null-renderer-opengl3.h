#pragma once
#ifdef null_renderer_use_glfw
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#endif

#include <opengl/opengl3.h>

#include <null-render.h>

namespace null::renderer {
	struct vertex_t {
		vec2_t<float> pos{ }, uv{ };
		std::uint32_t color{ };
	};

	inline std::uint32_t shader_program{ }, vertex_shader{ }, frag_shader{ };
	inline int attribute_texture{ }, attribute_proj_mtx{ };
	inline std::uint32_t attribute_position{ }, attribute_uv{ }, attribute_color{ };
	inline std::uint32_t vbo_handle{ }, elements_handle{ };
	inline std::uint32_t font_texture{ };

	void render(draw_data_t& _draw_data = draw_data);
	void setup_state(const std::uint32_t& vertex_array_object);
	bool create_fonts_texture();
	void destroy_fonts_texture();
	bool create_device_objects();
	void destroy_device_objects();

	static void begin_frame() { if(!shader_program) create_device_objects(); }

#ifdef null_renderer_use_glfw
	class c_window : public utils::win::c_window {
	public: using utils::win::c_window::c_window;
		color_t<float> clear_color{ 0.07f, 0.07f, 0.07f };
		GLFWwindow* glfw_window{ };

	public:
		bool create() override {
			if(!glfwInit()) throw std::runtime_error{ "cant init glfw" };

			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

			if(!(glfw_window = glfwCreateWindow(size.x, size.y, name.c_str(), NULL, NULL))) throw std::runtime_error{ "cant create glfw window" };
			glfwSetWindowPos(glfw_window, pos.x, pos.y);
			glfwMakeContextCurrent(glfw_window);
			glfwSwapInterval(1);

			wnd_handle = glfwGetWin32Window(glfw_window);

			on_create();
			return true;
		}

		void destroy() override {
			on_destroy();
			glfwDestroyWindow(glfw_window);
			glfwTerminate();
		}

		void main_loop() override {
			if(!wnd_handle) throw std::runtime_error{ "window handle is nullptr" };

			while(!glfwWindowShouldClose(glfw_window)) {
				glfwPollEvents();

				begin_frame();

				on_main_loop();

				setup_default_draw_data();

				vec2_t<int> framebuffer{ };
				glfwGetFramebufferSize(glfw_window, &framebuffer.x, &framebuffer.y);
				glViewport(0, 0, framebuffer.x, framebuffer.y);
				glClearColor(clear_color.r(), clear_color.g(), clear_color.b(), clear_color.a());
				glClear(GL_COLOR_BUFFER_BIT);
				render();

				glfwSwapBuffers(glfw_window);
			}
		}
	};
#endif
}