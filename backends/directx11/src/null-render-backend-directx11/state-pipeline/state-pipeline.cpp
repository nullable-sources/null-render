#include "state-pipeline.h"

namespace ntl::render::directx11 {
    void c_state_pipeline::setup_state() {
        D3D11_VIEWPORT viewport{ 0, 0,
            render::shared::viewport.x,
            render::shared::viewport.y,
            0, 1
        };
        shared.context->RSSetViewports(1, &viewport);

        i_state_pipeline::setup_state();
    }

    void c_state_pipeline::save_state() {
        shared.context->OMGetRenderTargets(1, &saved_state.render_target_view, &saved_state.depth_stencil_view);

        saved_state.scissor_rects_count = saved_state.viewports_count = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
        shared.context->RSGetScissorRects(&saved_state.scissor_rects_count, saved_state.scissor_rects);
        shared.context->RSGetViewports(&saved_state.viewports_count, saved_state.viewports);
        shared.context->RSGetState(&saved_state.rasterizer_state);
        shared.context->OMGetBlendState(&saved_state.blend_state, saved_state.blend_factor, &saved_state.sample_mask);
        shared.context->OMGetDepthStencilState(&saved_state.depth_stencil_state, &saved_state.stencil_ref);
        shared.context->PSGetShaderResources(0, 1, &saved_state.shader_resource);
        shared.context->PSGetSamplers(0, 1, &saved_state.sampler);
        saved_state.pixel_shader_instances_count = saved_state.vertex_shader_instances_count = saved_state.geometry_shader_instances_count = 256;
        shared.context->PSGetShader(&saved_state.pixel_shader, saved_state.pixel_shader_instances, &saved_state.pixel_shader_instances_count);
        shared.context->PSGetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, saved_state.pixel_shader_constant_buffer);
        shared.context->VSGetShader(&saved_state.vertex_shader, saved_state.vertex_shader_instances, &saved_state.vertex_shader_instances_count);
        shared.context->VSGetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, saved_state.vertex_shader_constant_buffer);
        shared.context->GSGetShader(&saved_state.geometry_shader, saved_state.geometry_shader_instances, &saved_state.geometry_shader_instances_count);

        shared.context->IAGetPrimitiveTopology(&saved_state.primitive_topology);
        shared.context->IAGetIndexBuffer(&saved_state.index_buffer, &saved_state.index_buffer_format, &saved_state.index_buffer_offset);
        shared.context->IAGetVertexBuffers(0, 1, &saved_state.vertex_buffer, &saved_state.vertex_buffer_stride, &saved_state.vertex_buffer_offset);
        shared.context->IAGetInputLayout(&saved_state.input_layout);
    }

    void c_state_pipeline::restore_state() {
        i_state_pipeline::restore_state();

        shared.context->RSSetScissorRects(saved_state.scissor_rects_count, saved_state.scissor_rects);
        shared.context->RSSetViewports(saved_state.viewports_count, saved_state.viewports);
        shared.context->OMSetDepthStencilState(saved_state.depth_stencil_state, saved_state.stencil_ref);
        if(saved_state.depth_stencil_state) saved_state.depth_stencil_state->Release();

        restore_framebuffer();
        restore_mesh();
        restore_shader();
        restore_texture();
        restore_rasterizer();
        restore_blend();

        if(saved_state.blend_state) saved_state.blend_state->Release();
        if(saved_state.render_target_view) saved_state.render_target_view->Release();
        if(saved_state.depth_stencil_view) saved_state.depth_stencil_view->Release();
        if(saved_state.rasterizer_state) saved_state.rasterizer_state->Release();
        if(saved_state.index_buffer) saved_state.index_buffer->Release();
        if(saved_state.vertex_buffer) saved_state.vertex_buffer->Release();
        if(saved_state.input_layout) saved_state.input_layout->Release();
        if(saved_state.sampler) saved_state.sampler->Release();
        if(saved_state.pixel_shader) saved_state.pixel_shader->Release();
        for(std::uint32_t i : std::views::iota(0u, saved_state.pixel_shader_instances_count)) if(saved_state.pixel_shader_instances[i]) saved_state.pixel_shader_instances[i]->Release();
        if(saved_state.vertex_shader) saved_state.vertex_shader->Release();
        for(std::uint32_t i : std::views::iota(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT)) if(saved_state.pixel_shader_constant_buffer[i]) saved_state.pixel_shader_constant_buffer[i]->Release();
        for(std::uint32_t i : std::views::iota(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT)) if(saved_state.vertex_shader_constant_buffer[i]) saved_state.vertex_shader_constant_buffer[i]->Release();
        if(saved_state.geometry_shader) saved_state.geometry_shader->Release();
        for(std::uint32_t i : std::views::iota(0u, saved_state.vertex_shader_instances_count)) if(saved_state.vertex_shader_instances[i]) saved_state.vertex_shader_instances[i]->Release();
        if(saved_state.shader_resource) saved_state.shader_resource->Release();
    }

    void c_state_pipeline::restore_framebuffer() {
        if(saved_state.render_target_view) {
            shared.context->OMSetRenderTargets(1, &saved_state.render_target_view, saved_state.depth_stencil_view);
        } else {
            backend::rendering_buffer->use();
        }
    }

    void c_state_pipeline::restore_mesh() {
        shared.context->IASetPrimitiveTopology(saved_state.primitive_topology);
        shared.context->IASetIndexBuffer(saved_state.index_buffer, saved_state.index_buffer_format, saved_state.index_buffer_offset);
        shared.context->IASetVertexBuffers(0, 1, &saved_state.vertex_buffer, &saved_state.vertex_buffer_stride, &saved_state.vertex_buffer_offset);
        shared.context->IASetInputLayout(saved_state.input_layout);
    }

    void c_state_pipeline::restore_shader() {
        shared.context->PSSetSamplers(0, 1, &saved_state.sampler);
        shared.context->PSSetShader(saved_state.pixel_shader, saved_state.pixel_shader_instances, saved_state.pixel_shader_instances_count);
        shared.context->PSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, saved_state.pixel_shader_constant_buffer);
        shared.context->VSSetShader(saved_state.vertex_shader, saved_state.vertex_shader_instances, saved_state.vertex_shader_instances_count);
        shared.context->VSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, saved_state.vertex_shader_constant_buffer);
        shared.context->GSSetShader(saved_state.geometry_shader, saved_state.geometry_shader_instances, saved_state.geometry_shader_instances_count);
    }

    void c_state_pipeline::restore_texture() {
        shared.context->PSSetShaderResources(0, 1, &saved_state.shader_resource);
    }

    void c_state_pipeline::restore_rasterizer() {
        shared.context->RSSetState(saved_state.rasterizer_state);
    }
    
    void c_state_pipeline::restore_blend() {
        shared.context->OMSetBlendState(saved_state.blend_state, saved_state.blend_factor, saved_state.sample_mask);
    }
}