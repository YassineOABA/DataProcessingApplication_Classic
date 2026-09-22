#ifndef DATA_PROCESSING_APPLICATION_CORE_SERVICES_TRANSFORMER_H
#define DATA_PROCESSING_APPLICATION_CORE_SERVICES_TRANSFORMER_H

#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "core/models/dataSet.h"

namespace data_processing::core::services {

class Transformer {
public:
    virtual ~Transformer() = default;
    virtual std::string name() const = 0;
};

class CsvTransformer : public Transformer {
public:
    data_processing::core::models::DataSet transform(
        const std::vector<std::string>& input) const;

    std::string name() const override {
        return "CsvTransformer";
    }
};

class JsonTransformer : public Transformer {
public:
    data_processing::core::models::DataSet transform(
        const nlohmann::json& input) const;

    std::string name() const override {
        return "JsonTransformer";
    }
};

class TextTransformer : public Transformer {
public:
    data_processing::core::models::DataSet transform(
        const std::vector<std::string>& input) const;

    std::string name() const override {
        return "TextTransformer";
    }
};

} // namespace data_processing::core::services

#endif // DATA_PROCESSING_APPLICATION_CORE_SERVICES_TRANSFORMER_H
