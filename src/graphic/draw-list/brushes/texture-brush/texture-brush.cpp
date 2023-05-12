#include <graphic/filters/texture/texture.h>
#include <graphic/draw-list/brushes/texture-brush/texture-brush.h>

#include <backend/internal/mesh.h>

namespace null::render {
	std::unique_ptr<commands::i_command> texture_brush_t::prepare_command(std::unique_ptr<commands::c_geometry>& command) const {
		vec2_t min{ }, max{ };
		command->get_bounding_box(min, max);
		command->recalculate_uvs(min, max, uvs);

		return std::make_unique<filters::c_texture>(std::move(command), texture);
	}
}