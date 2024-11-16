#pragma once
#include <null-render/backend/state-pipeline/state-pipeline.h>

namespace ntl::render::opengl3 {
    template <typename state_class_t>
    class c_recursively_inherited_state {
    private:
        state_class_t* inherited_state{ };

    protected:
        void update_inheritance() { inherited_state = state_class_t::previous_state(); }

        template <typename self_t, typename field_t>
        const field_t& inherit(this self_t&& self, backend::state_field_t<field_t> state_class_t::* state) {
            backend::state_field_t<field_t>& self_state = self.*state;

            if(self.inherited_state && self_state.has_inherit())
                return self.inherited_state->inherit(state);

            return self_state.get();
        }
    };
}