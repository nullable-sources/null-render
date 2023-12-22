#pragma once
#include "../../backend/internal/geometry-buffer.h"
#include "../brushes/brush.h"

namespace null::render {
	enum class e_pen_layer {
		foreground, //@note: drawing over original geometry
		background //@note: drawing under original geometry
	};

	enum class e_pen_edge {
		miter, //@note: the distance from the edge will be different, analogous to e_line_join::miter
		thickness //@note: the distance from the edge will always be equal to the thickness
	};

	struct pen_t {
	public:
		std::shared_ptr<c_brush> brush{ };
		float thickness{ 1.f };
		float origin{ 1.f }; //@note: offset from geometry start, 0.f -> inside the geometry, 1.f -> outside the geometry
		e_pen_layer layer{ };
		e_pen_edge edge{ };

	public:
		template <typename self_t> auto&& set_brush(this self_t&& self, const std::shared_ptr<c_brush>& brush) { self.brush = brush; return self; }
		template <typename self_t> auto&& set_thickness(this self_t&& self, float thickness) { self.thickness = thickness; return self; }
		template <typename self_t> auto&& set_origin(this self_t&& self, float origin) { self.origin = origin; return self; }
		template <typename self_t> auto&& set_layer(this self_t&& self, e_pen_layer layer) { self.layer = layer; return self; }
		template <typename self_t> auto&& set_edge(this self_t&& self, e_pen_edge edge) { self.edge = edge; return self; }

	public:
		std::shared_ptr<i_command> around_convex_shape(const std::shared_ptr<c_geometry_command>& command) const;
		std::shared_ptr<i_command> around_stroke(const std::shared_ptr<c_geometry_command>& command, const std::vector<backend::index_t>& outward_order, const std::vector<backend::index_t>& inward_order, bool closed) const;

	private:
		void around_order(std::shared_ptr<c_geometry_command>& pen_command, const std::shared_ptr<c_geometry_command>& command, const std::vector<backend::index_t>& order, math::e_rotation rotation, backend::index_t first_vertex_offset = { }) const;

		void make_geometry(std::shared_ptr<c_geometry_command>& pen_command, const std::shared_ptr<c_geometry_command>& command, backend::index_t previous, backend::index_t current, backend::index_t next, math::e_rotation rotation, bool is_last, backend::index_t first_vertex_offset) const;
	};
}