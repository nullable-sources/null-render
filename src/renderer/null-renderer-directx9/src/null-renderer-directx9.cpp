#include <null-renderer-directx9.h>
#include <shaders/passthrough-color/passthrough-color.h>
#include <shaders/passthrough-texture/passthrough-texture.h>
#include <shaders/quad-gradient/quad-gradient.h>
#include <shaders/sdf/sdf.h>

namespace null::render {
	void c_directx9::set_texture(void* texture) {
		device->SetTexture(0, (IDirect3DTexture9*)texture);
	}

	void c_directx9::set_clip(const rect_t<float>& rect) {
		const RECT _clip_rect{ (LONG)rect.min.x, (LONG)rect.min.y, (LONG)rect.max.x, (LONG)rect.max.y };
		device->SetScissorRect(&_clip_rect);
	}

	void c_directx9::draw_geometry(const size_t& vertex_count, const size_t& index_count, const size_t& vertex_offset, const size_t& index_offset) {
		device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, vertex_offset, 0, vertex_count, index_offset, index_count / 3);
	}

	void c_directx9::initialize() {
		if(device) device->AddRef();
		impl::shaders::passthrough_color = std::make_unique<renderer::shaders::c_passthrough_color>();
		impl::shaders::passthrough_texture = std::make_unique<renderer::shaders::c_passthrough_texture>();

		impl::shaders::quad_gradient = std::make_unique<renderer::shaders::c_quad_gradient>();
		impl::shaders::sdf = std::make_unique<renderer::shaders::c_sdf>();
	}

	void c_directx9::begin_render() {
		static int vtx_buffer_size{ 5000 }, idx_buffer_size{ 10000 };
		if(!vertex_buffer || vtx_buffer_size < geometry_buffer.vertex_buffer.size()) {
			if(vertex_buffer) { vertex_buffer->Release(); vertex_buffer = nullptr; }
			vtx_buffer_size = geometry_buffer.vertex_buffer.size() + 5000;
			if(auto result{ device->CreateVertexBuffer(vtx_buffer_size * sizeof(vertex_t), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, 0, D3DPOOL_DEFAULT, &vertex_buffer, nullptr) }; FAILED(result))
				utils::logger.log(utils::e_log_type::error, "cant create vertex buffer, return code {}.", result);
		}

		if(!index_buffer || idx_buffer_size < geometry_buffer.index_buffer.size()) {
			if(index_buffer) { index_buffer->Release(); index_buffer = nullptr; }
			idx_buffer_size = geometry_buffer.index_buffer.size() + 10000;
			if(auto result{ device->CreateIndexBuffer(idx_buffer_size * sizeof(impl::index_t), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_DEFAULT, &index_buffer, nullptr) }; FAILED(result))
				utils::logger.log(utils::e_log_type::error, "cant create index buffer, return code {}.", result);
		}

		IDirect3DStateBlock9* state_block{ };
		if(auto result{ device->CreateStateBlock(D3DSBT_ALL, &state_block) }; FAILED(result))
			utils::logger.log(utils::e_log_type::error, "cant create state_block, return code {}.", result);

		vertex_t* vertex_dst{ };
		impl::index_t* index_dst{ };
		if(auto result{ vertex_buffer->Lock(0, (UINT)(geometry_buffer.vertex_buffer.size() * sizeof(vertex_t)), (void**)&vertex_dst, D3DLOCK_DISCARD) }; FAILED(result))
			utils::logger.log(utils::e_log_type::error, "cant lock vertex buffer, return code {}.", result);
		if(auto result{ index_buffer->Lock(0, (UINT)(geometry_buffer.index_buffer.size() * sizeof(impl::index_t)), (void**)&index_dst, D3DLOCK_DISCARD) }; FAILED(result))
			utils::logger.log(utils::e_log_type::error, "cant lock index buffer, return code {}.", result);
		
		std::ranges::transform(geometry_buffer.vertex_buffer, vertex_dst, [](const impl::vertex_t& vertex) { return vertex_t{ vertex.pos, vertex.uv, vertex.color.cast<byte>() }; });
		std::ranges::move(geometry_buffer.index_buffer, index_dst);
		
		vertex_buffer->Unlock();
		index_buffer->Unlock();
		device->SetStreamSource(0, vertex_buffer, 0, sizeof(vertex_t));
		device->SetIndices(index_buffer);
		device->SetVertexDeclaration(vertex_declaration);

		setup_state();

		impl::shaders::passthrough_color->use();

		background.handle();
		background.clear();

		state_block->Apply();
		state_block->Release();
	}

	void c_directx9::end_render() {
		geometry_buffer.clear();
	}

	void c_directx9::setup_state() {
		D3DVIEWPORT9 viewport{ 0, 0,
			render::shared::viewport.x,
			render::shared::viewport.y,
			0.0f, 1.0f
		};
		device->SetViewport(&viewport);

		device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		device->SetRenderState(D3DRS_LIGHTING, false);
		device->SetRenderState(D3DRS_ZENABLE, false);
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		device->SetRenderState(D3DRS_ALPHATESTENABLE, false);
		device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		device->SetRenderState(D3DRS_SCISSORTESTENABLE, true);
		device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
		device->SetRenderState(D3DRS_FOGENABLE, false);
		device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
		device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

		set_clip({ { 0 }, render::shared::viewport });
		set_matrix(matrix4x4_t::project_ortho(0.5f, render::shared::viewport.x + 0.5f, render::shared::viewport.y + 0.5f, 0.5f, -10000.f, 10000.f));
		render::impl::shaders::event_dispatcher.setup_state();
	}

	void c_directx9::create_objects() {
		if(!device) return;

		render::impl::shaders::event_dispatcher.create();

		if(!vertex_declaration) {
			constexpr D3DVERTEXELEMENT9 elements[]{
				{ 0, offsetof(impl::vertex_t, pos), D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
				{ 0, offsetof(impl::vertex_t, uv), D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
				{ 0, offsetof(impl::vertex_t, color), D3DDECLTYPE_UBYTE4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
				D3DDECL_END()
			};
			device->CreateVertexDeclaration(elements, &vertex_declaration);
		}

		create_atlases();
	}

	void c_directx9::destroy_objects() {
		if(!device) return;

		render::impl::shaders::event_dispatcher.destroy();

		if(vertex_buffer) { vertex_buffer->Release(); vertex_buffer = nullptr; }
		if(index_buffer) { index_buffer->Release(); index_buffer = nullptr; }
		if(vertex_declaration) { vertex_declaration->Release(); vertex_declaration = nullptr; }

		destroy_atlases();
	}

	void* c_directx9::create_texture(const vec2_t<float>& size, void* data) {
		IDirect3DTexture9* texture{ };
		if(auto result{ device->CreateTexture(render::atlas.texture.size.x, render::atlas.texture.size.y, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &texture, nullptr) }; result != D3D_OK) {
			utils::logger.log(utils::e_log_type::warning, "failed to create texture, return code {}.", result);
			return nullptr;
		}

		if(data) {
			D3DLOCKED_RECT locked_rect{ };
			if(auto result{ texture->LockRect(0, &locked_rect, nullptr, 0) }; FAILED(result)) {
				utils::logger.log(utils::e_log_type::error, "LockRect failed, return code {}.", result);
				return texture;
			}

			for(float x{ size.x * 4 }; const int& y : std::views::iota(0, size.y)) {
				std::memcpy((std::uint8_t*)locked_rect.pBits + locked_rect.Pitch * y, (std::uint8_t*)data + (int)x * y, x);
			}

			texture->UnlockRect(0);
		}

		return texture;
	}

	void c_directx9::destroy_texture(void* texture) {
		if(!texture) {
			utils::logger.log(utils::e_log_type::warning, "it is impossible to destroy the texture because it is empty.");
			return;
		}

		if(auto result{ ((IDirect3DTexture9*)texture)->Release() }; FAILED(result))
			utils::logger.log(utils::e_log_type::warning, "cant release texture, return code {}.", result);
		texture = nullptr;
	}
}