#pragma once

namespace null::render {
    class c_atlas;
    class i_font_loader {
    public:
        virtual void build(c_atlas* atlas) = 0;
    };
}