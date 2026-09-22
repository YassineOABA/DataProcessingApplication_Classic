#include "dataSet.h"

namespace data_processing::core::models {

DataSet::DataSet(const std::vector<Record>& records): records_(std::move(records)) {
}

void DataSet::addRecord(const Record& record) {
    records_.push_back(record);
}

const std::vector<Record>& DataSet::records() const {
    return records_;
}

size_t DataSet::size() const {
    return records_.size();
}

bool DataSet::empty() const {
    return records_.empty();
}

void DataSet::clear() {
    records_.clear();
}

void DataSet::setMetadata(const std::string& key, const std::string& value) {
    metadata_[key] = value;
}

std::string DataSet::getMetadata(const std::string& key) const {
    auto it = metadata_.find(key);
    if (it != metadata_.end()) {
        return it->second;
    }
    return "";
}

const std::unordered_map<std::string, std::string>& DataSet::metadata() const {
    return metadata_;
}

} // namespace data_processing::core::models