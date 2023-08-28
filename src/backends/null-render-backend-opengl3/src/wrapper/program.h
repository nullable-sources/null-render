#pragma once
#include "wrapper/shader.h"

namespace null::render::opengl3 {
	class c_program {
	public:
		std::uint32_t program{ };

	public:
		virtual void create() { program = opengl::create_program(); }
		virtual void destroy() { opengl::delete_program(program); check(opengl::e_delete_status, "destroy"); }

		virtual void attach_shader(i_compiled_shader* shader) { opengl::attach_shader(program, shader->shader); }
		virtual void detach_shader(i_compiled_shader* shader) { opengl::detach_shader(program, shader->shader); }

		virtual void use() { opengl::use_program(program); }
		virtual void link() { opengl::link_program(program); check(opengl::e_link_status, "link"); }

		virtual bool empty() const { return program == 0; }

	public:
		void check(opengl::e_constants status, std::string_view desc) const {
			std::string log{ "empty" };

			int result_status{ }, log_length{ };
			opengl::get_programiv(program, status, &result_status);
			opengl::get_programiv(program, opengl::e_info_log_length, &log_length);

			if(log_length > 1) {
				log = std::string(log_length + 1, char{ });
				opengl::get_program_info_log(program, log_length, nullptr, log.data());
			}

			if(!result_status) utils::logger(utils::e_log_type::warning, "cant '{}' program, log \"{}\".", desc, log);
		}

	public:
		virtual operator bool() const { return !empty(); }
	};
}