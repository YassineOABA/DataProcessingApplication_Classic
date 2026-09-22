#ifndef DATA_PROCESSING_APPLICATION_CORE_MODELS_RECORD_H
#define DATA_PROCESSING_APPLICATION_CORE_MODELS_RECORD_H

#include <string>
#include <vector>
#include <unordered_map>
#include "field.h"

namespace data_processing::core::models {

class Record {
public:
    Record() = default;
    explicit Record(const std::unordered_map<std::string, Field>& fields);
    const Field& get(const std::string& key) const;
    void set(const std::string& key, const Field& value);
    bool hasKey(const std::string& key) const;
    bool hasValue(const std::string& value) const;
    const std::unordered_map<std::string, Field>& getFields() const;
    std::vector<std::string> getKeys() const;

private:
    std::unordered_map<std::string, Field> fields_;
};

} // namespace data_processing::core::models

#endif // DATA_PROCESSING_APPLICATION_CORE_MODELS_RECORD_H