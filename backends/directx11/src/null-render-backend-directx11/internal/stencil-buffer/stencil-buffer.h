#pragma once
#include "../../renderer/renderer.h"

namespace null::render::directx11 {
	class c_stencil_buffer : public backend::i_stencil_buffer {
	public: using i_stencil_buffer::i_stencil_buffer;
		ID3D11DepthStencilView* buffer{ };
		ID3D11Texture2D* texture{ };
		int stencil_ref{ 0 };
		backend::e_stencil_operation last_operation{ };

		ID3D11DepthStencilState* disabled_state{ }, *enabled_state{ },
			*end_write_state{ }, *set_write_state{ }, *intersect_write_state{ };

		ID3D11BlendState* disabled_rt_write{ }, *enabled{ };

	public:
		void* get_buffer() override { return (void*)buffer; }

	public:
		void create() override;
		void destroy() override;

		void clear() override;

		void set_test(bool test) override;
		void set_operation(backend::e_stencil_operation operation) override;

	public:
		bool empty() const override { return !buffer; }
	};
}