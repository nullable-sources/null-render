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
		const brush_t* brush{ };
		float thickness{ 5.f };
		float origin{ 0.5f }; //@note: offset from geometry start, 0.f -> inside the geometry, 1.f -> outside the geometry
		e_pen_layer layer{ };

	public:
		template <typename self_t> auto&& set_brush(this self_t&& self, const brush_t& brush) { self.brush = &brush; return self; }
		template <typename self_t> auto&& set_thickness(this self_t&& self, const float& thickness) { self.thickness = thickness; return self; }
		template <typename self_t> auto&& set_origin(this self_t&& self, const float& origin) { self.origin = origin; return self; }
		template <typename self_t> auto&& set_layer(this self_t&& self, const e_pen_layer& layer) { self.layer = layer; return self; }
		
	public:
		std::unique_ptr<commands::i_command> around_convex_shape(const std::unique_ptr<commands::c_geometry>& command) const;
		std::unique_ptr<commands::i_command> around_stroke(const std::unique_ptr<commands::c_geometry>& command, const std::vector<backend::index_t>& outward_order, const std::vector<backend::index_t>& inward_order, const bool& closed) const;

	private:
		void around_order(std::unique_ptr<commands::c_geometry>& pen_command, const std::unique_ptr<commands::c_geometry>& command, const std::vector<backend::index_t>& order, const math::e_rotation& rotation, backend::index_t first_vertex_offset = { }) const;

		void make_geometry(std::unique_ptr<commands::c_geometry>& pen_command, const std::unique_ptr<commands::c_geometry>& command, const backend::index_t& previous, const backend::index_t& current, const backend::index_t& next, const math::e_rotation& rotation, const bool& is_last, const backend::index_t& first_vertex_offset) const;
	};
}