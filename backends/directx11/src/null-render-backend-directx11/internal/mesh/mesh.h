#pragma once
#include "../../renderer/renderer.h"

namespace null::render::directx11 {
    class c_mesh : public backend::c_mesh {
    public:
        ID3D11InputLayout* input_layout{ };
        ID3D11Buffer* vertex_buffer{ }, *index_buffer{ };

    public:
        void create() override;
        void destroy() override;

    public:
        void compile() override;

        void use() override;
    };
}