#pragma once
#include <null-renderer-opengl3.h>

namespace null::render::shaders {
	class i_shader {
	public:
		class i_shader_wrapper {
		public:
			std::uint32_t shader{ };

		public:
			virtual void create() = 0;
			virtual void compile(const void* source) {
				opengl::shader_source(shader, 1, (const char* const*)&source, NULL);
				opengl::compile_shader(shader);
				check(opengl::e_compile_status, "compile");
			}

			virtual void destroy() { if(shader) { opengl::delete_shader(shader); check(opengl::e_delete_status, "destroy"); shader = 0; } }

		public:
			void check(const opengl::e_constants& status, const std::string_view& desc) {
				std::string log{ "empty" };
				
				int result_status{ }, log_length{ };
				opengl::get_shaderiv(shader, status, &result_status);
				opengl::get_shaderiv(shader, opengl::e_info_log_length, &log_length);
				
				if(log_length > 1) {
					log = std::string(log_length + 1, char{ });
					opengl::get_shader_info_log(shader, log_length, nullptr, log.data());
				}

				if(!result_status) throw std::runtime_error{ std::format("cant \"{}\" shader, log \"{}\"", desc, log) };
			}

		public:
			virtual operator bool() const { return shader != 0; }
		};

		class c_vertex_shader_wrapper : public i_shader_wrapper {
		public:
			virtual void create() override { shader = opengl::create_shader(opengl::e_vertex_shader); }
		};

		class c_fragment_shader_wrapper : public i_shader_wrapper {
		public:
			virtual void create() override { shader = opengl::create_shader(opengl::e_fragment_shader); }
		};

		class c_program {
		public:
			std::uint32_t program{ };

		public:
			virtual void create() { program = opengl::create_program(); }
			virtual void destroy() { opengl::delete_program(program); check(opengl::e_delete_status, "destroy"); }

			virtual void attach_shader(i_shader_wrapper* shader) { opengl::attach_shader(program, shader->shader); }
			virtual void detach_shader(i_shader_wrapper* shader) { opengl::detach_shader(program, shader->shader); }

			virtual void use() { opengl::use_program(program); }
			virtual void link() { opengl::link_program(program); check(opengl::e_link_status, "link"); }

		public:
			void check(const opengl::e_constants& status, const std::string_view& desc) {
				std::string log{ "empty" };

				int result_status{ }, log_length{ };
				opengl::get_programiv(program, status, &result_status);
				opengl::get_programiv(program, opengl::e_info_log_length, &log_length);

				if(log_length > 1) {
					log = std::string(log_length + 1, char{ });
					opengl::get_program_info_log(program, log_length, nullptr, log.data());
				}

				if(!result_status) throw std::runtime_error{ std::format("cant \"{}\" program, log \"{}\"", desc, log) };
			}

		public:
			virtual operator bool() const { return program != 0; }
		};

		class i_shader_variable {
		public:
			int location{ };

		public:
			virtual void get_location(c_program* program, const std::string_view& name) = 0;
		};

		class c_attribute : public i_shader_variable {
		public:
			virtual void get_location(c_program* program, const std::string_view& name) override {
				if(int result{ opengl::get_attrib_location(program->program, name.data()) }; result != -1) location = result;
				else throw std::runtime_error{ std::format("cant get {} attribute location", name) };
			}
		};

		class c_uniform : public i_shader_variable {
		public:
			virtual void get_location(c_program* program, const std::string_view& name) override {
				if(int result{ opengl::get_uniform_location(program->program, name.data()) }; result != -1) location = result;
				else throw std::runtime_error{ std::format("cant get {} uniforn location", name) };
			}
		};

		template <typename vertex_t, size_t size, std::uint32_t type, bool normalized, std::uint32_t offset>
		class c_vertex_element : public c_attribute {
		public:
			void set() {
				opengl::enable_vertex_attrib_array(location);
				opengl::vertex_attrib_pointer(location, size, type, normalized, sizeof(vertex_t), (void*)offset);
			}
		};

		class c_matrix4x4 : public c_uniform {
		public:
			matrix4x4_t saved_matrix{ };

		public:
			void setup(const matrix4x4_t& matrix) { saved_matrix = matrix; }
			void set(const matrix4x4_t& matrix) { opengl::uniform_matrix4fv(location, 1, false, matrix.linear_array.data()); }
			void set() { opengl::uniform_matrix4fv(location, 1, false, saved_matrix.linear_array.data()); }
		};

	public:
		std::unique_ptr<c_program> program{ };
		std::unique_ptr<c_fragment_shader_wrapper> fragment{ };
		std::unique_ptr<c_vertex_shader_wrapper> vertex{ };

	public:
		virtual void create() = 0;
		virtual void destroy() = 0;

		virtual void begin_frame() { }
		virtual void end_frame() { }

		virtual void setup_state() { }

		virtual void use() = 0;
	};
}