#ifndef DATA_PROCESSING_APPLICATION_CORE_MODELS_DATASET_H
#define DATA_PROCESSING_APPLICATION_CORE_MODELS_DATASET_H

#include <string>
#include <vector>
#include "record.h"

namespace data_processing::core::models {

class DataSet {
public:
    DataSet() = default;
    explicit DataSet(const std::vector<Record>& records);

    void addRecord(const Record& record);
    const std::vector<Record>& records() const;
    size_t size() const;
    bool empty() const;
    void clear();
    void setMetadata(const std::string& key, const std::string& value);
    std::string getMetadata(const std::string& key) const;
    const std::unordered_map<std::string, std::string>& metadata() const;

private:
    std::vector<Record> records_;
    std::unordered_map<std::string, std::string> metadata_;
};

} // namespace data_processing::core::models

#endif // DATA_PROCESSING_APPLICATION_CORE_MODELS_DATASET_H