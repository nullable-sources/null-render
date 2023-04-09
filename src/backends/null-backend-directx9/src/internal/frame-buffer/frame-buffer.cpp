#include <null-render.h>

#include <internal/frame-buffer/frame-buffer.h>

namespace null::render::backend::directx9 {
    void c_frame_buffer::create() {
        if(!empty()) return;

        //@note: creating texture for render target
        if(type == e_frame_buffer_type::postprocessing) {
            IDirect3DSurface9* main_surface{ };
            shared.device->GetRenderTarget(0, &main_surface);

            D3DSURFACE_DESC main_surface_desc{ };
            main_surface->GetDesc(&main_surface_desc);

            if(auto result{ shared.device->CreateTexture(shared::viewport.x, shared::viewport.y, 1, D3DUSAGE_RENDERTARGET, main_surface_desc.Format, D3DPOOL_DEFAULT, &texture, nullptr) }; FAILED(result)) {
                utils::logger.log(utils::e_log_type::error, "CreateTexture2D failed, return code {}.", result);
                return;
            }

            //@note: we get surface textures for later copying or creating a rengering target
            if(auto result{ texture->GetSurfaceLevel(0, &surface) }; FAILED(result)) {
                utils::logger.log(utils::e_log_type::error, "GetSurfaceLevel failed, return code {}.", result);
                return;
            }

            D3DMULTISAMPLE_TYPE multisample_type{ D3DMULTISAMPLE_NONE };
            if(flags & e_frame_buffer_flags::msaa && shared::msaa_quality != 1) {
                multisample_type = (D3DMULTISAMPLE_TYPE)shared::msaa_quality;
            }

            //@note: creating render target
            if(auto result{ shared.device->CreateRenderTarget(shared::viewport.x, shared::viewport.y, main_surface_desc.Format, multisample_type, 0, false, &surface, nullptr) }; FAILED(result)) {
                utils::logger.log(utils::e_log_type::error, "CreateRenderTarget failed, return code {}.", result);
                return;
            }
        } else {
            if(auto result{ shared.device->GetRenderTarget(0, &surface) }; FAILED(result)) {
                utils::logger.log(utils::e_log_type::error, "GetBuffer failed, return code {}.", result);
                return;
            }
        }
    }

    void c_frame_buffer::destroy() {
        if(texture) { texture->Release(); texture = nullptr; }
        if(surface) { surface->Release(); surface = nullptr; }
    }

    void c_frame_buffer::clear() {
        shared.device->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
    }

    void c_frame_buffer::set() {
        shared.device->SetRenderTarget(0, surface);
    }

    void c_frame_buffer::copy_from_backbuffer() {
        IDirect3DSurface9* backbuffer{ };
        shared.device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);

        shared.device->StretchRect(backbuffer, nullptr, (IDirect3DSurface9*)get_texture(), nullptr, D3DTEXF_LINEAR);

        backbuffer->Release();
    }

    void c_frame_buffer::copy_from_another_frame_buffer(const std::unique_ptr<i_frame_buffer>& another_frame_buffer) {
        shared.device->StretchRect((IDirect3DSurface9*)another_frame_buffer->get_texture(), nullptr, (IDirect3DSurface9*)get_texture(), nullptr, D3DTEXF_LINEAR);
    }
}