#include <font/font.h>

#include <renderer/renderer.h>
#include <renderer/shaders/sdf/sdf.h>
#include <renderer/shaders/passthrough/passthrough.h>

#include <graphic/filters/sdf/sdf.h>
#include <font/loaders/truetype/truetype.h>

namespace null::render::filters {
	void c_sdf::handle() {
        float aa{ 0.25f / sdf_padding * float(sdf_pixel_size) / size };
        if(size < 16.f)
            aa *= std::pow(size / 16.f, 2);

        renderer->set_texture(texture);
        impl::shaders::sdf->set_aa(aa);
        impl::shaders::sdf->set_outline_thickness(std::clamp(0.5f - thickness / 2.0f, aa, 0.5f));
        impl::shaders::sdf->set_outline_start(outline_start);
        impl::shaders::sdf->set_outline_end(outline_end);
        impl::shaders::sdf->use();

        child_command->handle();

        impl::shaders::passthrough_color->use();
        renderer->set_texture(nullptr);
	}
}