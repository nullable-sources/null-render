module;
#include <memory>
export module null.render:graphic.draw_list.brush;

import null.sdk;

import :graphic.command;
import :graphic.commands.geometry;
import :graphic.filter;

export namespace null::render {
	struct brush_t {
	public:
		color_t<int> color{ };

	public: //@todo: P0847
		auto& set_color(const color_t<int>& _color) { color = _color; return *this; }

	public:
		virtual std::unique_ptr<i_command> prepare_command(std::unique_ptr<c_geometry_command>& command) const { return std::move(command); };
	};

	struct filter_brush_t : public brush_t {
	public:
		std::shared_ptr<i_filter_instancer> filter_instancer{ };

	public: //@todo: P0847
		template <typename filter_instancer_t> auto& set_filter(const filter_instancer_t& _filter_instancer) { filter_instancer = std::make_shared<filter_instancer_t>(_filter_instancer); return *this; }

	public:
		virtual std::unique_ptr<i_command> prepare_command(std::unique_ptr<c_geometry_command>& command) const {
			return filter_instancer->instance_filter(std::move(command));
		};
	};
}