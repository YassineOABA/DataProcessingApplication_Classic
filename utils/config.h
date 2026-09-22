#ifndef DATA_PROCESSING_APPLICATION_UTILS_CONFIG_H
#define DATA_PROCESSING_APPLICATION_UTILS_CONFIG_H

#include <string>
#include <unordered_map>

namespace data_processing::utils {

class Config {
public:
    Config() = default;
    explicit Config(std::unordered_map<std::string, std::string> configMap);
    void setValue(const std::string& key, const std::string& value);
    std::string getValue(const std::string& key) const;
    std::unordered_map<std::string, std::string> getAll() const;
private:
    std::unordered_map<std::string, std::string> configMap_;
};

} // namespace data_processing::utils

#endif // DATA_PROCESSING_APPLICATION_UTILS_CONFIG_H