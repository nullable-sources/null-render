#pragma once
#include <backend/internal/geometry-buffer.h>

namespace null::render::backend {
	class i_mesh {
	public:
		c_geometry_buffer geometry_buffer{ };

	public:
		virtual void create() = 0;
		virtual void destroy() = 0;

		virtual void compile() = 0;

		virtual void set() = 0;

	public:
		void clear_geometry() { geometry_buffer.clear(); }
	}; inline std::unique_ptr<i_mesh> mesh{ };
}