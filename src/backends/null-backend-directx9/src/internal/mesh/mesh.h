#pragma once
#include <renderer/renderer.h>
#include <backend/internal/mesh.h>

namespace null::render::backend::directx9 {
	class c_mesh : public i_mesh {
	public:
		struct directx_vertex_t {
			vec2_t<float> pos{ }, uv{ };
			sdk::i_color<byte> color{ };
		};

	public:
		IDirect3DVertexDeclaration9* vertex_declaration{ };
		IDirect3DVertexBuffer9* vertex_buffer{ };
		IDirect3DIndexBuffer9* index_buffer{ };

	public:
		void create() override;
		void destroy() override;

		void compile() override;

		void set() override;
	};
}