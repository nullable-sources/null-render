module;
#include <array>
#include <memory>
export module null.render:graphic.filters.quad_gradient;

import null.sdk;

import :graphic.command;
import :graphic.commands.geometry;
import :graphic.filter;
import :backend.shaders.quad_gradient;

export namespace null::render {
	class c_quad_gradient_filter : public i_filter {
	public:
		std::array<color_t<int>, 4> colors{ };

	public:
		c_quad_gradient_filter(std::unique_ptr<i_command>&& _child_command, const std::array<color_t<int>, 4>& _colors)
			: i_filter{ std::move(_child_command) }, colors{ _colors } { }

	public:
		virtual void handle() override {
			backend::shaders::quad_gradient->set_colors(colors);
			backend::shaders::quad_gradient->use();

			child_command->handle();

			backend::shaders::passthrough_color->use();
		}
	};

	struct quad_gradient_filter_t : public i_filter_instancer {
	public:
		std::array<color_t<int>, 4> colors{ };

	public: //@todo: P0847
		auto& set_colors(const std::array<color_t<int>, 4>& _colors) { colors = colors; return *this; }
		auto& set_top_left_color(const color_t<int>& color) { colors[0] = color; return *this; }
		auto& set_top_right_color(const color_t<int>& color) { colors[1] = color; return *this; }
		auto& set_bottom_left_color(const color_t<int>& color) { colors[2] = color; return *this; }
		auto& set_bottom_right_color(const color_t<int>& color) { colors[3] = color; return *this; }

	public:
		std::unique_ptr<i_filter> instance_filter(std::unique_ptr<c_geometry_command>&& child_command) const override {
			child_command->set_default_uvs();
			return std::make_unique<c_quad_gradient_filter>(std::move(child_command), colors);
		}
	};
}