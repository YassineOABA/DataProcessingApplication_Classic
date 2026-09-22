#include "transformer.h"

#include <sstream>
#include <stdexcept>

#include "core/models/field.h"
#include "core/models/record.h"

namespace data_processing::core::services {
namespace {

std::vector<std::string> splitCsvLine(const std::string& line, char delimiter = ',') {
    std::vector<std::string> fields;
    std::stringstream ss(line);
    std::string item;

    while (std::getline(ss, item, delimiter)) {
        fields.push_back(item);
    }

    return fields;
}

} // namespace

data_processing::core::models::DataSet CsvTransformer::transform(
    const std::vector<std::string>& input) const {
    data_processing::core::models::DataSet dataset;

    if (input.empty()) {
        return dataset;
    }

    const auto header = splitCsvLine(input.front());

    for (size_t i = 1; i < input.size(); ++i) {
        const auto values = splitCsvLine(input[i]);
        data_processing::core::models::Record record;

        for (size_t j = 0; j < header.size() && j < values.size(); ++j) {
            record.set(header[j], data_processing::core::models::Field(values[j]));
        }

        dataset.addRecord(record);
    }

    return dataset;
}

data_processing::core::models::DataSet JsonTransformer::transform(
    const nlohmann::json& input) const {
    data_processing::core::models::DataSet dataset;

    if (!input.is_array()) {
        throw std::runtime_error("JSON transformer expects an array of objects");
    }

    for (const auto& item : input) {
        if (!item.is_object()) {
            continue;
        }

        data_processing::core::models::Record record;

        for (auto it = item.begin(); it != item.end(); ++it) {
            const std::string key = it.key();
            const auto& value = it.value();

            if (value.is_string()) {
                record.set(key, data_processing::core::models::Field(value.get<std::string>()));
            } else if (value.is_number_integer()) {
                record.set(key, data_processing::core::models::Field(value.get<int>()));
            } else if (value.is_number_float()) {
                record.set(key, data_processing::core::models::Field(value.get<double>()));
            } else if (value.is_boolean()) {
                record.set(key, data_processing::core::models::Field(value.get<bool>()));
            } else {
                record.set(key, data_processing::core::models::Field(value.dump()));
            }
        }

        dataset.addRecord(record);
    }

    return dataset;
}

data_processing::core::models::DataSet TextTransformer::transform(
    const std::vector<std::string>& input) const {
    data_processing::core::models::DataSet dataset;

    for (const auto& line : input) {
        data_processing::core::models::Record record;
        record.set("line", data_processing::core::models::Field(line));
        dataset.addRecord(record);
    }

    return dataset;
}

} // namespace data_processing::core::services