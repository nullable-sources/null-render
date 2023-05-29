#pragma once
#include <renderer/renderer.h>
#include <backend/internal/mesh.h>

namespace null::render::backend::directx11 {
	class c_mesh : public backend::c_mesh {
	public:
		ID3D11InputLayout* input_layout{ };
		ID3D11Buffer* vertex_buffer{ }, *index_buffer{ };

	public:
		void on_create() override;
		void on_destroy() override;

	public:
		void compile() override;

		void set() override;
	};
}