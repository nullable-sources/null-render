#include "../../filters/texture/texture.h"
#include "text.h"

namespace ntl::render {
    std::shared_ptr<i_command> c_text_brush::prepare_command(std::shared_ptr<c_geometry_command>&& command) const {
        std::shared_ptr<c_texture_filter> filter = c_texture_filter::instance();
        filter->set_texture(font ? font->get_parent_atlas()->texture.data : get_default_font()->get_parent_atlas()->texture.data);
        filter->set_child_command(command);
        return filter;
    };
}