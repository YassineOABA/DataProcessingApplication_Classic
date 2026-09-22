#ifndef DATA_PROCESSING_APPLICATION_IO_READER_H
#define DATA_PROCESSING_APPLICATION_IO_READER_H

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <filesystem>

namespace data_processing::io {

/**
 * @brief A utility class for reading data from files.
 */
class Reader final {
public:
    using LineList = std::vector<std::string>;
    using CsvRow = std::vector<std::string>;
    using CsvTable = std::vector<CsvRow>;

    Reader() = delete;

    /**
     * @brief Reads the entire content of a file into a string.
     * @param filePath The path to the file to read.
     * @return The content of the file as a string.
     */
    static std::string readFile(const std::filesystem::path& filePath) {
        std::ifstream input(filePath);
        if (!input) {
            throw std::runtime_error("Failed to open file: " + filePath.string());
        }

        std::ostringstream buffer;
        buffer << input.rdbuf();
        return buffer.str();
    }

    /**
     * @brief Reads all lines from a file into a vector of strings.
     * @param filePath The path to the file to read.
     * @return A vector containing each line from the file.
     */
    static LineList readLines(const std::filesystem::path& filePath) {
        std::ifstream input(filePath);
        if (!input) {
            throw std::runtime_error("Failed to open file: " + filePath.string());
        }

        LineList lines;
        std::string line;
        while (std::getline(input, line)) {
            lines.push_back(line);
        }

        return lines;
    }

    /**
     * @brief Reads a CSV file into a table structure.
     * @param filePath The path to the CSV file to read.
     * @param delimiter The character used to separate fields in the CSV file (default is comma).
     * @return A table structure containing the CSV data.
     */
    static CsvTable readCsv(const std::filesystem::path& filePath, char delimiter = ',') {
        std::ifstream input(filePath);
        if (!input) {
            throw std::runtime_error("Failed to open file: " + filePath.string());
        }

        CsvTable table;
        std::string line;
        while (std::getline(input, line)) {
            if (line.empty()) {
                continue;
            }

            table.push_back(splitCsvLine(line, delimiter));
        }

        return table;
    }

private:
    /**
     * @brief Splits a CSV line into individual fields based on the specified delimiter.
     * @param line The CSV line to split.
     * @param delimiter The character used to separate fields in the CSV line.
     * @return A vector containing the individual fields from the CSV line.
     */
    static CsvRow splitCsvLine(const std::string& line, char delimiter) {
        CsvRow row;
        std::string field;
        std::stringstream stream(line);

        while (std::getline(stream, field, delimiter)) {
            row.push_back(field);
        }

        return row;
    }
};

}  // namespace data_processing::io

#endif  // DATA_PROCESSING_APPLICATION_IO_READER_H
