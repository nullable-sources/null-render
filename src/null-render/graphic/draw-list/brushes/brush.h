#pragma once
#include "../../filters/filters.h"
#include "../../commands/geometry/geometry.h"

namespace null::render {
	struct brush_t {
	public:
		color_t<int> color{ };

	public:
		template <typename self_t> self_t& set_color(this self_t& self, const color_t<int>& color) { self.color = color; return self; }

	public:
		virtual std::unique_ptr<i_command> prepare_command(std::unique_ptr<c_geometry_command>& command) const { return std::move(command); };
	};

	struct filter_brush_t : public brush_t {
	public:
		std::shared_ptr<i_filter_instancer> filter_instancer{ };

	public:
		template <typename self_t, typename filter_instancer_t> self_t& set_filter(this self_t& self, const filter_instancer_t& filter_instancer) { self.filter_instancer = std::make_shared<filter_instancer_t>(filter_instancer); return self; }

	public:
		virtual std::unique_ptr<i_command> prepare_command(std::unique_ptr<c_geometry_command>& command) const {
			return filter_instancer->instance_filter(std::move(command));
		};
	};
}