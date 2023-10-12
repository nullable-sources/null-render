#pragma once
#include "null-render/backend/shaders/quad-gradient-shader.h"
#include "../filters.h"

namespace null::render {
	class c_quad_gradient_filter : public i_filter {
	public:
		backend::i_quad_gradient_shader::constants_t constants{ };

	public:
		c_quad_gradient_filter(std::unique_ptr<i_command>&& _child_command, const backend::i_quad_gradient_shader::constants_t& _constants)
			: i_filter{ std::move(_child_command) }, constants{ _constants } { }

	public:
		virtual void handle() override;
	};

	struct quad_gradient_filter_t : public i_filter_instancer {
	public:
		backend::i_quad_gradient_shader::constants_t constants{ };

	public:
		auto&& set_colors(this auto&& self, const std::array<color_t<int>, 4>& colors) { self.constants.colors = colors; return self; }
		auto&& set_top_left_color(this auto&& self, const color_t<int>& color) { self.constants.colors[0] = color; return self; }
		auto&& set_top_right_color(this auto&& self, const color_t<int>& color) { self.constants.colors[1] = color; return self; }
		auto&& set_bottom_left_color(this auto&& self, const color_t<int>& color) { self.constants.colors[2] = color; return self; }
		auto&& set_bottom_right_color(this auto&& self, const color_t<int>& color) { self.constants.colors[3] = color; return self; }

	public:
		std::unique_ptr<i_filter> instance_filter(std::unique_ptr<c_geometry_command>&& child_command) const override {
			child_command->set_default_uvs();
			return std::make_unique<c_quad_gradient_filter>(std::move(child_command), constants);
		}
	};
}