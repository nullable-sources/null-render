#pragma once

namespace null::render::backend {
    class i_usable_object {
    public:
        i_usable_object() { }
        virtual ~i_usable_object() { }

    public:
        virtual void use() = 0;
    };
}