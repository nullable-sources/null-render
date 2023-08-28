#include "internal/mesh/mesh.h"

namespace null::render::directx9 {
	void c_mesh::on_create() {
		if(vertex_declaration) return;

		constexpr D3DVERTEXELEMENT9 elements[]{
				{ 0, offsetof(directx_vertex_t, pos), D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
				{ 0, offsetof(directx_vertex_t, uv), D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
				{ 0, offsetof(directx_vertex_t, color), D3DDECLTYPE_UBYTE4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
				D3DDECL_END()
		};

		if(auto result{ shared.device->CreateVertexDeclaration(elements, &vertex_declaration) }; FAILED(result))
			utils::logger(utils::e_log_type::error, "cant create vertex input layout, return code {}.", result);
	}

	void c_mesh::on_destroy() {
		if(index_buffer) { index_buffer->Release(); index_buffer = nullptr; }
		if(vertex_buffer) { vertex_buffer->Release(); vertex_buffer = nullptr; }
		if(vertex_declaration) { vertex_declaration->Release(); vertex_declaration = nullptr; }
	}

	void c_mesh::compile() {
		static int vtx_buffer_size{ 5000 }, idx_buffer_size{ 10000 };
		if(!vertex_buffer || vtx_buffer_size < geometry_buffer.vertex_buffer.size()) {
			if(vertex_buffer) { vertex_buffer->Release(); vertex_buffer = nullptr; }
			vtx_buffer_size = geometry_buffer.vertex_buffer.size() + 5000;
			if(auto result{ shared.device->CreateVertexBuffer(vtx_buffer_size * sizeof(directx_vertex_t), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, 0, D3DPOOL_DEFAULT, &vertex_buffer, nullptr) }; FAILED(result))
				utils::logger(utils::e_log_type::error, "cant create vertex buffer, return code {}.", result);
		}

		if(!index_buffer || idx_buffer_size < geometry_buffer.index_buffer.size()) {
			if(index_buffer) { index_buffer->Release(); index_buffer = nullptr; }
			idx_buffer_size = geometry_buffer.index_buffer.size() + 10000;
			if(auto result{ shared.device->CreateIndexBuffer(idx_buffer_size * sizeof(backend::index_t), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_DEFAULT, &index_buffer, nullptr) }; FAILED(result))
				utils::logger(utils::e_log_type::error, "cant create index buffer, return code {}.", result);
		}

		directx_vertex_t* vertex_dst{ };
		backend::index_t* index_dst{ };
		if(auto result{ vertex_buffer->Lock(0, (UINT)(geometry_buffer.vertex_buffer.size() * sizeof(directx_vertex_t)), (void**)&vertex_dst, D3DLOCK_DISCARD) }; FAILED(result))
			utils::logger(utils::e_log_type::error, "cant lock vertex buffer, return code {}.", result);
		if(auto result{ index_buffer->Lock(0, (UINT)(geometry_buffer.index_buffer.size() * sizeof(backend::index_t)), (void**)&index_dst, D3DLOCK_DISCARD) }; FAILED(result))
			utils::logger(utils::e_log_type::error, "cant lock index buffer, return code {}.", result);

		std::ranges::transform(geometry_buffer.vertex_buffer, vertex_dst, [](const backend::vertex_t& vertex) { return directx_vertex_t{ vertex.pos, vertex.uv, vertex.color.cast<byte>() }; });
		std::ranges::move(geometry_buffer.index_buffer, index_dst);

		vertex_buffer->Unlock();
		index_buffer->Unlock();
	}

	void c_mesh::set() {
		shared.device->SetStreamSource(0, vertex_buffer, 0, sizeof(directx_vertex_t));
		shared.device->SetIndices(index_buffer);
		shared.device->SetVertexDeclaration(vertex_declaration);
	}
}