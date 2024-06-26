#include "frame-buffer.h"

namespace null::render::directx9 {
    void c_frame_buffer::create() {
        if(!empty()) return;

        //@note: creating texture for render target
        if(type == backend::e_frame_buffer_type::postprocessing) {
            IDirect3DSurface9* main_surface{ };
            shared.device->GetRenderTarget(0, &main_surface);

            D3DSURFACE_DESC main_surface_desc{ };
            if(main_surface) {
                main_surface->GetDesc(&main_surface_desc);
                main_surface->Release();
            } else main_surface_desc.MultiSampleQuality = 0;

            if(auto result = shared.device->CreateTexture(size.x, size.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &texture, nullptr); FAILED(result)) {
                utils::logger(utils::e_log_type::error, "CreateTexture2D failed, return code {}.", result);
                return;
            }

            D3DMULTISAMPLE_TYPE multisample_type = D3DMULTISAMPLE_NONE;
            if(flags & backend::e_frame_buffer_flags::msaa) {
                if(flags & backend::e_frame_buffer_flags::msaa && shared::msaa_quality != 1) {
                    multisample_type = (D3DMULTISAMPLE_TYPE)shared::msaa_quality;
                }

                if(auto result = shared.device->CreateRenderTarget(size.x, size.y, D3DFMT_A8R8G8B8, multisample_type, main_surface_desc.MultiSampleQuality, false, &surface, nullptr); FAILED(result)) {
                    utils::logger(utils::e_log_type::error, "CreateRenderTarget failed, return code {}.", result);
                    return;
                }
            } else {
                if(texture) {
                    if(auto result = texture->GetSurfaceLevel(0, &surface); FAILED(result)) {
                        utils::logger(utils::e_log_type::error, "GetSurfaceLevel failed, return code {}.", result);
                        return;
                    }
                }
            }
        } else {
            if(auto result = shared.device->GetRenderTarget(0, &surface); FAILED(result)) {
                utils::logger(utils::e_log_type::error, "GetBuffer failed, return code {}.", result);
                return;
            }
        }
    }

    void c_frame_buffer::destroy() {
        if(surface) { surface->Release(); surface = nullptr; }
        if(texture) { texture->Release(); texture = nullptr; }
    }

    void c_frame_buffer::clear() {
        IDirect3DSurface9* old_rt{ };
        shared.device->GetRenderTarget(0, &old_rt);

        //@note: https://learn.microsoft.com/en-us/windows/win32/direct3d9/scissor-test#:~:text=IDirect3DDevice9%3A%3ASetRenderTarget%20resets%20the%20scissor%20rectangle%20to%20the%20full%20render%20target
        //       ����� ������������ ���� ����, ����� ������������� scissor ��� ������ SetRenderTarget, �� ��� � �� �������� �� ���� � ������������ � SetRenderTarget?
        use();
        shared.device->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

        //@note: saving scissor due to "dolbaeb pisal dx9 API & dx9 doc"
        RECT scissor{ };
        shared.device->GetScissorRect(&scissor);
        shared.device->SetRenderTarget(0, old_rt);
        shared.device->SetScissorRect(&scissor);
        old_rt->Release();
        old_rt = nullptr;
    }

    void c_frame_buffer::use() {
        //@note: saving scissor due to "dolbaeb pisal dx9 API & dx9 doc"
        RECT scissor{ };
        shared.device->GetScissorRect(&scissor);
        shared.device->SetRenderTarget(0, surface);
        shared.device->SetScissorRect(&scissor);
    }

    void c_frame_buffer::copy_from(i_frame_buffer* another_frame_buffer) {
        shared.device->StretchRect((IDirect3DSurface9*)another_frame_buffer->get_surface(), nullptr, surface, nullptr, D3DTEXF_LINEAR);
    }

    void c_frame_buffer::blit_region_from(i_frame_buffer* another_frame_buffer, const vec2_t<int>& blit_offset, const rect_t<int>& region) {
        const vec2_t<int> region_size = region.size();
        const RECT src_rect(region.min.x, region.min.y, region.max.x, region.max.y);
        const RECT dst_rect(blit_offset.x, blit_offset.y, blit_offset.x + region_size.x, blit_offset.y + region_size.y);
        shared.device->StretchRect((IDirect3DSurface9*)another_frame_buffer->get_surface(), &src_rect, surface, &dst_rect, D3DTEXF_LINEAR);
    }

    void c_frame_buffer::copy_in_texture(void* texture, const rect_t<int>& region) {
        IDirect3DSurface9* texture_surface{ };
        ((IDirect3DTexture9*)texture)->GetSurfaceLevel(0, &texture_surface);

        const RECT rect(region.min.x, region.min.y, region.max.x, region.max.y);
        shared.device->StretchRect(surface, &rect, texture_surface, &rect, D3DTEXF_LINEAR);
    }
}