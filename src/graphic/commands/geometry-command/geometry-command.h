#pragma once
#include <graphic/commands/commands.h>
#include <graphic/filters/filters.h>

namespace null::render::commands {
	class c_geometry : public i_command {
	public:
		size_t index_offset{ }, vertex_offset{ };
		size_t index_count{ }, vertex_count{ };

	public:
		c_geometry();
		virtual ~c_geometry() { }

	public:
		virtual void handle() override;

	public:
		void get_bounding_box(vec2_t<float>& min, vec2_t<float>& max) const;
		void recalculate_uvs(const vec2_t<float>& min, const vec2_t<float>& max) const;
	};
}