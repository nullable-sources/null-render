#include "renderer.h"

namespace null::render::directx11 {
	matrix4x4_t c_renderer::get_projection_matrix() const {
		return matrix4x4_t::projection_ortho(0.f, render::shared::viewport.x, render::shared::viewport.y, 0.f, -10000.f, 10000.f);
	}

	void c_renderer::set_texture(void* texture) {
		shared.context->PSSetShaderResources(0, 1, (ID3D11ShaderResourceView**)&texture);
	}

	void c_renderer::set_clip(const rect_t<float>& rect) {
		const D3D11_RECT clip(rect.min.x, rect.min.y, rect.max.x, rect.max.y);
		shared.context->RSSetScissorRects(1, &clip);
	}

	void c_renderer::draw_geometry(backend::e_topology topology, size_t vertex_count, size_t index_count, size_t vertex_offset, size_t index_offset) {
		if(old_topology != topology) {
			shared.context->IASetPrimitiveTopology(topology == backend::e_topology::triangle_list ? D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST : D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
			old_topology = topology;
		}
		shared.context->DrawIndexed(index_count, index_offset, vertex_offset);
	}

	void* c_renderer::create_texture(const vec2_t<float>& size, void* data) {
		D3D11_TEXTURE2D_DESC texture_desc{
				.Width{ (std::uint32_t)size.x },
				.Height{ (std::uint32_t)size.y },
				.MipLevels{ 1 },
				.ArraySize{ 1 },
				.Format{ DXGI_FORMAT_R8G8B8A8_UNORM },
				.SampleDesc{
					.Count{ 1 }
				},
				.Usage{ D3D11_USAGE_DEFAULT },
				.BindFlags{ D3D11_BIND_SHADER_RESOURCE },
				.CPUAccessFlags{ 0 }
		};

		std::unique_ptr<std::uint8_t[]> premultiplied = premultiply_texture_alpha(size, (std::uint8_t*)data);

		ID3D11Texture2D* texture{ };
		D3D11_SUBRESOURCE_DATA subresource{
			.pSysMem{ (void*)premultiplied.get() },
			.SysMemPitch{ texture_desc.Width * 4 },
			.SysMemSlicePitch{ 0 }
		};
		if(auto result = shared.device->CreateTexture2D(&texture_desc, &subresource, &texture); FAILED(result))
			utils::logger(utils::e_log_type::error, "cant create texture2d, return code {}.", result);

		ID3D11ShaderResourceView* texture_view{ };
		D3D11_SHADER_RESOURCE_VIEW_DESC shader_resource_view_desc{
			.Format{ DXGI_FORMAT_R8G8B8A8_UNORM },
			.ViewDimension{ D3D11_SRV_DIMENSION_TEXTURE2D },
			.Texture2D{
				.MostDetailedMip{ 0 },
				.MipLevels{ texture_desc.MipLevels }
			}
		};
		if(auto result = shared.device->CreateShaderResourceView(texture, &shader_resource_view_desc, &texture_view); FAILED(result))
			utils::logger(utils::e_log_type::error, "cant create shader resource view, return code {}.", result);

		if(auto result = texture->Release(); FAILED(result))
			utils::logger(utils::e_log_type::warning, "cant release texture, return code {}.", result);

		return texture_view;
	}

	void c_renderer::destroy_texture(void* texture) {
		if(!texture) {
			utils::logger(utils::e_log_type::warning, "it is impossible to destroy the texture because it is empty.");
			return;
		}

		if(auto result = ((ID3D11ShaderResourceView*)texture)->Release(); FAILED(result))
			utils::logger(utils::e_log_type::warning, "cant release texture, return code {}.", result);
		texture = nullptr;
	}

	void c_renderer::create_internal_objects() {
		if(!internal_objects.blend) {
			D3D11_BLEND_DESC blend_desc{
				.AlphaToCoverageEnable{ false },
				.RenderTarget{
					{
						.BlendEnable{ true },
						.SrcBlend{ D3D11_BLEND_ONE  },
						.DestBlend{ D3D11_BLEND_INV_SRC_ALPHA },
						.BlendOp{ D3D11_BLEND_OP_ADD },
						.SrcBlendAlpha{ D3D11_BLEND_ONE },
						.DestBlendAlpha{ D3D11_BLEND_INV_SRC_ALPHA },
						.BlendOpAlpha{ D3D11_BLEND_OP_ADD },
						.RenderTargetWriteMask{ D3D11_COLOR_WRITE_ENABLE_ALL }
					}
				}
			};
			if(auto result = shared.device->CreateBlendState(&blend_desc, &internal_objects.blend); FAILED(result))
				utils::logger(utils::e_log_type::error, "cant create blend state, return code {}.", result);
		}

		if(!internal_objects.depth_stencil) {
			D3D11_DEPTH_STENCIL_DESC depth_stencil_desc{
				.DepthEnable{ false },
				.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL,
				.DepthFunc = D3D11_COMPARISON_ALWAYS,
				.StencilEnable = false,
				.FrontFace{
					.StencilFailOp{ D3D11_STENCIL_OP_KEEP },
					.StencilDepthFailOp{ D3D11_STENCIL_OP_KEEP },
					.StencilPassOp{ D3D11_STENCIL_OP_KEEP },
					.StencilFunc{ D3D11_COMPARISON_ALWAYS }
				},
				.BackFace {
					.StencilFailOp{ D3D11_STENCIL_OP_KEEP },
					.StencilDepthFailOp{ D3D11_STENCIL_OP_KEEP },
					.StencilPassOp{ D3D11_STENCIL_OP_KEEP },
					.StencilFunc{ D3D11_COMPARISON_ALWAYS }
				}
			};
			if(auto result = shared.device->CreateDepthStencilState(&depth_stencil_desc, &internal_objects.depth_stencil); FAILED(result))
				utils::logger(utils::e_log_type::error, "cant create depth stencil state, return code {}.", result);
		}

		if(!internal_objects.sampler) {
			D3D11_SAMPLER_DESC sampler_desc{
				.Filter{ D3D11_FILTER_MIN_MAG_MIP_LINEAR },
				.AddressU{ D3D11_TEXTURE_ADDRESS_CLAMP },
				.AddressV{ D3D11_TEXTURE_ADDRESS_CLAMP },
				.AddressW{ D3D11_TEXTURE_ADDRESS_WRAP },
				.MipLODBias{ 0.f },
				.ComparisonFunc{ D3D11_COMPARISON_ALWAYS },
				.MinLOD{ 0.f },
				.MaxLOD{ 0.f }
			};
			if(auto result = shared.device->CreateSamplerState(&sampler_desc, &internal_objects.sampler); FAILED(result))
				utils::logger(utils::e_log_type::error, "cant create sampler state, return code {}.", result);
		}

		if(!internal_objects.raster_state) {
			D3D11_RASTERIZER_DESC raster_desc{
				.FillMode{ D3D11_FILL_SOLID },
				.CullMode{ D3D11_CULL_NONE },
				.DepthClipEnable{ true },
				.ScissorEnable{ true }
			};
			if(auto result = shared.device->CreateRasterizerState(&raster_desc, &internal_objects.raster_state); FAILED(result))
				utils::logger(utils::e_log_type::error, "cant create sampler state, return code {}.", result);
		}
	}

	void c_renderer::destroy_internal_objects() {
		if(internal_objects.raster_state) { internal_objects.raster_state->Release(); internal_objects.raster_state = nullptr; }
		if(internal_objects.sampler) { internal_objects.sampler->Release(); internal_objects.sampler = nullptr; }
		if(internal_objects.blend) { internal_objects.blend->Release(); internal_objects.blend = nullptr; }
		if(internal_objects.depth_stencil) { internal_objects.depth_stencil->Release(); internal_objects.depth_stencil = nullptr; }
	}

	void c_renderer::on_setup_state() {
		shared.context->RSSetState(internal_objects.raster_state);
		shared.context->PSSetSamplers(0, 1, &internal_objects.sampler);

		constexpr float blend_factor[4]{ 0.f, 0.f, 0.f, 0.f };
		shared.context->OMSetBlendState(internal_objects.blend, blend_factor, 0xffffffff);
		shared.context->OMSetDepthStencilState(internal_objects.depth_stencil, 0);
	}
}