#include "application.h"

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include "core/models/dataSet.h"
#include "core/models/field.h"
#include "core/models/record.h"
#include "core/services/transformer.h"
#include "io/csvParser.h"
#include "io/jsonParser.h"
#include "io/reader.h"
#include "io/textParser.h"

namespace {

void printDataset(const data_processing::core::models::DataSet& dataset, const std::string& label) {
    std::cout << "\n=== " << label << " ===\n";
    if (dataset.empty()) {
        std::cout << "empty dataset\n";
        return;
    }

    for (const auto& record : dataset.records()) {
        std::cout << "Record: ";
        const auto& fields = record.getFields();
        for (const auto& [key, value] : fields) {
            std::cout << key << "=" << value.toString() << " ";
        }
        std::cout << '\n';
    }
}

std::vector<std::string> csvTableToLines(const data_processing::io::CSVParser::Table& table) {
    std::vector<std::string> lines;
    lines.reserve(table.size());

    for (const auto& row : table) {
        std::string line;
        for (size_t i = 0; i < row.size(); ++i) {
            if (i > 0) {
                line += ',';
            }
            line += row[i];
        }
        lines.push_back(line);
    }

    return lines;
}

} // namespace

Application::Application() = default;

int Application::run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    try {
        const std::filesystem::path basePath = std::filesystem::path("samples");

        const auto csvTable = data_processing::io::CSVParser(basePath / "sample_data.csv").parse();
        const auto csvLines = csvTableToLines(csvTable);

        const auto jsonText = data_processing::io::Reader::readFile(basePath / "sample_data.json");
        const auto jsonDocument = data_processing::io::JsonParser::parse(jsonText);

        const auto textLines = data_processing::io::TextParser(basePath / "sample_data.txt").parse();

        data_processing::core::services::CsvTransformer csvTransformer;
        data_processing::core::services::JsonTransformer jsonTransformer;
        data_processing::core::services::TextTransformer textTransformer;

        auto csvDataset = csvTransformer.transform(csvLines);
        auto jsonDataset = jsonTransformer.transform(jsonDocument);
        auto textDataset = textTransformer.transform(textLines);

        printDataset(csvDataset, "CSV dataset");
        printDataset(jsonDataset, "JSON dataset");
        printDataset(textDataset, "Text dataset");
    } catch (const std::exception& ex) {
        std::cerr << "Application error: " << ex.what() << '\n';
        return 1;
    }

    return 0;
}
