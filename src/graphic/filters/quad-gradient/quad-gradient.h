#pragma once
#include <graphic/filters/filters.h>

namespace null::render::filters {
	class c_quad_gradient : public i_filter {
	public:
		std::array<color_t<int>, 4> colors{ };

	public:
		c_quad_gradient(std::unique_ptr<commands::i_command>&& _child_command, const std::array<color_t<int>, 4>& _colors)
			: i_filter{ std::move(_child_command) }, colors { _colors } { }

	public:
		virtual void handle() override;
	};

	struct quad_gradient_t : public i_filter_instancer {
	public:
		std::array<color_t<int>, 4> colors{ };

	public:
		template <typename self_t> auto&& set_colors(this self_t&& self, const std::array<color_t<int>, 4>& colors) { self.colors = colors; return self; }
		template <typename self_t> auto&& set_top_left_color(this self_t&& self, const color_t<int>& color) { self.colors[0] = color; return self; }
		template <typename self_t> auto&& set_top_right_color(this self_t&& self, const color_t<int>& color) { self.colors[1] = color; return self; }
		template <typename self_t> auto&& set_bottom_left_color(this self_t&& self, const color_t<int>& color) { self.colors[2] = color; return self; }
		template <typename self_t> auto&& set_bottom_right_color(this self_t&& self, const color_t<int>& color) { self.colors[3] = color; return self; }

	public:
		std::unique_ptr<i_filter> instance_filter(std::unique_ptr<commands::c_geometry>&& child_command) const override {
			child_command->set_default_uvs();
			return std::make_unique<c_quad_gradient>(std::move(child_command), colors);
		}

		quad_gradient_t* clone() const override { return new quad_gradient_t{ *this }; }
	};
}