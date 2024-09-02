#pragma once
#include <unordered_map>

namespace ntl::render::directx11 {
    template <typename object_t, typename object_desc_t>
    class i_state_instances_stack {
    protected:
        std::unordered_map<std::size_t, object_t*> instances{ };

    public:
        i_state_instances_stack() { }
        virtual ~i_state_instances_stack() {
            for(auto& [hash, state] : instances)
                if(state) { state->Release(); state = nullptr; }
        }

    protected:
        virtual object_t* instance_object(std::size_t desc_hash, const object_desc_t& desc) = 0;

    public:
        object_t* instance(std::size_t desc_hash, const object_desc_t& desc) {
            if(auto finded = instances.find(desc_hash); finded != instances.end())
                return finded->second;

            return instance_object(desc_hash, desc);
        }
    };

    template <typename interface_t, typename object_t, typename object_desc_t>
    class i_state_machine : public interface_t {
    public:
        using state_instances_stack_t = i_state_instances_stack<object_t, object_desc_t>;

    private:
        bool unhandled_changes{ };
        std::size_t desc_hash{ };
        object_desc_t object_desc{ };
        object_t* object{ };

    public:
        i_state_machine() { }
        i_state_machine(object_desc_t _object_desc) : object_desc(_object_desc) { }

    protected:
        virtual state_instances_stack_t& get_stack() const = 0;
        virtual i_state_machine* get_previous_state() const = 0;

        virtual void set_state_object(object_t* object, const object_desc_t& desc) = 0;
        virtual void append_state_to_desc(object_desc_t& desc) = 0;

    public:
        virtual void use() override {
            if(interface_t::overridden_hash == 0) return;
            bool recreate_object = unhandled_changes || object == nullptr;

            if(i_state_machine* previous_state = get_previous_state()) {
                if(previous_state->desc_hash != desc_hash) {
                    unlock();
                    object_desc = previous_state->object_desc;
                    lock();

                    recreate_object = true;
                } else if(previous_state->overridden_hash == interface_t::overridden_hash)
                    return;
            }

            if(recreate_object) object = get_stack().instance(desc_hash, object_desc);
            set_state_object(object, object_desc);
        }

    public:
        virtual void lock() override {
            interface_t::lock();

            append_state_to_desc(object_desc);

            std::size_t new_desc_hash = std::hash<object_desc_t>{ }(object_desc);
            unhandled_changes = new_desc_hash != desc_hash;
            desc_hash = new_desc_hash;
        }

        virtual void unlock() override {
            interface_t::unlock();
            desc_hash = std::hash<object_desc_t>{ }(object_desc);
        }
    };
}