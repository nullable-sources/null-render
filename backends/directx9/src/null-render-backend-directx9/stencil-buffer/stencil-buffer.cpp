#include "stencil-buffer.h"

namespace ntl::render::directx9 {
    void c_stencil_buffer::create() {
        if(!empty()) return;

        shared.device->CreateDepthStencilSurface(shared::viewport.x, shared::viewport.y, D3DFMT_D24S8, (D3DMULTISAMPLE_TYPE)shared::msaa_quality, 0, false, &buffer, nullptr);
    }

    void c_stencil_buffer::destroy() {
        if(buffer) { buffer->Release(); buffer = nullptr; }
    }

    void c_stencil_buffer::clear() {
        IDirect3DSurface9* old_dss{ };
        shared.device->GetDepthStencilSurface(&old_dss);

        shared.device->SetDepthStencilSurface(buffer);
        shared.device->Clear(0, nullptr, D3DCLEAR_STENCIL, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

        shared.device->SetDepthStencilSurface(old_dss);
        if(old_dss) { old_dss->Release(); old_dss = nullptr; }
    }
}