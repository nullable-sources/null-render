#include <null-renderer-directx9.h>

namespace null::renderer {
	void c_directx9::render(const compiled_geometry_data_t& _compiled_geometry_data) {
		if(render::shared::viewport <= 0.f)
			return;

		static int vtx_buffer_size{ 5000 }, idx_buffer_size{ 10000 };
		if(!vtx_buffer || vtx_buffer_size < _compiled_geometry_data.total_vtx_count) {
			if(vtx_buffer) { vtx_buffer->Release(); vtx_buffer = nullptr; }
			vtx_buffer_size = _compiled_geometry_data.total_vtx_count + 5000;
			if(device->CreateVertexBuffer(vtx_buffer_size * sizeof(vertex_t), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1, D3DPOOL_DEFAULT, &vtx_buffer, nullptr) < 0)
				throw std::runtime_error{ "CreateVertexBuffer error" };
		}

		if(!idx_buffer || idx_buffer_size < _compiled_geometry_data.total_idx_count) {
			if(idx_buffer) { idx_buffer->Release(); idx_buffer = nullptr; }
			idx_buffer_size = _compiled_geometry_data.total_idx_count + 10000;
			if(device->CreateIndexBuffer(idx_buffer_size * sizeof(std::uint32_t), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_DEFAULT, &idx_buffer, nullptr) < 0)
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
		std::uint32_t* idx_dst{ };
		if(vtx_buffer->Lock(0, (UINT)(_compiled_geometry_data.total_vtx_count * sizeof(vertex_t)), (void**)&vtx_dst, D3DLOCK_DISCARD) < 0) throw std::runtime_error{ "vtx_buffer->Lock error" };
		if(idx_buffer->Lock(0, (UINT)(_compiled_geometry_data.total_idx_count * sizeof(std::uint32_t)), (void**)&idx_dst, D3DLOCK_DISCARD) < 0) throw std::runtime_error{ "idx_buffer->Lock error" };
		for(render::c_geometry_buffer* geometry_buffer : _compiled_geometry_data.geometry_buffers) {
			for(const render::vertex_t& vertex : geometry_buffer->vtx_buffer) {
				*vtx_dst = {
					{ vertex.pos.x, vertex.pos.y, 0.f },
					D3DCOLOR_RGBA(vertex.color.r, vertex.color.g, vertex.color.b, vertex.color.a),
					{ vertex.uv.x, vertex.uv.y }
				};

				vtx_dst++;
			}
			memcpy(idx_dst, geometry_buffer->idx_buffer.data(), geometry_buffer->idx_buffer.size() * sizeof(std::uint32_t));
			idx_dst += geometry_buffer->idx_buffer.size();
		}
		vtx_buffer->Unlock();
		idx_buffer->Unlock();
		device->SetStreamSource(0, vtx_buffer, 0, sizeof(vertex_t));
		device->SetIndices(idx_buffer);
		device->SetVertexDeclaration(vtx_declaration);
		device->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);

		setup_state();

		for(int global_vtx_offset{ }, global_idx_offset{ }; render::c_geometry_buffer* geometry_buffer : _compiled_geometry_data.geometry_buffers) {
			for(render::c_geometry_buffer::cmd_t& cmd : geometry_buffer->cmd_buffer) {
				if(auto& callback{ cmd.callbacks.at<render::e_cmd_callbacks::on_draw>() }; !callback.empty() && callback.call(cmd)) {
					setup_state();
					continue;
				}

				const RECT clip_rect{ (LONG)cmd.clip_rect.min.x, (LONG)cmd.clip_rect.min.y, (LONG)cmd.clip_rect.max.x, (LONG)cmd.clip_rect.max.y };
				device->SetTexture(0, (IDirect3DTexture9*)cmd.texture);
				device->SetScissorRect(&clip_rect);
				device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, cmd.vtx_offset + global_vtx_offset, 0, (UINT)geometry_buffer->vtx_buffer.size(), cmd.idx_offset + global_idx_offset, cmd.element_count / 3);
				global_idx_offset += cmd.element_count;
			}
			global_vtx_offset += geometry_buffer->vtx_buffer.size();
		}

		device->SetTransform(D3DTS_WORLD, &last_world);
		device->SetTransform(D3DTS_VIEW, &last_view);
		device->SetTransform(D3DTS_PROJECTION, &last_projection);

		d3d9_state_block->Apply();
		d3d9_state_block->Release();
	}

	void c_directx9::setup_state() {
		D3DVIEWPORT9 viewport{ 0, 0,
			render::shared::viewport.x,
			render::shared::viewport.y,
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
			device->SetTransform(D3DTS_WORLD, (D3DMATRIX*)matrix4x4_t::identity().linear_array.data());
			device->SetTransform(D3DTS_VIEW, (D3DMATRIX*)matrix4x4_t::identity().linear_array.data());
			device->SetTransform(D3DTS_PROJECTION, (D3DMATRIX*)matrix4x4_t::project_ortho(0.5f, render::shared::viewport.x + 0.5f, render::shared::viewport.y + 0.5f, 0.5f, -10000.f, 10000.f).linear_array.data());
		}
	}

	void c_directx9::create_objects() {
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

	void c_directx9::destroy_objects() {
		if(!device) return;

		if(vtx_buffer) { vtx_buffer->Release(); vtx_buffer = nullptr; }
		if(idx_buffer) { idx_buffer->Release(); idx_buffer = nullptr; }
		if(vtx_declaration) { vtx_declaration->Release(); vtx_declaration = nullptr; }
		if(font_texture) { font_texture->Release(); font_texture = nullptr; render::atlas.texture.data = nullptr; }
	}

	void c_directx9::create_fonts_texture() {
		if(render::atlas.texture.pixels_alpha8.empty()) {
			if(render::atlas.configs.empty()) render::atlas.add_font_default();
			render::atlas.build();
		}

		render::atlas.texture.get_data_as_rgba32();

		font_texture = nullptr;
		if(device->CreateTexture(render::atlas.texture.size.x, render::atlas.texture.size.y, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &font_texture, nullptr) < 0)
			throw std::runtime_error{ "cannot create font texture" };

		D3DLOCKED_RECT tex_locked_rect{ };
		if(int result{ font_texture->LockRect(0, &tex_locked_rect, nullptr, 0) }; result != D3D_OK)
			throw std::runtime_error{ std::format("lock rect error, code {}", result) };

		for(float size{ render::atlas.texture.size.x * 4 }; const int& y : std::views::iota(0, render::atlas.texture.size.y)) {
			std::memcpy((std::uint8_t*)tex_locked_rect.pBits + tex_locked_rect.Pitch * y, (std::uint8_t*)render::atlas.texture.pixels_rgba32.data() + (int)size * y, size);
		}

		font_texture->UnlockRect(0);

		render::atlas.texture.data = (void*)font_texture;
	}
}