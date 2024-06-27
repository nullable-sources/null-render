#pragma once
#include "../../../backend/shaders/sdf.h"
#include "../texture/texture.h"

namespace null::render {
    class c_sdf_filter : public c_texture_filter {
    public:
        static std::shared_ptr<c_sdf_filter> instance() { return std::make_shared<c_sdf_filter>(); }
        static std::shared_ptr<c_sdf_filter> instance(float thickness, const color_t<int>& start, const color_t<int>& end) { return std::make_shared<c_sdf_filter>(thickness, start, end); }

    public:
        backend::i_sdf_shader::constants_t constants{ };

    public:
        c_sdf_filter() { }
        c_sdf_filter(float thickness, const color_t<int>& start, const color_t<int>& end)
            : constants{ .outline_start = start, .outline_end = end, .outline_thickness = thickness } { }
        virtual ~c_sdf_filter() { }

    public:
        void set_outline(float thickness, const color_t<int>& start, const color_t<int>& end) { constants.outline_thickness = thickness; constants.outline_start = start; constants.outline_end = end; }
        void set_aa(float aa) { constants.aa = aa; }
        void set_default_aa(float dst_aa);

    public:
        virtual void handle() override;

    public:
        virtual std::shared_ptr<i_filter> clone() const override {
            return std::shared_ptr<c_sdf_filter>(new c_sdf_filter(*this));
        }
    };
}
