#include <graphic/filters/texture/texture.h>
#include <graphic/draw-list/text-styles/text-style/text-style.h>

namespace null::render {
	std::unique_ptr<commands::i_command> text_style_t::prepare_command(std::unique_ptr<commands::c_geometry>& command) const {
		std::unique_ptr<filters::c_texture> filter{ std::make_unique<filters::c_texture>(font->container_atlas->texture.data) };
		filter->child_command = std::move(command);
		return std::move(filter);
	};
}