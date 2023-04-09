#include <font/font.h>

#include <backend/renderer/renderer.h>
#include <backend/shaders/sdf/sdf.h>
#include <backend/shaders/passthrough/passthrough.h>

#include <graphic/filters/sdf/sdf.h>
#include <font/loaders/truetype/truetype.h>

namespace null::render::filters {
	void c_sdf::handle() {
        float aa{ 0.25f / sdf_padding * float(sdf_pixel_size) / size };
        if(size < 16.f)
            aa *= std::pow(size / 16.f, 2);

        backend::renderer->set_texture(texture);
        backend::shaders::sdf->set_aa(aa);
        backend::shaders::sdf->set_outline_thickness(std::clamp(0.5f - thickness / 2.0f, aa, 0.5f));
        backend::shaders::sdf->set_outline_start(outline_start);
        backend::shaders::sdf->set_outline_end(outline_end);
        backend::shaders::sdf->use();

        child_command->handle();

        backend::shaders::passthrough_color->use();
        backend::renderer->set_texture(nullptr);
	}
}