#pragma once
#include "../commands.h"
#include "../../../backend/state-pipeline/state-pipeline.h"

namespace null::render {
    class c_setup_state_command : public i_command {
    public:
        static std::shared_ptr<c_setup_state_command> instance() { return std::make_shared<c_setup_state_command>(); }

    public:
        void handle() override;
    };

    class c_rasterizer_push_command : public i_command {
    public:
        static std::shared_ptr<c_rasterizer_push_command> instance() { return std::make_shared<c_rasterizer_push_command>(); }
        static std::shared_ptr<c_rasterizer_push_command> instance(backend::i_rasterizer_state* rasterizer_state) { return std::make_shared<c_rasterizer_push_command>(rasterizer_state); }
        static std::shared_ptr<c_rasterizer_push_command> instance(std::unique_ptr<backend::i_rasterizer_state>& rasterizer_state) { return std::make_shared<c_rasterizer_push_command>(rasterizer_state.get()); }

    protected:
        backend::i_rasterizer_state* rasterizer_state{ };

    public:
        c_rasterizer_push_command() { }
        c_rasterizer_push_command(backend::i_rasterizer_state* _rasterizer_state) : rasterizer_state(_rasterizer_state) { }

    public:
        void handle() override;

    public:
        void set_rasterizer_state(backend::i_rasterizer_state* _rasterizer_state) { rasterizer_state = _rasterizer_state; }
        void set_rasterizer_state(std::unique_ptr<backend::i_rasterizer_state>& _rasterizer_state) { rasterizer_state = _rasterizer_state.get(); }
    };

    class c_rasterizer_pop_command : public i_command {
    public:
        static std::shared_ptr<c_rasterizer_pop_command> instance() { return std::make_shared<c_rasterizer_pop_command>(); }

    public:
        void handle() override;
    };
}