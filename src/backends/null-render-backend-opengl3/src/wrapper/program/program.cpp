#include <wrapper/program/program.h>

namespace null::render::backend::opengl3::wrapper {
	void c_program::check(const opengl::e_constants& status, const std::string_view& desc) const {
		std::string log{ "empty" };

		int result_status{ }, log_length{ };
		opengl::get_programiv(program, status, &result_status);
		opengl::get_programiv(program, opengl::e_info_log_length, &log_length);

		if(log_length > 1) {
			log = std::string(log_length + 1, char{ });
			opengl::get_program_info_log(program, log_length, nullptr, log.data());
		}

		if(!result_status) utils::logger.log(utils::e_log_type::warning, "cant '{}' program, log \"{}\".", desc, log);
	}
}