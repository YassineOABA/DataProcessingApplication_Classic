#ifndef DATA_PROCESSING_APPLICATION_IO_JSONPARSER_H
#define DATA_PROCESSING_APPLICATION_IO_JSONPARSER_H

#include <nlohmann/json.hpp>
#include <string>
#include <utility>

namespace data_processing::io {

/**
 * @brief Parses raw JSON text into a structured representation.
 * The parser is intentionally thin: it converts text to JSON without
 * performing file I/O or business-domain validation.
 */
class JsonParser final {
public:
    explicit JsonParser(std::string jsonText)
        : jsonText_(std::move(jsonText)) {}

    nlohmann::json parse() const {
        return parseJson(jsonText_);
    }

    static nlohmann::json parse(const std::string& jsonString) {
        return parseJson(jsonString);
    }

private:
    static nlohmann::json parseJson(const std::string& jsonString) {
        return nlohmann::json::parse(jsonString);
    }

    std::string jsonText_;
};

} // namespace data_processing::io

#endif // DATA_PROCESSING_APPLICATION_IO_JSONPARSER_H