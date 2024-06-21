#pragma once
#include "../filters/filter.h"
#include "brush.h"

namespace null::render {
    class c_filter_brush : public c_brush {
    public:
        static std::shared_ptr<c_filter_brush> instance() { return std::make_shared<c_filter_brush>(); }
        static std::shared_ptr<c_filter_brush> instance(const std::shared_ptr<i_filter>& filter) { return std::make_shared<c_filter_brush>(filter); }

    public:
        std::shared_ptr<i_filter> filter{ };

    public:
        c_filter_brush() { }
        c_filter_brush(const std::shared_ptr<i_filter>& _filter) : filter(_filter) { }
        virtual ~c_filter_brush() { }

    public:
        void set_filter(const std::shared_ptr<i_filter>& _filter) { filter = _filter; }

    public:
        virtual std::shared_ptr<i_command> prepare_command(std::shared_ptr<c_geometry_command>&& command) const;
    };
}