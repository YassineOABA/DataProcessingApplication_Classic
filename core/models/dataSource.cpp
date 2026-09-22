#include "dataSource.h"

namespace data_processing::core::models {

DataSource::DataSource(const std::filesystem::path& path, const std::string& format):
    path_(path), format_(format) {
}

const std::filesystem::path& DataSource::path() const {
    return path_;
}

void DataSource::setPath(const std::filesystem::path& path) {
    path_ = path;
}

const std::string& DataSource::format() const {
    return format_;
}

void DataSource::setFormat(const std::string& format) {
    format_ = format;
}

} // namespace data_processing::core::models