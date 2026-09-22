#include "record.h"

#include <stdexcept>

namespace data_processing::core::models {

Record::Record(const std::unordered_map<std::string, Field>& fields)
    : fields_(fields) {}

const Field& Record::get(const std::string& key) const {
    auto it = fields_.find(key);
    if (it != fields_.end()) {
        return it->second;
    }
    throw std::out_of_range("Key not found: " + key);
}

void Record::set(const std::string& key, const Field& value) {
    fields_[key] = value;
}

bool Record::hasKey(const std::string& key) const {
    return fields_.find(key) != fields_.end();
}

bool Record::hasValue(const std::string& value) const {
    for (const auto& pair : fields_) {
        if (pair.second.isString() && pair.second.getValueAs<std::string>() == value) {
            return true;
        }
    }
    return false;
}

const std::unordered_map<std::string, Field>& Record::getFields() const {
    return fields_;
}

std::vector<std::string> Record::getKeys() const {
    std::vector<std::string> keys;
    keys.reserve(fields_.size());
    for (const auto& pair : fields_) {
        keys.push_back(pair.first);
    }
    return keys;
}

} // namespace data_processing::core::models