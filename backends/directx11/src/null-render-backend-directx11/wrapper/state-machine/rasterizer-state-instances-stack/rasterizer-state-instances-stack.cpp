#include "rasterizer-state-instances-stack.h"

namespace null::render::directx11 {
    dx_rasterizer_state_t* c_rasterizer_state_instances_stack::instance_object(std::size_t desc_hash, const dx_rasterizer_state_desc_t& desc) {
        dx_rasterizer_state_t** object = &instances[desc_hash];
#ifdef NULL_RENDER_DX11_ALLOW_FEATURE_11_1
        if(auto result = null::render::directx11::shared.device->CreateRasterizerState1(&desc, object); FAILED(result))
            utils::logger(utils::e_log_type::error, "failed to create rasterizer state1, error code: {}", result);
#else
        if(auto result = null::render::directx11::shared.device->CreateRasterizerState(&desc, object); FAILED(result))
            utils::logger(utils::e_log_type::error, "failed to create rasterizer state, error code: {}", result);
#endif
        return *object;
    }
}