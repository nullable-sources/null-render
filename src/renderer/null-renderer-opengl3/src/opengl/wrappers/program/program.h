#pragma once
#include <opengl/wrappers/shader/shader.h>

namespace null::renderer::wrapper {
	class c_program {
	public:
		std::uint32_t program{ };

	public:
		virtual void create() { program = opengl::create_program(); }
		virtual void destroy() { opengl::delete_program(program); check(opengl::e_delete_status, "destroy"); }

		virtual void attach_shader(i_shader* shader) { opengl::attach_shader(program, shader->shader); }
		virtual void detach_shader(i_shader* shader) { opengl::detach_shader(program, shader->shader); }

		virtual void use() { opengl::use_program(program); }
		virtual void link() { opengl::link_program(program); check(opengl::e_link_status, "link"); }

		virtual bool empty() const { return program == 0; }

	public:
		void check(const opengl::e_constants& status, const std::string_view& desc) const;

	public:
		virtual operator bool() const { return !empty(); }
	};
}