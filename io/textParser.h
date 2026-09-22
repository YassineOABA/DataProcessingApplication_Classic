#ifndef DATA_PROCESSING_APPLICATION_IO_TEXTPARSER_H
#define DATA_PROCESSING_APPLICATION_IO_TEXTPARSER_H

#include <filesystem>
#include <fstream>
#include <string>
#include <utility>
#include <vector>

namespace data_processing::io {

class TextParser {
public:
    using Line = std::string;
    using Document = std::vector<Line>;

    explicit TextParser(std::filesystem::path filePath) : filePath_(std::move(filePath)) {}

    Document parse() const;

private:
    std::filesystem::path filePath_;
};

} // namespace data_processing::io

#endif // DATA_PROCESSING_APPLICATION_IO_TEXTPARSER_H