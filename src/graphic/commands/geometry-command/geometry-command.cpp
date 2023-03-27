#include <renderer/renderer.h>

#include <graphic/commands/geometry-command/geometry-command.h>

namespace null::render::commands {
	c_geometry::c_geometry()
		: index_offset{ renderer->geometry_buffer.index_buffer.size() }, vertex_offset{ renderer->geometry_buffer.vertex_buffer.size() } { }

	void c_geometry::handle() {
		renderer->draw_geometry(vertex_count, index_count, vertex_offset, index_offset);
	}
}