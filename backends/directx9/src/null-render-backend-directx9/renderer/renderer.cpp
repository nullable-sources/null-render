#include "renderer.h"

namespace null::render::directx9 {
    matrix4x4_t c_renderer::get_projection_matrix() const {
        return matrix4x4_t::projection_ortho(0.f, render::shared::viewport.x, render::shared::viewport.y, 0.f, -10000.f, 10000.f);
    }

    void c_renderer::set_texture(void* texture) {
        shared.device->SetTexture(0, (IDirect3DTexture9*)texture);
    }

    void c_renderer::set_clip(const rect_t<float>& rect) {
        const RECT _clip_rect(rect.min.x, rect.min.y, rect.max.x, rect.max.y);
        shared.device->SetScissorRect(&_clip_rect);
    }

    void c_renderer::draw_geometry(backend::e_topology topology, size_t vertex_count, size_t index_count, size_t vertex_offset, size_t index_offset) {
        shared.device->DrawIndexedPrimitive(topology == backend::e_topology::triangle_list ? D3DPT_TRIANGLELIST : D3DPT_TRIANGLESTRIP, vertex_offset, 0, vertex_count, index_offset, topology == backend::e_topology::triangle_list ? (index_count / 3) : (index_count - 2));
    }

    void* c_renderer::create_texture(const vec2_t<float>& size, void* data) {
        IDirect3DTexture9* texture{ };
        if(auto result = shared.device->CreateTexture(size.x, size.y, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &texture, nullptr); result != D3D_OK) {
            utils::logger(utils::e_log_type::warning, "failed to create texture, return code {}.", result);
            return nullptr;
        }

        if(data) {
            D3DLOCKED_RECT locked_rect{ };
            if(auto result = texture->LockRect(0, &locked_rect, nullptr, 0); FAILED(result)) {
                utils::logger(utils::e_log_type::error, "LockRect failed, return code {}.", result);
                return texture;
            }

            for(float x = size.x * 4; int y : std::views::iota(0, size.y)) {
                std::memcpy((std::uint8_t*)locked_rect.pBits + locked_rect.Pitch * y, (std::uint8_t*)data + (int)x * y, x);
            }

            texture->UnlockRect(0);
        }

        return texture;
    }

    void c_renderer::destroy_texture(void* texture) {
        if(!texture) {
            utils::logger(utils::e_log_type::warning, "it is impossible to destroy the texture because it is empty.");
            return;
        }

        if(auto result = ((IDirect3DTexture9*)texture)->Release(); FAILED(result))
            utils::logger(utils::e_log_type::warning, "cant release texture, return code {}.", result);
    }
}