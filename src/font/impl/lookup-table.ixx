module;
#include <vector>
#include <xstring>
export module null.render:font.lookup_table;

import null.sdk;

export namespace null::render::font::impl {
    struct lookup_table_t {
    public:
        std::vector<float> advances_x{ };
        std::vector<std::uint16_t> indexes{ };
        bool dirty{ };

    public:
        void resize(const int& new_size) {
            if(advances_x.size() != indexes.size()) utils::logger.log(utils::e_log_type::warning, "advances_x.size() != indexes.size().");
            if(new_size <= indexes.size()) return;
            advances_x.resize(new_size, -1.0f);
            indexes.resize(new_size, (std::uint16_t)-1);
        }
    };
}