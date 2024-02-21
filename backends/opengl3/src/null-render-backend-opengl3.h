#pragma once
#ifdef null_renderer_use_glfw
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#endif

#include "null-render-backend-opengl3/state-pipeline/state-pipeline.h"
#include "null-render-backend-opengl3/renderer/renderer.h"
#include "null-render-backend-opengl3/internal/frame-buffer/frame-buffer.h"
#include "null-render-backend-opengl3/internal/stencil-buffer/stencil-buffer.h"
#include "null-render-backend-opengl3/internal/mesh/mesh.h"
#include "null-render-backend-opengl3/shaders/passthrough-color/passthrough-color.h"
#include "null-render-backend-opengl3/shaders/passthrough-texture/passthrough-texture.h"
#include "null-render-backend-opengl3/shaders/blur/blur.h"
#include "null-render-backend-opengl3/shaders/quad-gradient/quad-gradient.h"
#include "null-render-backend-opengl3/shaders/linear-gradient/linear-gradient.h"
#include "null-render-backend-opengl3/shaders/radial-gradient/radial-gradient.h"
#include "null-render-backend-opengl3/shaders/sdf/sdf.h"

namespace null::render::opengl3 {
	class c_factory : public backend::i_factory {
	public:
		std::unique_ptr<backend::i_renderer> instance_renderer() override { return std::make_unique<c_renderer>(); }
		std::unique_ptr<backend::c_mesh> instance_mesh() override { return std::make_unique<c_mesh>(); }
		std::unique_ptr<backend::i_frame_buffer> instance_frame_buffer(const vec2_t<int>& size, backend::e_frame_buffer_type type, backend::e_frame_buffer_flags flags) override { return std::make_unique<c_frame_buffer>(size, type, flags); }
		std::unique_ptr<backend::i_stencil_buffer> instance_stencil_buffer() override { return std::make_unique<c_stencil_buffer>(); }
		std::unique_ptr<backend::i_state_pipeline> instance_state_pipeline() override { return std::make_unique<c_state_pipeline>(); }

		std::unique_ptr<backend::i_passthrough_color_shader> instance_passthrough_color_shader() override { return std::make_unique<c_passthrough_color_shader>(); }
		std::unique_ptr<backend::i_passthrough_texture_shader> instance_passthrough_texture_shader() override { return std::make_unique<c_passthrough_texture_shader>(); }
		std::unique_ptr<backend::i_blur_shader> instance_blur_shader() override { return std::make_unique<c_blur_shader>(); }
		std::unique_ptr<backend::i_quad_gradient_shader> instance_quad_gradient_shader() override { return std::make_unique<c_quad_gradient_shader>(); }
		std::unique_ptr<backend::i_linear_gradient_shader> instance_linear_gradient_shader() override { return std::make_unique<c_linear_gradient_shader>(); }
		std::unique_ptr<backend::i_radial_gradient_shader> instance_radial_gradient_shader() override { return std::make_unique<c_radial_gradient_shader>(); }
		std::unique_ptr<backend::i_sdf_shader> instance_sdf_shader() override { return std::make_unique<c_sdf_shader>(); }
	};

#ifdef null_renderer_use_glfw
	class c_window : public utils::win::c_window {
	public: using utils::win::c_window::c_window;
		color_t<float> clear_color{ 0.07f, 0.07f, 0.07f };
		GLFWwindow* glfw_window{ };

	public:
		static void resize(GLFWwindow* window, int width, int height) {
			backend::renderer->begin_resize_viewport(vec2_t<float>(width, height));
			backend::renderer->end_resize_viewport();
		}

	public:
		bool create() override {
			if(!glfwInit()) { utils::logger(utils::e_log_type::error, "cant init glfw."); return false; }

			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

			if(!(glfw_window = glfwCreateWindow(size.x, size.y, name.c_str(), NULL, NULL))) { utils::logger(utils::e_log_type::error, "cant create glfw window."); return false; }
			glfwSetWindowPos(glfw_window, pos.x, pos.y);
			glfwMakeContextCurrent(glfw_window);
			glfwSwapInterval(1);

			wnd_handle = glfwGetWin32Window(glfw_window);

			backend::factory = std::make_unique<c_factory>();

			render::initialize(size);

			glfwSetFramebufferSizeCallback(glfw_window, resize);

			on_create();
			return true;
		}

		void destroy() override {
			on_destroy();
			glfwDestroyWindow(glfw_window);
			glfwTerminate();
		}

		void main_loop() override {
			if(!wnd_handle) { utils::logger(utils::e_log_type::error, "window handle is nullptr."); return; };

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