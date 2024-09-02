#pragma once
#include "../../../renderer/renderer.h"
#include "../state-machine.h"
#include <boost/container_hash/hash.hpp>

template<>
struct std::hash<ntl::render::directx11::dx_blend_factor_desc> {
    std::size_t operator()(const ntl::render::directx11::dx_blend_factor_desc& desc) const noexcept {
        std::size_t hash{ };
        boost::hash_combine(hash, desc.RenderTarget[0].BlendEnable);
        boost::hash_combine(hash, desc.RenderTarget[0].SrcBlend);
        boost::hash_combine(hash, desc.RenderTarget[0].DestBlend);
        boost::hash_combine(hash, desc.RenderTarget[0].SrcBlendAlpha);
        boost::hash_combine(hash, desc.RenderTarget[0].DestBlendAlpha);
        boost::hash_combine(hash, desc.RenderTarget[0].RenderTargetWriteMask);
        return hash;
    }
};

namespace ntl::render::directx11 {
    class c_blend_state_instances_stack : public i_state_instances_stack<ID3D11BlendState, dx_blend_factor_desc> {
    protected:
        virtual ID3D11BlendState* instance_object(std::size_t desc_hash, const dx_blend_factor_desc& desc) override;
    };
}