#pragma once
#include <null-render/backend/state-pipeline/rasterizer-state.h>

namespace null::render::directx9 {
    class c_rasterizer_state : public backend::i_rasterizer_state {
    public:
        virtual void use() override;
    };
}