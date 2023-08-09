#include "renderer/renderer.h"

namespace null::render::directx9 {
	matrix4x4_t c_renderer::get_projection_matrix() const {
		return matrix4x4_t::projection_ortho(0.5f, render::shared::viewport.x + 0.5f, render::shared::viewport.y + 0.5f, 0.5f, -10000.f, 10000.f);
	}

	void c_renderer::set_texture(void* texture) {
		shared.device->SetTexture(0, (IDirect3DTexture9*)texture);
	}

	void c_renderer::set_clip(const rect_t<float>& rect) {
		const RECT _clip_rect{ (LONG)rect.min.x, (LONG)rect.min.y, (LONG)rect.max.x, (LONG)rect.max.y };
		shared.device->SetScissorRect(&_clip_rect);
	}

	void c_renderer::draw_geometry(size_t vertex_count, size_t index_count, size_t vertex_offset, size_t index_offset) {
		shared.device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, vertex_offset, 0, vertex_count, index_offset, index_count / 3);
	}

	void* c_renderer::create_texture(const vec2_t<float>& size, void* data) {
		IDirect3DTexture9* texture{ };
		if(auto result{ shared.device->CreateTexture(size.x, size.y, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &texture, nullptr) }; result != D3D_OK) {
			utils::logger(utils::e_log_type::warning, "failed to create texture, return code {}.", result);
			return nullptr;
		}

		if(data) {
			D3DLOCKED_RECT locked_rect{ };
			if(auto result{ texture->LockRect(0, &locked_rect, nullptr, 0) }; FAILED(result)) {
				utils::logger(utils::e_log_type::error, "LockRect failed, return code {}.", result);
				return texture;
			}

			for(float x{ size.x * 4 }; int y : std::views::iota(0, size.y)) {
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

		if(auto result{ ((IDirect3DTexture9*)texture)->Release() }; FAILED(result))
			utils::logger(utils::e_log_type::warning, "cant release texture, return code {}.", result);
		texture = nullptr;
	}

	void c_renderer::setup_state() {
		D3DVIEWPORT9 viewport{ 0, 0,
			render::shared::viewport.x,
			render::shared::viewport.y,
			0.0f, 1.0f
		};
		shared.device->SetViewport(&viewport);

		shared.device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		shared.device->SetRenderState(D3DRS_LIGHTING, false);
		shared.device->SetRenderState(D3DRS_ZENABLE, false);
		shared.device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		shared.device->SetRenderState(D3DRS_ALPHATESTENABLE, false);
		shared.device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		shared.device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		shared.device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		shared.device->SetRenderState(D3DRS_SCISSORTESTENABLE, true);
		shared.device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
		shared.device->SetRenderState(D3DRS_FOGENABLE, false);
		shared.device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		shared.device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		shared.device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		shared.device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		shared.device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		shared.device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
		shared.device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		shared.device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);


		set_clip({ { 0 }, render::shared::viewport });
		set_matrix(get_projection_matrix());
		backend::object_event_dispatcher.setup_state();

		backend::mesh->set();
	}

	void c_renderer::save_state() {
		if(auto result{ shared.device->CreateStateBlock(D3DSBT_ALL, &state_block) }; FAILED(result))
			utils::logger(utils::e_log_type::error, "cant create state_block, return code {}.", result);
	}

	void c_renderer::restore_state() {
		if(state_block) {
			state_block->Apply();
			state_block->Release();
			state_block = nullptr;
		}
	}
}