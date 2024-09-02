#include <null-sdk.h>

#include "loader.h"

namespace ntl::render {
    font_config_t& font_config_t::load_font_default(this font_config_t& self) {
        return self.load_font_from_file(std::format("{}\\fonts\\Tahoma.ttf", std::getenv("windir")));
    }

    font_config_t& font_config_t::load_font_from_resource(this font_config_t& self, std::string_view resource_name, std::string_view resource_type) {
        mem::resource_t resource = mem::resource_t(resource_name, resource_type, mem::c_module::self().pe_image).load();
        if(resource.empty()) {
            utils::logger(utils::e_log_type::warning, "failed to load \"{}\" \"{}\" resource for font.", resource_name, resource_type);
            return self;
        }

        std::string resource_data = mem::resource_cast_t<std::string>::cast(resource);
        return self.set_font_data(std::vector<std::uint8_t>(resource_data.begin(), resource_data.end()));
    }

    font_config_t& font_config_t::load_font_from_file(this font_config_t& self, std::string_view filepath) {
        std::ifstream file(filepath.data(), std::ios::in | std::ios::binary | std::ios::ate);
        if(!file.is_open()) { utils::logger(utils::e_log_type::error, "cannot open font file."); return self; }

        std::vector<std::uint8_t> font_file((std::size_t)file.tellg());
        file.seekg(0, std::ios::beg);
        file.read((char*)font_file.data(), font_file.size());
        file.close();

        return self.set_font_data(font_file);
    }

    font_config_t& font_config_t::load_artery_from_resource(this font_config_t& self, std::string_view resource_name, std::string_view resource_type) {
        mem::resource_t resource = mem::resource_t(resource_name, resource_type, mem::c_module::self().pe_image).load();
        if(resource.empty()) {
            utils::logger(utils::e_log_type::warning, "failed to load \"{}\" \"{}\" resource for font.", resource_name, resource_type);
            return self;
        }

        std::string resource_data = mem::resource_cast_t<std::string>::cast(resource);
        return self.set_artery_data(std::vector<std::uint8_t>(resource_data.begin(), resource_data.end()));
    }

    font_config_t& font_config_t::load_artery_from_file(this font_config_t& self, std::string_view filepath) {
        std::ifstream file(filepath.data(), std::ios::in | std::ios::binary | std::ios::ate);
        if(!file.is_open()) { utils::logger(utils::e_log_type::error, "cannot open font file."); return self; }

        std::vector<std::uint8_t> font_file((std::size_t)file.tellg());
        file.seekg(0, std::ios::beg);
        file.read((char*)font_file.data(), font_file.size());
        file.close();

        return self.set_font_data(font_file);
    }
}