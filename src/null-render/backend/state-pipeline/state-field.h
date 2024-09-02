#pragma once
#include <boost/container_hash/hash.hpp>

namespace ntl::render::backend {
    template <typename field_t>
    struct state_field_t {
    private:
        bool inherited{ true };
        field_t overridden_value{ };

    public:
        state_field_t() { }
        state_field_t(const field_t& _new_value) : overridden_value(_new_value), inherited(false) { }

    public:
        const bool& has_inherit() const { return inherited; }
        const field_t& get() const { return overridden_value; }
        void set(const field_t& value) { inherited = false; overridden_value = value; }

        void to_hash(std::size_t& value_hash) const {
            if(inherited) return;
            boost::hash_combine(value_hash, overridden_value);
        }
    };
}