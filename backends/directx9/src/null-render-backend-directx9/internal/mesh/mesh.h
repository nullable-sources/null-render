#pragma once
#include "../../renderer/renderer.h"

namespace null::render::directx9 {
	class c_mesh : public backend::c_mesh {
	public:
		struct directx_vertex_t {
		public:
			vec2_t<float> pos{ }, uv{ };
			sdk::i_color<byte> color{ };
		};

	public:
		IDirect3DVertexDeclaration9* vertex_declaration{ };
		IDirect3DVertexBuffer9* vertex_buffer{ };
		IDirect3DIndexBuffer9* index_buffer{ };

	public:
		void on_create() override;
		void on_destroy() override;

	public:
		void compile() override;

		void set() override;
	};
}