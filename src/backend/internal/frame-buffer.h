#pragma once
#include <null-sdk.h>

namespace null::render::backend {
	enum class e_frame_buffer_type {
		backbuffer,
		postprocessing
	};

	enum class e_frame_buffer_flags {
		none = 0,
		msaa = 1 << 0
	}; enum_create_bit_operators(e_frame_buffer_flags, true);
	enum_create_cast_operator(e_frame_buffer_flags, -);

	class i_frame_buffer {
	public:
		vec2_t<int> size{ };
		e_frame_buffer_type type{ };
		e_frame_buffer_flags flags{ };

	public:
		i_frame_buffer(const vec2_t<int>& _size, const e_frame_buffer_type& _type, const e_frame_buffer_flags& _flags = { }) : size{ _size }, type { _type }, flags{ _flags } { }
		virtual ~i_frame_buffer() {/* destroy();*/ }

	public:
		virtual void* get_surface() = 0;
		virtual void* get_texture() = 0;

	public:
		virtual void create() = 0;
		virtual void destroy() = 0;

	public:
		virtual void clear() = 0;
		virtual void set() = 0;
		
	public:
		virtual void copy_from(const std::unique_ptr<i_frame_buffer>& another_frame_buffer) = 0;

	public:
		virtual bool empty() const = 0;
	}; inline std::unique_ptr<i_frame_buffer> msaa_buffer{ }, rendering_buffer{ };
	

	inline std::unique_ptr<i_frame_buffer> rtt_buffer{ };
	inline std::array<std::unique_ptr<i_frame_buffer>, 4> postprocessing_buffers{ }; //@note: the result of postprocessing is written to the first buffer
}