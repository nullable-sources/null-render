#pragma once
#include <graphic/filters/filters.h>
#include <graphic/commands/geometry-command/geometry-command.h>

namespace null::render {
	struct brush_t {
	public:
		color_t<int> color{ };

	public:
		template <typename self_t> auto&& set_color(this self_t&& self, const color_t<int>& color) { self.color = color; return self; }

	public:
		virtual std::unique_ptr<commands::i_command> prepare_command(std::unique_ptr<commands::c_geometry>& command) const { return std::move(command); };
	};

	struct filter_brush_t : public brush_t {
	public:
		std::shared_ptr<filters::i_filter_instancer> filter_instancer{ };

	public:
		template <typename self_t, typename filter_instancer_t> auto&& set_filter(this self_t&& self, const filter_instancer_t& filter_instancer) { self.filter_instancer = std::make_shared<filter_instancer_t>(filter_instancer); return self; }

	public:
		virtual std::unique_ptr<commands::i_command> prepare_command(std::unique_ptr<commands::c_geometry>& command) const {
			return filter_instancer->instance_filter(std::move(command));
		};
	};
}