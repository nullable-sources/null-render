#pragma once
#include "../../../backend/shaders/quad-gradient.h"
#include "../filter.h"

namespace null::render {
	class c_quad_gradient_filter : public i_filter {
	public:
		static std::shared_ptr<c_quad_gradient_filter> instance() { return std::make_shared<c_quad_gradient_filter>(); }
		static std::shared_ptr<c_quad_gradient_filter> instance(const std::array<color_t<int>, 4>& colors) { return std::make_shared<c_quad_gradient_filter>(colors); }

	public:
		backend::i_quad_gradient_shader::constants_t constants{ };

	public:
		c_quad_gradient_filter() { }
		c_quad_gradient_filter(const std::array<color_t<int>, 4>& colors) : constants(colors) { }
		virtual ~c_quad_gradient_filter() {
			int i = 0;
		}

	public:
		void set_colors(const std::array<color_t<int>, 4>& colors) { constants.colors = colors; }
		void set_top_left_color(const color_t<int>& color) { constants.colors[0] = color; }
		void set_top_right_color(const color_t<int>& color) { constants.colors[1] = color; }
		void set_bottom_left_color(const color_t<int>& color) { constants.colors[2] = color; }
		void set_bottom_right_color(const color_t<int>& color) { constants.colors[3] = color; }

	public:
		virtual void set_child_command(std::shared_ptr<c_geometry_command>& _child_command) override {
			_child_command->set_default_uvs();
			i_filter::set_child_command(_child_command);
		}

		virtual void handle() override;

	public:
		virtual std::shared_ptr<i_filter> clone() const override {
			return std::shared_ptr<c_quad_gradient_filter>(new c_quad_gradient_filter(*this));
		}
	};
}