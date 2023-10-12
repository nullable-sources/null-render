#pragma once
#include "../../commands/geometry-command/geometry-command.h"

namespace null::render {
	enum class e_line_join {
		miter,
		bevel,
		none
	};

	enum class e_line_cap {
		butt,
		square,
		joint
	};

	struct stroke_t {
	public:
		struct segment_t {
		public:
			struct i_edge {
			public:
				vec2_t<float> normal{ }, from_previous_direction{ }, to_next_direction{ };
				angle_t<radians_t> miter_angle{ };
				float max_miter_dist{ };

			public:
				void from_points(const vec2_t<float>& previous_point, const vec2_t<float>& current_point, const vec2_t<float>& next_point, math::e_rotation normal_rotation = math::e_rotation::ccw);
				void miter_from_points(const vec2_t<float>& previous_point, const vec2_t<float>& current_point, const vec2_t<float>& next_point);

			public:
				vec2_t<float> get_mitter_offset(float distance) {
					return normal * std::min(distance / miter_angle.cos(), static_cast<double>(max_miter_dist));
				}
			};

			struct edge_t : public i_edge {
			public:
				const vec2_t<float>* point{ };
				size_t outward_begin{ }, outward_end{ };
				size_t inward_begin{ }, inward_end{ };
				size_t join_size{ };

				bool inversed{ };
			};

		public:
			std::shared_ptr<edge_t> begin_edge{ }, end_edge{ };
			bool is_first{ }, is_last{ };
		};

	public:
		float thickness{ 1.f };
		float origin{ 0.5f };
		e_line_join line_join{ };
		e_line_cap line_cap{ };

	public:
		auto&& set_thickness(this auto&& self, float thickness) { self.thickness = std::max(thickness, 1.f); return self; }
		auto&& set_origin(this auto&& self, float origin) { self.origin = std::clamp(origin, 0.f, 1.f); return self; }
		auto&& set_join(this auto&& self, e_line_join line_join) { self.line_join = line_join; return self; }
		auto&& set_cap(this auto&& self, e_line_cap line_cap) { self.line_cap = line_cap; return self; }

	public:
		void build_segments(std::vector<segment_t>& segments, const std::vector<vec2_t<float>>& points) const;
	};
}
