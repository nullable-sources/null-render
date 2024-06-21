#pragma once
#include "../../filters/sdf/sdf.h"
#include "../text/text.h"

namespace null::render {
    class c_sdf_brush : public i_text_brush {
    public:
        static std::shared_ptr<c_sdf_brush> instance() { return std::make_shared<c_sdf_brush>(); }

    private:
        float outline_thickness{ };
        color_t<int> outline_start{ }, outline_end{ };

    public:
        c_sdf_brush() { }
        virtual ~c_sdf_brush() { }

    public:
        void set_outline(float thickness, const color_t<int>& start, const color_t<int>& end) { outline_thickness = thickness; outline_start = start; outline_end = end; }

    public:
        virtual std::shared_ptr<i_command> prepare_command(std::shared_ptr<c_geometry_command>&& command) const override;
    };
}