#pragma once
#ifdef null_renderer_use_glfw
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#endif

#include <opengl/opengl3.h>

#include <null-render.h>

namespace null::renderer {
	class c_opengl3 : public i_renderer {
	public:
		struct vertex_t {
			vec2_t<float> pos{ }, uv{ };
			std::uint32_t color{ };
		};

	public:
		std::uint32_t vbo_handle{ }, elements_handle{ };
		std::uint32_t font_texture{ };

		std::uint32_t vertex_array_object{ };

	public:
		void initialize() override { }
		void shutdown() override { }

		void begin_frame() override { if(!font_texture) create_objects(); }
		void end_frame() override { }

		void render(const compiled_geometry_data_t& _compiled_geometry_data = compiled_geometry_data) override;
		void setup_state() override;

		void create_objects() override;
		void destroy_objects() override;

	public:
		bool create_fonts_texture();
		void destroy_fonts_texture();
	}; inline std::unique_ptr<c_opengl3> opengl3{ };

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
			glfwSwapInterval(0);

			wnd_handle = glfwGetWin32Window(glfw_window);

			opengl3 = std::make_unique<c_opengl3>();
			renderer = opengl3.get();

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

				on_main_loop();

				compile_default_geometry_data();

				vec2_t<int> framebuffer{ };
				glfwGetFramebufferSize(glfw_window, &framebuffer.x, &framebuffer.y);
				glViewport(0, 0, framebuffer.x, framebuffer.y);
				glClearColor(clear_color.r(), clear_color.g(), clear_color.b(), clear_color.a());
				glClear(GL_COLOR_BUFFER_BIT);
				renderer->render();

				glfwSwapBuffers(glfw_window);
			}
		}
	};
#endif
}