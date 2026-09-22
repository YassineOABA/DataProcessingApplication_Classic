#include "config.h"

namespace data_processing::utils {

Config::Config(std::unordered_map<std::string, std::string> configMap)
    : configMap_(std::move(configMap)) {}

void Config::setValue(const std::string& key, const std::string& value) {
    configMap_[key] = value;
}

std::string Config::getValue(const std::string& key) const {
    auto it = configMap_.find(key);
    if (it != configMap_.end()) {
        return it->second;
    }
    return "";
}

std::unordered_map<std::string, std::string> Config::getAll() const {
    return configMap_;
}

} // namespace data_processing::utils