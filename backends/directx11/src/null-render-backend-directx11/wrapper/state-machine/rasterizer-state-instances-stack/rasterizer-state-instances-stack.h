#pragma once
#include "../../../renderer/renderer.h"
#include "../state-machine.h"
#include <boost/container_hash/hash.hpp>

template<>
struct std::hash<null::render::directx11::dx_rasterizer_state_desc_t> {
    std::size_t operator()(const null::render::directx11::dx_rasterizer_state_desc_t& desc) const noexcept {
        std::size_t hash{ };
        boost::hash_combine(hash, desc.ScissorEnable);
        boost::hash_combine(hash, desc.MultisampleEnable);
#ifdef NULL_RENDER_DX11_ALLOW_FEATURE_11_1
        boost::hash_combine(hash, desc.ForcedSampleCount);
#endif
        return hash;
    }
};

namespace null::render::directx11 {
    class c_rasterizer_state_instances_stack : public i_state_instances_stack<dx_rasterizer_state_t, dx_rasterizer_state_desc_t> {
    protected:
        virtual dx_rasterizer_state_t* instance_object(std::size_t desc_hash, const dx_rasterizer_state_desc_t& desc) override;
    };
}