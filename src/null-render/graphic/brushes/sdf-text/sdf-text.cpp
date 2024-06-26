#include "sdf-text.h"

namespace null::render {
    std::shared_ptr<i_command> c_sdf_brush::prepare_command(std::shared_ptr<c_geometry_command>&& command) const {
        std::shared_ptr<c_sdf_filter> filter = c_sdf_filter::instance();
        filter->set_texture(font->container_atlas->texture.data);
        filter->set_outline(outline_thickness, outline_start, outline_end);
        if(aa == -1) filter->set_default_aa(font->size);
        else filter->set_aa(aa);
        filter->set_child_command(command);
        return filter;
    };
}