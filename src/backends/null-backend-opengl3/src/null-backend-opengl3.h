#pragma once
#ifdef null_renderer_use_glfw
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#endif

#include <null-render.h>
#include <backend/backend.h>

#include <renderer/renderer.h>
#include <internal/frame-buffer/frame-buffer.h>
#include <internal/mesh/mesh.h>
#include <shaders/passthrough-color/passthrough-color.h>
#include <shaders/passthrough-texture/passthrough-texture.h>
#include <shaders/quad-gradient/quad-gradient.h>
#include <shaders/linear-gradient/linear-gradient.h>
#include <shaders/sdf/sdf.h>

namespace null::render::backend::opengl3 {
	class c_factory : public i_factory {
	public:
		std::unique_ptr<i_renderer> instance_renderer() override { return std::make_unique<c_renderer>(); }
		std::unique_ptr<backend::c_mesh> instance_mesh() override { return std::make_unique<c_mesh>(); }
		std::unique_ptr<i_frame_buffer> instance_frame_buffer(const e_frame_buffer_type& type, const e_frame_buffer_flags& flags) override { return std::make_unique<c_frame_buffer>(type, flags); }

		std::unique_ptr<backend::shaders::i_passthrough_color> instance_passthrough_color_shader() override { return std::make_unique<shaders::c_passthrough_color>(); }
		std::unique_ptr<backend::shaders::i_passthrough_texture> instance_passthrough_texture_shader() override { return std::make_unique<shaders::c_passthrough_texture>(); }
		std::unique_ptr<backend::shaders::i_quad_gradient> instance_quad_gradient_shader() override { return std::make_unique<shaders::c_quad_gradient>(); }
		std::unique_ptr<backend::shaders::i_linear_gradient> instance_linear_gradient_shader() override { return std::make_unique<shaders::c_linear_gradient>(); }
		std::unique_ptr<backend::shaders::i_sdf> instance_sdf_shader() override { return std::make_unique<shaders::c_sdf>(); }
	};

#ifdef null_renderer_use_glfw
	class c_window : public utils::win::c_window {
	public: using utils::win::c_window::c_window;
		color_t<float> clear_color{ 0.07f, 0.07f, 0.07f };
		GLFWwindow* glfw_window{ };

	public:
		bool create() override {
			if(!glfwInit()) { utils::logger.log(utils::e_log_type::error, "cant init glfw."); return false; }

			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

			if(!(glfw_window = glfwCreateWindow(size.x, size.y, name.c_str(), NULL, NULL))) { utils::logger.log(utils::e_log_type::error, "cant create glfw window."); return false; }
			glfwSetWindowPos(glfw_window, pos.x, pos.y);
			glfwMakeContextCurrent(glfw_window);
			glfwSwapInterval(0);

			/*auto* primaryMonitor = glfwGetPrimaryMonitor();
			float m_frameBufferScaleX, m_frameBufferScaleY;
			glfwGetMonitorContentScale(primaryMonitor, &m_frameBufferScaleX, &m_frameBufferScaleY);*/

			wnd_handle = glfwGetWin32Window(glfw_window);

			factory = std::make_unique<c_factory>();

			render::initialize();
			
			on_create();
			return true;
		}

		void destroy() override {
			on_destroy();
			glfwDestroyWindow(glfw_window);
			glfwTerminate();
		}

		void main_loop() override {
			if(!wnd_handle) { utils::logger.log(utils::e_log_type::error, "window handle is nullptr."); return; };

			while(!glfwWindowShouldClose(glfw_window)) {
				glfwPollEvents();

				vec2_t<int> framebuffer{ };
				glfwGetFramebufferSize(glfw_window, &framebuffer.x, &framebuffer.y);
				glViewport(0, 0, framebuffer.x, framebuffer.y);
				glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
				glClear(GL_COLOR_BUFFER_BIT);
				on_main_loop();

				glfwSwapBuffers(glfw_window);
			}
		}
	};
#endif
}