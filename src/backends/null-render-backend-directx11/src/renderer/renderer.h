#pragma once
#pragma comment (lib, "d3d11.lib")
#include <d3d11.h>

#include <backend/renderer/renderer.h>

namespace null::render::directx11 {
	struct shared_t {
	public:
		ID3D11Device* device{ };
		ID3D11DeviceContext* context{ };
		IDXGISwapChain* swap_chain{ };
	} inline shared{ };

	class c_renderer : public backend::i_renderer {
	public:
		struct {
			UINT scissor_rects_count{ }, viewports_count{ };
			D3D11_RECT scissor_rects[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE]{ };
			D3D11_VIEWPORT viewports[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE]{ };
			ID3D11RasterizerState* rasterizer_state{ };
			ID3D11BlendState* blend_state{ };
			FLOAT blend_factor[4]{ };
			UINT sample_mask{ };
			UINT stencil_ref{ };
			ID3D11DepthStencilState* depth_stencil_state{ };
			ID3D11ShaderResourceView* shader_resource{ };
			ID3D11SamplerState* sampler{ };
			ID3D11PixelShader* pixel_shader{ };
			ID3D11VertexShader* vertex_shader{ };
			ID3D11GeometryShader* geometry_shader{ };
			UINT pixel_shader_instances_count{ }, vertex_shader_instances_count{ }, geometry_shader_instances_count{ };
			ID3D11ClassInstance* pixel_shader_instances[256]{ }, * vertex_shader_instances[256]{ }, * geometry_shader_instances[256]{ };
			D3D11_PRIMITIVE_TOPOLOGY primitive_topology{ };
			ID3D11Buffer* index_buffer{ }, * vertex_buffer{ }, * vertex_shader_constant_buffer{ };
			UINT index_buffer_offset{ }, vertex_buffer_stride{ }, vertex_buffer_offset{ };
			DXGI_FORMAT index_buffer_format{ };
			ID3D11InputLayout* input_layout{ };
		} saved_state{ };

		struct {
			ID3D11SamplerState* sampler{ };
			ID3D11BlendState* blend{ };
			ID3D11DepthStencilState* depth_stencil{ };
		} internal_objects{ };

	public:
		matrix4x4_t get_projection_matrix() const override;

		void set_texture(void* texture) override;
		void set_clip(const rect_t<float>& rect) override;
		void draw_geometry(size_t vertex_count, size_t index_count, size_t vertex_offset, size_t index_offset) override;

		void* create_texture(const vec2_t<float>& size, void* data) override;
		void destroy_texture(void* texture) override;

		void setup_state() override;

	private:
		void save_state() override;
		void restore_state() override;

		void create_internal_objects() override;
		void destroy_internal_objects() override;

	public:
		bool empty() const override { return !shared.device; }
	};
}