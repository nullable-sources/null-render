#pragma once
#include <backend/internal/geometry-buffer.h>
#include <graphic/draw-list/brushes/brush.h>

namespace null::render {
	enum class e_pen_layer {
		foreground, //@note: drawing over original geometry
		background //@note: drawing under original geometry
	};

	struct pen_t {
	public:
		std::shared_ptr<brush_t> brush{ };
		float thickness{ 1.f };
		float origin{ 1.f }; //@note: offset from geometry start, 0.f -> inside the geometry, 1.f -> outside the geometry
		e_pen_layer layer{ };

	public:
		template <typename brush_type_t> auto&& set_brush(this auto&& self, const brush_type_t& brush) { self.brush = std::make_shared<brush_type_t>(brush); return self; }
		auto&& set_thickness(this auto&& self, float thickness) { self.thickness = thickness; return self; }
		auto&& set_origin(this auto&& self, float origin) { self.origin = origin; return self; }
		auto&& set_layer(this auto&& self, e_pen_layer layer) { self.layer = layer; return self; }
		
	public:
		std::unique_ptr<commands::i_command> around_convex_shape(const std::unique_ptr<commands::c_geometry>& command) const;
		std::unique_ptr<commands::i_command> around_stroke(const std::unique_ptr<commands::c_geometry>& command, const std::vector<backend::index_t>& outward_order, const std::vector<backend::index_t>& inward_order, bool closed) const;

	private:
		void around_order(std::unique_ptr<commands::c_geometry>& pen_command, const std::unique_ptr<commands::c_geometry>& command, const std::vector<backend::index_t>& order, math::e_rotation rotation, backend::index_t first_vertex_offset = { }) const;

		void make_geometry(std::unique_ptr<commands::c_geometry>& pen_command, const std::unique_ptr<commands::c_geometry>& command, backend::index_t previous, backend::index_t current, backend::index_t next, math::e_rotation rotation, bool is_last, backend::index_t first_vertex_offset) const;
	};
}