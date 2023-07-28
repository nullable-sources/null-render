module;
#include <vector>
#include <memory>
export module null.render:graphic.filters.linear_gradient;

import null.sdk;

import :graphic.filter;
import :graphic.command;
import :graphic.commands.geometry;
import :backend.shaders.linear_gradient;

export namespace null::render {
	class c_linear_gradient_filter : public i_filter {
	public:
		radians_t angle{ };
		std::vector<std::pair<color_t<int>, float>> stops{ };

	public:
		c_linear_gradient_filter(std::unique_ptr<i_command> _child_command, radians_t _angle, const std::vector<std::pair<color_t<int>, float>>& _stops)
			: i_filter{ std::move(_child_command) }, angle{ _angle }, stops{ _stops } { }

	public:
		virtual void handle() override {
			backend::shaders::linear_gradient->set_angle(angle);
			backend::shaders::linear_gradient->set_stops(stops);
			backend::shaders::linear_gradient->use();

			child_command->handle();

			backend::shaders::passthrough_color->use();
		}
	};

	struct linear_gradient_filter_t : public i_filter_instancer {
	public:
		radians_t angle{ };
		std::vector<std::pair<color_t<int>, float>> stops{ };

	public: //@todo: P0847
		auto& set_angle(radians_t _angle) { angle = _angle; return *this; }
		auto& set_stops(const std::vector<std::pair<color_t<int>, float>>& _stops) { stops = _stops; return *this; }
		auto& add_stop(const color_t<int>& color, float uv) { stops.push_back({ color, uv }); return *this; }

	public:
		std::unique_ptr<i_filter> instance_filter(std::unique_ptr<c_geometry_command>&& child_command) const override {
			child_command->set_default_uvs();
			return std::make_unique<c_linear_gradient_filter>(std::move(child_command), angle, stops);
		}
	};
}