#include <null-renderer-directx9.h>

namespace null::render::directx9 {
	void render_draw_data(c_draw_list::draw_data_t* _draw_data) {
		if(_draw_data->window_size <= vec2_t{ 0.f })
			throw std::runtime_error{ "_draw_data->window_size <= 0.f" };

		static int vtx_buffer_size{ 5000 }, idx_buffer_size{ 10000 };
		if(!vtx_buffer || vtx_buffer_size < _draw_data->total_vtx_count) {
			if(vtx_buffer) { vtx_buffer->Release(); vtx_buffer = nullptr; }
			vtx_buffer_size = _draw_data->total_vtx_count + 5000;
			if(device->CreateVertexBuffer(vtx_buffer_size * sizeof(vertex_t), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1, D3DPOOL_DEFAULT, &vtx_buffer, nullptr) < 0)
				throw std::runtime_error{ "CreateVertexBuffer error" };
		}

		if(!idx_buffer || idx_buffer_size < _draw_data->total_idx_count) {
			if(idx_buffer) { idx_buffer->Release(); idx_buffer = nullptr; }
			idx_buffer_size = _draw_data->total_idx_count + 10000;
			if(device->CreateIndexBuffer(idx_buffer_size * sizeof(std::uint16_t), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, sizeof(std::uint16_t) == 2 ? D3DFMT_INDEX16 : D3DFMT_INDEX32, D3DPOOL_DEFAULT, &idx_buffer, nullptr) < 0)
				throw std::runtime_error{ "CreateIndexBuffer error" };
		}

		IDirect3DStateBlock9* d3d9_state_block{ };
		if(device->CreateStateBlock(D3DSBT_ALL, &d3d9_state_block) < 0)
			throw std::runtime_error{ "CreateStateBlock error" };

		D3DMATRIX last_world{ }, last_view{ }, last_projection{ };
		device->GetTransform(D3DTS_WORLD, &last_world);
		device->GetTransform(D3DTS_VIEW, &last_view);
		device->GetTransform(D3DTS_PROJECTION, &last_projection);

		vertex_t* vtx_dst{ };
		std::uint16_t* idx_dst{ };
		if(vtx_buffer->Lock(0, (UINT)(_draw_data->total_vtx_count * sizeof(vertex_t)), (void**)&vtx_dst, D3DLOCK_DISCARD) < 0) throw std::runtime_error{ "vtx_buffer->Lock error" };
		if(idx_buffer->Lock(0, (UINT)(_draw_data->total_idx_count * sizeof(std::uint16_t)), (void**)&idx_dst, D3DLOCK_DISCARD) < 0) throw std::runtime_error{ "idx_buffer->Lock error" };
		for(c_draw_list* cmd_list : _draw_data->cmd_lists) {
			for(c_draw_list::vertex_t vtx_src : cmd_list->vtx_buffer) {
				vtx_dst->pos[0] = vtx_src.pos.x; vtx_dst->pos[1] = vtx_src.pos.y;;

				std::uint32_t color = (std::uint32_t)vtx_src.color;
				vtx_dst->color = (color & 0xFF00FF00) | ((color & 0xFF0000) >> 16) | ((color & 0xFF) << 16);

				vtx_dst->uv[0] = vtx_src.uv.x; vtx_dst->uv[1] = vtx_src.uv.y;

				vtx_dst++;
			}
			memcpy(idx_dst, cmd_list->idx_buffer.data(), cmd_list->idx_buffer.size() * sizeof(std::uint16_t));
			idx_dst += cmd_list->idx_buffer.size();
		}
		vtx_buffer->Unlock();
		idx_buffer->Unlock();
		device->SetStreamSource(0, vtx_buffer, 0, sizeof(vertex_t));
		device->SetIndices(idx_buffer);
		device->SetVertexDeclaration(vtx_declaration);
		device->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);

		setup_render_state();

