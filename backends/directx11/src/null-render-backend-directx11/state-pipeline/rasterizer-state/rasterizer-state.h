#pragma once
#include <null-render/backend/state-pipeline/rasterizer-state.h>
#include "../../wrapper/state-machine/rasterizer-state-instances-stack/rasterizer-state-instances-stack.h"

namespace null::render::directx11 {
    //@note: The forced shutdown of MSAA only works for the first feature level.
    //       To use it in your projects, connect null-render-backend-directx11-feature1.
    //       Or define the NULL_RENDER_DX11_ALLOW_FEATURE_11_1 macro in the null-render-backend-directx11 project settings
    class c_rasterizer_state : public i_state_machine<backend::i_rasterizer_state, dx_rasterizer_state_t, dx_rasterizer_state_desc_t> {
    private:
        static inline c_rasterizer_state_instances_stack instances_stack{ };

    private:
        using state_machine_t = i_state_machine<backend::i_rasterizer_state, dx_rasterizer_state_t, dx_rasterizer_state_desc_t>;

    public:
        c_rasterizer_state() : i_state_machine(dx_rasterizer_state_desc_t{
            .FillMode{ D3D11_FILL_SOLID },
            .CullMode{ D3D11_CULL_NONE },
            .DepthClipEnable{ true },
            .ScissorEnable{ true },
            .MultisampleEnable{ true },
#ifdef NULL_RENDER_DX11_ALLOW_FEATURE_11_1
            .ForcedSampleCount{ 0 }
#endif
            }) { }

    protected:
        virtual state_instances_stack_t& get_stack() const override { return instances_stack; }
        virtual state_machine_t* get_previous_state() const override;

        virtual void set_state_object(dx_rasterizer_state_t* object) override { shared.context->RSSetState(object); }
        virtual void append_state_to_desc(dx_rasterizer_state_desc_t& desc) override;
    };
}