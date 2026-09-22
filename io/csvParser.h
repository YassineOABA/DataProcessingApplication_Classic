#ifndef DATA_PROCESSING_APPLICATION_IO_CSVPARSER_H
#define DATA_PROCESSING_APPLICATION_IO_CSVPARSER_H

#include <filesystem>
#include <string>
#include <utility>
#include <vector>

#include "reader.h"

namespace data_processing::io {

class CSVParser {
public:
    using Row = std::vector<std::string>;
    using Table = std::vector<Row>;

    explicit CSVParser(std::filesystem::path filePath, char delimiter = ',')
        : filePath_(std::move(filePath)), delimiter_(delimiter) {}

    Table parse() const {
        return Reader::readCsv(filePath_, delimiter_);
    }

    std::vector<std::string> readHeader() const {
        const auto table = Reader::readCsv(filePath_, delimiter_);
        if (table.empty()) {
            return {};
        }
        return table.front();
    }

private:
    std::filesystem::path filePath_;
    char delimiter_;
};

} // namespace data_processing::io

#endif // DATA_PROCESSING_APPLICATION_IO_CSVPARSER_H