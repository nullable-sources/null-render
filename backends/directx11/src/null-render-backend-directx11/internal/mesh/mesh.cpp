#include "../../shaders/passthrough/compiled-object.h"
#include "mesh.h"

namespace ntl::render::directx11 {
    void c_mesh::create() {
        if(input_layout) return;

        D3D11_INPUT_ELEMENT_DESC desc[] = {
            { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, (UINT)offsetof(backend::vertex_t, pos), D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, (UINT)offsetof(backend::vertex_t, uv), D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, (UINT)offsetof(backend::vertex_t, color), D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };
        if(auto result = shared.device->CreateInputLayout(desc, 3, sources::passthrough_vs().data(), sources::passthrough_vs().size(), &input_layout); FAILED(result))
            sdk::logger(sdk::e_log_type::error, "cant create vertex input layout, return code {}.", result);
    }

    void c_mesh::destroy() {
        if(index_buffer) { index_buffer->Release(); index_buffer = nullptr; }
        if(vertex_buffer) { vertex_buffer->Release(); vertex_buffer = nullptr; }
        if(input_layout) { input_layout->Release(); input_layout = nullptr; }
    }

    void c_mesh::compile() {
        if(!input_layout) create();

        static int vertex_buffer_size = 5000, index_buffer_size = 10000;
        if(!vertex_buffer || vertex_buffer_size < geometry_buffer.vertex_buffer.size()) {
            if(vertex_buffer) { vertex_buffer->Release(); vertex_buffer = nullptr; }
            vertex_buffer_size = geometry_buffer.vertex_buffer.size() + 5000;
            D3D11_BUFFER_DESC buffer_desc{
                .ByteWidth{ vertex_buffer_size * sizeof(backend::vertex_t) },
                .Usage{ D3D11_USAGE_DYNAMIC },
                .BindFlags{ D3D11_BIND_VERTEX_BUFFER },
                .CPUAccessFlags{ D3D11_CPU_ACCESS_WRITE },
                .MiscFlags{ 0 }
            };
            if(auto result = shared.device->CreateBuffer(&buffer_desc, nullptr, &vertex_buffer); FAILED(result)) {
                sdk::logger(sdk::e_log_type::error, "cant create vertex buffer, return code {}.", result);
                return;
            }
        }

        if(!index_buffer || index_buffer_size < geometry_buffer.index_buffer.size()) {
            if(index_buffer) { index_buffer->Release(); index_buffer = nullptr; }
            index_buffer_size = geometry_buffer.index_buffer.size() + 10000;
            D3D11_BUFFER_DESC buffer_desc{
                .ByteWidth{ index_buffer_size * sizeof(backend::index_t) },
                .Usage{ D3D11_USAGE_DYNAMIC },
                .BindFlags{ D3D11_BIND_INDEX_BUFFER },
                .CPUAccessFlags{ D3D11_CPU_ACCESS_WRITE },
                .MiscFlags{ 0 }
            };

            if(auto result = shared.device->CreateBuffer(&buffer_desc, nullptr, &index_buffer); FAILED(result)) {
                sdk::logger(sdk::e_log_type::error, "cant create index buffer, return code {}.", result);
                return;
            }
        }

        D3D11_MAPPED_SUBRESOURCE vertex_buffer_subresource{ }, index_buffer_subresource{ };
        if(auto result = shared.context->Map(vertex_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &vertex_buffer_subresource); FAILED(result)) {
            sdk::logger(sdk::e_log_type::error, "map vertex buffer failed, return code {}.", result);
            return;
        }
        if(auto result = shared.context->Map(index_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &index_buffer_subresource); FAILED(result)) {
            sdk::logger(sdk::e_log_type::error, "map index buffer failed, return code {}.", result);
            return;
        }

        std::ranges::move(geometry_buffer.vertex_buffer, (backend::vertex_t*)vertex_buffer_subresource.pData);
        std::ranges::move(geometry_buffer.index_buffer, (backend::index_t*)index_buffer_subresource.pData);

        shared.context->Unmap(vertex_buffer, 0);
        shared.context->Unmap(index_buffer, 0);
    }

    void c_mesh::use() {
        std::uint32_t stride{ sizeof(backend::vertex_t) };
        std::uint32_t offset{ };
        shared.context->IASetInputLayout(input_layout);
        shared.context->IASetVertexBuffers(0, 1, &vertex_buffer, &stride, &offset);
        shared.context->IASetIndexBuffer(index_buffer, DXGI_FORMAT_R32_UINT, 0);
        shared.context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    }
}