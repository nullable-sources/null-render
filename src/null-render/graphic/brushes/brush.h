#pragma once
#include "../commands/geometry/geometry.h"

namespace null::render {
    class c_brush {
    public:
        static std::shared_ptr<c_brush> instance() { return std::make_shared<c_brush>(); }
        static std::shared_ptr<c_brush> instance(const color_t<int>& color) { return std::make_shared<c_brush>(color); }

    public:
        color_t<int> color{ };

    public:
        c_brush() { }
        c_brush(const color_t<int>& _color) : color(_color) { }
        virtual ~c_brush() { }

    public:
        void set_color(const color_t<int>& _color) { color = _color; }

    public:
        virtual std::shared_ptr<i_command> prepare_command(std::shared_ptr<c_geometry_command>&& command) const;
    };
}