#pragma once
#include <null-sdk.h>
#include <opengl/opengl3.h>

namespace null::renderer::wrapper {
	class i_shader {
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

		virtual bool empty() const { return shader == 0; }

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

			if(!result_status) utils::logger.log(utils::e_log_type::warning, "cant '{}' shader, log \"{}\".", desc, log);
		}

	public:
		virtual operator bool() const { return !empty(); }
	};

	class c_vertex_shader : public i_shader {
	public:
		virtual void create() override { shader = opengl::create_shader(opengl::e_vertex_shader); }
	};

	class c_fragment_shader : public i_shader {
	public:
		virtual void create() override { shader = opengl::create_shader(opengl::e_fragment_shader); }
	};
}