#ifndef DATA_PROCESSING_APPLICATION_CORE_MODELS_DATASOURCE_H
#define DATA_PROCESSING_APPLICATION_CORE_MODELS_DATASOURCE_H

#include <string>
#include <vector>
#include <filesystem>
#include "dataSet.h"

namespace data_processing::core::models {

class DataSource {
public:
    DataSource() = default;
    DataSource(const std::filesystem::path& path, const std::string& format);

    const std::string& path() const;
    void setPath(const std::filesystem::path& path);

    const std::string& format() const;
    void setFormat(const std::string& format);
private:
    std::filesystem::path path_;
    std::string format_;
};

} // namespace data_processing::core::models

#endif // DATA_PROCESSING_APPLICATION_CORE_MODELS_DATASOURCE_H