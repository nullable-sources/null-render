#include <graphic/filters/sdf/sdf.h>

#include <graphic/draw-list/text-styles/sdf-text-style/sdf-text-style.h>

namespace null::render {
	std::unique_ptr<commands::i_command> sdf_text_style_t::prepare_command(std::unique_ptr<commands::c_geometry>& command) const {
		std::unique_ptr<filters::c_sdf> filter{ std::make_unique<filters::c_sdf>(font->container_atlas->texture.data, outline_thickness, size, outline_start, outline_end) };
		filter->child_command = std::move(command);
		return std::move(filter);
	};
}