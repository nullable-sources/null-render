#pragma once
#include <backend/internal/geometry-buffer.h>

namespace null::render::backend {
	class i_mesh {
	public:
		virtual void create() = 0;
		virtual void destroy() = 0;

		virtual void compile() = 0;

		virtual void set() = 0;

	public:
		virtual void clear_geometry() = 0;
	};;

	class c_mesh : public i_mesh {
	public:
		c_geometry_buffer geometry_buffer{ };

	public:
		void clear_geometry() { geometry_buffer.clear(); }
	}; inline std::unique_ptr<c_mesh> mesh{ };
}