		int global_vtx_offset{ }, global_idx_offset{ };
		for(c_draw_list* draw_list : _draw_data->cmd_lists) {
			for(c_draw_list::cmd_t cmd : draw_list->cmd_buffer) {
				if(cmd.callbacks.have_callback(e_cmd_callbacks::render_draw_data) && std::any_cast<bool>(cmd.callbacks.call<bool(c_draw_list::cmd_t*)>(e_cmd_callbacks::render_draw_data, &cmd))) {
					setup_render_state();
					continue;
				}

				const RECT clip_rect{ (LONG)(cmd.clip_rect.min.x - _draw_data->window_pos.x), (LONG)(cmd.clip_rect.min.y - _draw_data->window_pos.y), (LONG)(cmd.clip_rect.max.x - _draw_data->window_pos.x), (LONG)(cmd.clip_rect.max.y - _draw_data->window_pos.y) };
				device->SetTexture(0, (IDirect3DTexture9*)cmd.texture_id);
				device->SetScissorRect(&clip_rect);
				device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, cmd.vtx_offset + global_vtx_offset, 0, (UINT)draw_list->vtx_buffer.size(), cmd.idx_offset + global_idx_offset, cmd.element_count / 3);
			}
			global_idx_offset += draw_list->idx_buffer.size();
			global_vtx_offset += draw_list->vtx_buffer.size();
		}

		device->SetTransform(D3DTS_WORLD, &last_world);
		device->SetTransform(D3DTS_VIEW, &last_view);
		device->SetTransform(D3DTS_PROJECTION, &last_projection);

		d3d9_state_block->Apply();
		d3d9_state_block->Release();
	}

	void setup_render_state(c_draw_list::draw_data_t* _draw_data) {
		D3DVIEWPORT9 viewport{ 0, 0,
			_draw_data->window_size.x,
			_draw_data->window_size.y,
			0.0f, 1.0f
		};

		device->SetViewport(&viewport);

		device->SetPixelShader(nullptr);
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

		{
			float l{ 0.5f }, r{ _draw_data->window_size.x + 0.5f }, t{ 0.5f }, b{ _draw_data->window_size.y + 0.5f };

			D3DMATRIX mat_identity{ { { 1.0f, 0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 0.0f, 1.0f } } };
			D3DMATRIX mat_projection{ { {
				2.f / (r - l),		0.f,				0.f,	0.f,
				0.f,				2.f / (t - b),		0.f,	0.f,
				0.f,				0.f,				0.5f,	0.f,
				(l + r) / (l - r),  (t + b) / (b - t),	0.5f,	1.f
			} } };
			device->SetTransform(D3DTS_WORLD, &mat_identity);
			device->SetTransform(D3DTS_VIEW, &mat_identity);
			device->SetTransform(D3DTS_PROJECTION, &mat_projection);
		}
	}

	void create_fonts_texture() {
		if(global_atlas.texture.pixels_alpha8.empty()) {
			if(global_atlas.configs.empty()) global_atlas.add_font_default();
			global_atlas.build_with_stb_truetype();
		}

		global_atlas.texture.get_data_as_rgba32();

		font_texture = nullptr;
		if(device->CreateTexture(global_atlas.texture.size.x, global_atlas.texture.size.y, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &font_texture, nullptr) < 0)
			throw std::runtime_error{ "cannot create font texture" };

		D3DLOCKED_RECT tex_locked_rect{ };
		if(int result = font_texture->LockRect(0, &tex_locked_rect, nullptr, 0); result != D3D_OK)
			throw std::runtime_error{ std::format("lock rect error, code {}", result) };

		for(int y : std::views::iota(0, global_atlas.texture.size.y)) {
			int size = global_atlas.texture.size.x * 4;
			std::memcpy((std::uint8_t*)tex_locked_rect.pBits + tex_locked_rect.Pitch * y, (std::uint8_t*)global_atlas.texture.pixels_rgba32.data() + size * y, size);
		}

		font_texture->UnlockRect(0);

		global_atlas.texture.id = (void*)font_texture;
	}

	void create_device_objects() {
		if(!device) return;
		create_fonts_texture();

		if(!vtx_declaration) {
			constexpr D3DVERTEXELEMENT9 elements[]{
				{ 0, 0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
				{ 0, 8, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
				{ 0, 16, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
				D3DDECL_END()
			};
			device->CreateVertexDeclaration(elements, &vtx_declaration);
		}
	}

	void invalidate_device_objects() {
		if(!device) return;

		if(vtx_buffer) { vtx_buffer->Release(); vtx_buffer = nullptr; }
		if(idx_buffer) { idx_buffer->Release(); idx_buffer = nullptr; }
		if(vtx_declaration) { vtx_declaration->Release(); vtx_declaration = nullptr; }
		if(font_texture) { font_texture->Release(); font_texture = nullptr; global_atlas.texture.id = nullptr; }
	}
}