#pragma once
#include "graphic/commands/commands.h"

namespace null::render {
	class c_geometry_command : public i_command {
	public:
		size_t index_offset{ }, vertex_offset{ };
		size_t index_count{ }, vertex_count{ };

	public:
		c_geometry_command();
		virtual ~c_geometry_command() { }

	public:
		virtual void handle() override;

	public:
		void get_bounding_box(vec2_t<float>& min, vec2_t<float>& max) const;
		void recalculate_uvs(const vec2_t<float>& min, const vec2_t<float>& max) const;
		void recalculate_uvs(const vec2_t<float>& min, const vec2_t<float>& max, const rect_t<float>& uvs) const;

		void set_uvs(const rect_t<float>& uvs) const;
		void set_default_uvs() const;
	};
}