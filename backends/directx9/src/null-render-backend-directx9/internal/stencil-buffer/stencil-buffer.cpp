#include "stencil-buffer.h"

namespace null::render::directx9 {
	void c_stencil_buffer::create() {
		if(!empty()) return;

		shared.device->CreateDepthStencilSurface(shared::viewport.x, shared::viewport.y, D3DFMT_D24S8, (D3DMULTISAMPLE_TYPE)shared::msaa_quality, 0, false, &buffer, nullptr);
	}

	void c_stencil_buffer::destroy() {
		if(buffer) { buffer->Release(); buffer = nullptr; }
	}

	void c_stencil_buffer::clear() {
		IDirect3DSurface9* old_stencil{ };
		shared.device->GetDepthStencilSurface(&old_stencil);

		shared.device->SetDepthStencilSurface(buffer);
		shared.device->Clear(0, nullptr, D3DCLEAR_STENCIL, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

		shared.device->SetDepthStencilSurface(old_stencil);
		old_stencil->Release();
		old_stencil = nullptr;
	}

	void c_stencil_buffer::set_test(bool test) {
		shared.device->SetDepthStencilSurface(buffer);
		shared.device->SetRenderState(D3DRS_STENCILENABLE, test);
	}

	void c_stencil_buffer::set_operation(backend::e_stencil_operation operation) {
		if(operation == backend::e_stencil_operation::end_write) {
			shared.device->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_RED);
			shared.device->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
			shared.device->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
			shared.device->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);

			shared.device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
			shared.device->SetRenderState(D3DRS_STENCILREF, stencil_ref);
			shared.device->SetRenderState(D3DRS_STENCILMASK, -1);
			return;
		}

		shared.device->SetRenderState(D3DRS_COLORWRITEENABLE, 0);

		shared.device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
		shared.device->SetRenderState(D3DRS_STENCILREF, 1);
		shared.device->SetRenderState(D3DRS_STENCILMASK, -1);

		shared.device->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
		shared.device->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);

		switch(operation) {
			case backend::e_stencil_operation::set:
			case backend::e_stencil_operation::set_inverse: {
				shared.device->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);
				stencil_ref = operation == backend::e_stencil_operation::set ? 1 : 0;
			} break;

			case backend::e_stencil_operation::intersect: {
				shared.device->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCRSAT);
				stencil_ref++;
			} break;
		}
	}
}