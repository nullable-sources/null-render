#pragma once
#ifdef null_renderer_use_glfw
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#endif

#include <opengl/opengl3.h>

#include <null-render.h>

namespace null::render {
	class c_opengl3 : public i_renderer {
	public:
		struct saved_state_t {
		public:
			std::uint32_t active_texture{ }, program{ }, texture{ }, sampler{ }, array_buffer{ }, vertex_array_object{ };
			std::uint32_t blend_src_rgb{ }, blend_dst_rgb{ }, blend_src_alpha{ }, blend_dst_alpha{ }, blend_equation_rgb{ }, blend_equation_alpha{ };
			bool enable_blend{ }, enable_cull_face{ }, enable_depth_test{ }, enable_stencil_test{ }, enable_scissor_test{ }, enable_primitive_restart{ };
			int viewport[4]{ }, scissor_box[4]{ };

		public:
			void save() {
				opengl::get_integerv(opengl::e_active_texture, (int*)&active_texture);
				opengl::active_texture(opengl::e_texture0);
				opengl::get_integerv(opengl::e_current_program, (int*)&program);
				opengl::get_integerv(opengl::e_texture_binding_2d, (int*)&texture);
				opengl::get_integerv(opengl::e_sampler_binding, (int*)&sampler);
				opengl::get_integerv(opengl::e_array_buffer_binding, (int*)&array_buffer);
				opengl::get_integerv(opengl::e_vertex_array_binding, (int*)&vertex_array_object);

				opengl::get_integerv(opengl::e_viewport, viewport);
				opengl::get_integerv(opengl::e_scissor_box, scissor_box);
				opengl::get_integerv(opengl::e_blend_src_rgb, (int*)&blend_src_rgb);
				opengl::get_integerv(opengl::e_blend_dst_rgb, (int*)&blend_dst_rgb);
				opengl::get_integerv(opengl::e_blend_src_alpha, (int*)&blend_src_alpha);
				opengl::get_integerv(opengl::e_blend_dst_alpha, (int*)&blend_dst_alpha);
				opengl::get_integerv(opengl::e_blend_equation_rgb, (int*)&blend_equation_rgb);
				opengl::get_integerv(opengl::e_blend_equation_alpha, (int*)&blend_equation_alpha);
				enable_blend = opengl::is_enabled(opengl::e_blend);
				enable_cull_face = opengl::is_enabled(opengl::e_cull_face);
				enable_depth_test = opengl::is_enabled(opengl::e_depth_test);
				enable_stencil_test = opengl::is_enabled(opengl::e_stencil_test);
				enable_scissor_test = opengl::is_enabled(opengl::e_scissor_test);
				enable_primitive_restart = opengl::is_enabled(opengl::e_primitive_restart);
			}

			void restore() {
				opengl::use_program(program);
				opengl::bind_texture(opengl::e_texture_2d, texture);
				opengl::bind_sampler(0, sampler);
				opengl::active_texture(active_texture);
				opengl::bind_vertex_array(vertex_array_object);
				opengl::bind_buffer(opengl::e_array_buffer, array_buffer);
				opengl::blend_equation_separate(blend_equation_rgb, blend_equation_alpha);
				opengl::blend_func_separate(blend_src_rgb, blend_dst_rgb, blend_src_alpha, blend_dst_alpha);
				if(enable_blend) opengl::enable(opengl::e_blend); else opengl::disable(opengl::e_blend);
				if(enable_cull_face) opengl::enable(opengl::e_cull_face); else opengl::disable(opengl::e_cull_face);
				if(enable_depth_test) opengl::enable(opengl::e_depth_test); else opengl::disable(opengl::e_depth_test);
				if(enable_stencil_test) opengl::enable(opengl::e_stencil_test); else opengl::disable(opengl::e_stencil_test);
				if(enable_scissor_test) opengl::enable(opengl::e_scissor_test); else opengl::disable(opengl::e_scissor_test);
				if(enable_primitive_restart) opengl::enable(opengl::e_primitive_restart); else opengl::disable(opengl::e_primitive_restart);
				opengl::viewport(viewport[0], viewport[1], viewport[2], viewport[3]);
				opengl::scissor(scissor_box[0], scissor_box[1], scissor_box[2], scissor_box[3]);
			}
		} saved_state{ };

	public:
		std::uint32_t vbo_handle{ }, elements_handle{ };

		std::uint32_t vertex_array_object{ };

	public:
		void set_texture(void* texture) override;
		void set_clip(const rect_t<float>& rect) override;
		void draw_geometry(const size_t& vertex_count, const size_t& index_count, const size_t& vertex_offset, const size_t& index_offset) override;

	public:
		void initialize() override;
		void shutdown() override { }

		void begin_frame() override { create_objects(); }
		void end_frame() override { }

		void begin_render() override;
		void end_render() override;

		void setup_state() override;

		void create_objects() override;
		void destroy_objects() override;

		void* create_texture(const vec2_t<float>& size, void* data) override;
		void destroy_texture(void* texture) override;
	}; inline std::unique_ptr<c_opengl3> opengl3{ };

#ifdef null_renderer_use_glfw
	class c_window : public ::utils::win::c_window {
	public: using ::utils::win::c_window::c_window;
		color_t<float> clear_color{ 0.07f, 0.07f, 0.07f };
		GLFWwindow* glfw_window{ };

		//std::vector<commands::c_command_buffer*> user_buffers{ };

	public:
		bool create() override {
			if(!glfwInit()) throw std::runtime_error{ "cant init glfw" };

			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

			if(!(glfw_window = glfwCreateWindow(size.x, size.y, name.c_str(), NULL, NULL))) throw std::runtime_error{ "cant create glfw window" };
			glfwSetWindowPos(glfw_window, pos.x, pos.y);
			glfwMakeContextCurrent(glfw_window);
			glfwSwapInterval(0);

			/*auto* primaryMonitor = glfwGetPrimaryMonitor();
			float m_frameBufferScaleX, m_frameBufferScaleY;
			glfwGetMonitorContentScale(primaryMonitor, &m_frameBufferScaleX, &m_frameBufferScaleY);*/

			wnd_handle = glfwGetWin32Window(glfw_window);

			opengl3 = std::make_unique<c_opengl3>();
			renderer = opengl3.get();

			on_create();
			renderer->initialize();
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

				vec2_t<int> framebuffer{ };
				glfwGetFramebufferSize(glfw_window, &framebuffer.x, &framebuffer.y);
				glViewport(0, 0, framebuffer.x, framebuffer.y);
				glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
				glClear(GL_COLOR_BUFFER_BIT);
				renderer->begin_render();
				//for(auto& user_buffer : user_buffers) user_buffer->handle();
				renderer->end_render();

				glfwSwapBuffers(glfw_window);
			}
		}
	};
#endif
}