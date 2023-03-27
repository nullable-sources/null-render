#pragma once
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
}