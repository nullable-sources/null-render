#pragma once
#include <opengl/wrappers/program/program.h>

namespace null::renderer::wrapper {
	class i_field {
	public:
		int location{ };

	public:
		virtual void get_location(c_program* program, const std::string_view& name) = 0;
	};

	class c_attribute : public i_field {
	public:
		virtual void get_location(c_program* program, const std::string_view& name) override {
			if(int result{ opengl::get_attrib_location(program->program, name.data()) }; result != -1) location = result;
			else ::utils::logger.log(::utils::e_log_type::warning, "cant get '{}' attribute location.", name);
		}
	};

	template <typename type_t>
	class c_uniform : public i_field {
	public:
		type_t stored_value{ };

	private:
		template <typename value_t>
		static void set_impl(const int& _location, const value_t& value) { utils::logger.log(utils::e_log_type::error, "c_uniform::set is not implemented for this type."); }
		
		static void set_impl(const int& _location, const matrix4x4_t& value) { opengl::uniform_matrix4fv(_location, 1, false, value.linear_array.data()); }
		static void set_impl(const int& _location, const vec4_t<float>& value) { opengl::uniform4f(_location, value.x, value.y, value.z, value.w); }
		static void set_impl(const int& _location, const vec4_t<int>& value) { opengl::uniform4i(_location, value.x, value.y, value.z, value.w); }
		static void set_impl(const int& _location, const float& value) { opengl::uniform1f(_location, value); }

	public:
		template <typename self_t> auto&& value(this self_t&& self) { return self.stored_value; }
		void set(const type_t& value) { set_impl(location, value); }
		void set() { set(stored_value); }

	public:
		virtual void get_location(c_program* program, const std::string_view& name) override {
			if(int result{ opengl::get_uniform_location(program->program, name.data()) }; result != -1) location = result;
			else ::utils::logger.log(::utils::e_log_type::warning, "cant get '{}' uniforn location.", name);
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

	/*class c_matrix4x4 : public c_uniform {
	public:
		matrix4x4_t saved_matrix{ };

	public:
		template <typename self_t> auto&& value(this self_t&& self) { return self.saved_matrix; }
		void set(const matrix4x4_t& matrix) { opengl::uniform_matrix4fv(location, 1, false, matrix.linear_array.data()); }
		void set() { set(saved_matrix); }
	};

	class c_vec4 : public c_uniform {
	public:
		vec4_t<float> saved_vec{ };

	public:
		template <typename self_t> auto&& value(this self_t&& self) { return self.saved_vec; }
		void set(const vec4_t<float>& vec) { opengl::uniform4f(location, vec.x, vec.y, vec.z, vec.w); }
		void set() { set(saved_vec); }
	};

	class c_float : public c_uniform {
	public:
		float saved_float{ };

	public:
		template <typename self_t> auto&& value(this self_t&& self) { return self.saved_float; }
		void set(const float& _float) { opengl::uniform1f(location, _float); }
		void set() { set(saved_float); }
	};*/
}