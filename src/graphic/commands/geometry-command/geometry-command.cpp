#include <backend/renderer/renderer.h>
#include <backend/internal/mesh.h>

#include <graphic/commands/geometry-command/geometry-command.h>

namespace null::render::commands {
	c_geometry::c_geometry()
		: index_offset{ backend::mesh->geometry_buffer.index_buffer.size() }, vertex_offset{ backend::mesh->geometry_buffer.vertex_buffer.size() } { }

	void c_geometry::handle() {
		backend::renderer->draw_geometry(vertex_count, index_count, vertex_offset, index_offset);
	}
}