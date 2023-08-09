#pragma once
#include "graphic/commands/geometry-command/geometry-command.h"

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
			struct edge_t {
			public:
				const vec2_t<float>* point{ };
				size_t outward_begin{ }, outward_end{ };
				size_t inward_begin{ }, inward_end{ };
				size_t join_size{ };

				vec2_t<float> normal{ }, from_previous_direction{ }, to_next_direction{ };
				angle_t<radians_t> miter_angle{ };
				double half_miter_cos{ };
				float max_miter_dist{ };
				bool inversed{ };
			};

		public:
			std::shared_ptr<edge_t> begin_edge{ }, end_edge{ };
			bool is_first{ }, is_last{ };
		};

	public:
		float thickness{ 1.f };
		e_line_join line_join{ };
		e_line_cap line_cap{ };

	public:
		auto&& set_thickness(this auto&& self, float thickness) { self.thickness = std::max(thickness, 1.f); return self; }
		auto&& set_join(this auto&& self, e_line_join line_join) { self.line_join = line_join; return self; }
		auto&& set_cap(this auto&& self, e_line_cap line_cap) { self.line_cap = line_cap; return self; }

	public:
		std::vector<segment_t> build_segments(const std::vector<vec2_t<float>>& points) const;
	};
}