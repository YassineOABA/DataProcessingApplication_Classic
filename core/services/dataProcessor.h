#ifndef DATA_PROCESSING_APPLICATION_CORE_SERVICES_DATAPROCESSOR_H
#define DATA_PROCESSING_APPLICATION_CORE_SERVICES_DATAPROCESSOR_H

#include <string>
#include <vector>
#include "core/models/record.h"

namespace data_processing::core::services {

class DataProcessor {
public:
    virtual ~DataProcessor() = default;
    virtual void process(data_processing::core::models::Record& record) = 0;
    virtual std::string name() const = 0;
};


} // namespace data_processing::core::services

#endif // DATA_PROCESSING_APPLICATION_CORE_SERVICES_DATAPROCESSOR_H