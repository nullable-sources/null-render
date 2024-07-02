#include "sdf-text.h"

namespace null::render {
    std::shared_ptr<c_sdf_filter> c_sdf_brush::instance_sdf_filter() const { return c_sdf_filter::instance(); }
    std::shared_ptr<c_msdf_filter> c_msdf_brush::instance_sdf_filter() const { return c_msdf_filter::instance(); }
    std::shared_ptr<c_mtsdf_filter> c_mtsdf_brush::instance_sdf_filter() const {
        std::shared_ptr<c_mtsdf_filter> filter = c_mtsdf_filter::instance();
        filter->set_rounds(glyph_rounding, outline_rounding);
        return std::move(filter);
    }
}