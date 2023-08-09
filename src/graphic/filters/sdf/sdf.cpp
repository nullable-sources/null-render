#include <font/font.h>

#include <backend/renderer/renderer.h>
#include <backend/shaders/passthrough/passthrough.h>

#include <graphic/filters/sdf/sdf.h>
#include <font/loaders/truetype/truetype.h>

namespace null::render {
	void c_sdf_filter::handle() {
        float aa{ 0.25f / sdf_padding * float(sdf_pixel_size) / constants.aa };
        if(constants.aa < 16.f)
            aa *= std::pow(constants.aa / 16.f, 2);

        constants.aa = aa;
        constants.outline_thickness = std::clamp(0.5f - constants.outline_thickness / 2.0f, constants.aa, 0.5f);

        backend::renderer->set_texture(texture);
        backend::sdf_shader->set_constants(constants);
        backend::sdf_shader->use();

        child_command->handle();

        backend::passthrough_color_shader->use();
        backend::renderer->set_texture(nullptr);
	}
}