#pragma once
#include "../../../renderer/renderer.h"
#include "../state-machine.h"
#include <boost/container_hash/hash.hpp>

template<>
struct std::hash<ntl::render::directx11::dx_stencil_desc> {
    std::size_t operator()(const ntl::render::directx11::dx_stencil_desc& desc) const noexcept {
        std::size_t hash{ };
        boost::hash_combine(hash, desc.DepthEnable);
        boost::hash_combine(hash, desc.StencilEnable);
        boost::hash_combine(hash, desc.BackFace.StencilFailOp);
        boost::hash_combine(hash, desc.BackFace.StencilDepthFailOp);
        boost::hash_combine(hash, desc.BackFace.StencilPassOp);
        boost::hash_combine(hash, desc.BackFace.StencilFunc);
        return hash;
    }
};

namespace ntl::render::directx11 {
    class c_stencil_state_instances_stack : public i_state_instances_stack<ID3D11DepthStencilState, dx_stencil_desc> {
    protected:
        virtual ID3D11DepthStencilState* instance_object(std::size_t desc_hash, const dx_stencil_desc& desc) override;
    };
}