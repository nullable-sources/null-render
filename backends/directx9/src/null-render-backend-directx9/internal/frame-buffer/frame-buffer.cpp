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
                shared.device->CreateDepthStencilSurface(size.x, size.y, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, false, &stencil, nullptr);

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
        if(stencil) { stencil->Release(); stencil = nullptr; }
        if(offscreen_transfer) { offscreen_transfer->Release(); offscreen_transfer = nullptr; }
    }

    void c_frame_buffer::clear() {
        std::uint32_t clear_flags = D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER;
        if(stencil) clear_flags |= D3DCLEAR_STENCIL;
        IDirect3DSurface9* old_rt{ }, *old_dss{ };
        shared.device->GetRenderTarget(0, &old_rt);
        if(stencil) shared.device->GetDepthStencilSurface(&old_dss);

        //@note: https://learn.microsoft.com/en-us/windows/win32/direct3d9/scissor-test#:~:text=IDirect3DDevice9%3A%3ASetRenderTarget%20resets%20the%20scissor%20rectangle%20to%20the%20full%20render%20target
        //       каким гидроцефалом надо быть, чтобы устанавливать scissor при вызове SetRenderTarget, да ещё и не написать об этом в документации к SetRenderTarget?
        use();
        shared.device->Clear(0, nullptr, clear_flags, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

        //@note: saving scissor due to "dolbaeb pisal dx9 API & dx9 doc"
        RECT scissor{ };
        shared.device->GetScissorRect(&scissor);
        shared.device->SetRenderTarget(0, old_rt);
        shared.device->SetScissorRect(&scissor);
        if(stencil) shared.device->SetDepthStencilSurface(old_dss);
        if(old_rt) { old_rt->Release(); old_rt = nullptr; }
        if(old_dss) { old_dss->Release(); old_dss = nullptr; }
    }

    void c_frame_buffer::use() {
        //@note: saving scissor due to "dolbaeb pisal dx9 API & dx9 doc"
        RECT scissor{ };
        shared.device->GetScissorRect(&scissor);
        shared.device->SetRenderTarget(0, surface);
        shared.device->SetDepthStencilSurface(stencil ? stencil : (IDirect3DSurface9*)backend::stencil_buffer->get_buffer());
        shared.device->SetScissorRect(&scissor);
    }

    void c_frame_buffer::copy_from(i_frame_buffer* another_frame_buffer) {
        if(auto result = shared.device->StretchRect((IDirect3DSurface9*)another_frame_buffer->get_surface(), nullptr, surface, nullptr, D3DTEXF_LINEAR); FAILED(result))
            utils::logger(utils::e_log_type::error, "StretchRect failed in copy_from, return code: {}.", result);
    }

    void c_frame_buffer::blit_region_from(i_frame_buffer* another_frame_buffer, const vec2_t<int>& blit_offset, const rect_t<int>& region) {
        const vec2_t<int> region_size = region.size();
        const RECT src_rect(region.min.x, region.min.y, region.max.x, region.max.y);
        const RECT dst_rect(blit_offset.x, blit_offset.y, blit_offset.x + region_size.x, blit_offset.y + region_size.y);
        if(auto result = shared.device->StretchRect((IDirect3DSurface9*)another_frame_buffer->get_surface(), &src_rect, surface, &dst_rect, D3DTEXF_LINEAR); FAILED(result))
            utils::logger(utils::e_log_type::error, "StretchRect failed in blit_region_from, return code: {}.", result);
    }

    void c_frame_buffer::copy_in_texture(void* texture, const rect_t<int>& region) {
        IDirect3DSurface9* texture_surface{ };
        ((IDirect3DTexture9*)texture)->GetSurfaceLevel(0, &texture_surface);
        if(!offscreen_transfer) {
            if(auto result = shared.device->CreateOffscreenPlainSurface(size.x, size.y, D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM, &offscreen_transfer, nullptr); FAILED(result)) {
                utils::logger(utils::e_log_type::error, "failed to create pain offscreen surface, return code: {}.", result);
                return;
            }
        }

        if(auto result = shared.device->GetRenderTargetData(surface, offscreen_transfer); FAILED(result)) {
            utils::logger(utils::e_log_type::error, "failed to get data from rendertarget, return code: {}.", result);
            return;
        }

        const RECT rect(region.min.x, region.min.y, region.max.x, region.max.y);
        const POINT point(region.min.x, region.min.y);
        if(auto result = shared.device->UpdateSurface(offscreen_transfer, &rect, texture_surface, &point); FAILED(result))
            utils::logger(utils::e_log_type::error, "failed to update texture surface, return code: {}.", result);

        if(texture_surface) { texture_surface->Release(); texture_surface = nullptr; }
    }
}