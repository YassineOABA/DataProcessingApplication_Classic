#include <gtest/gtest.h>

#include <chrono>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#include "core/services/transformer.h"
#include "io/csvParser.h"
#include "io/jsonParser.h"
#include "io/reader.h"
#include "io/textParser.h"
#include "io/writer.h"

namespace {

std::filesystem::path makeTempPath(const std::string& name) {
    const auto stamp = std::chrono::steady_clock::now().time_since_epoch().count();
    return std::filesystem::temp_directory_path() / ("dp_app_" + std::to_string(stamp) + "_" + name);
}

}  // namespace

TEST(ReaderTests, ReadFileReturnsWholeContent) {
    const auto filePath = makeTempPath("readfile.txt");
    std::ofstream output(filePath);
    output << "alpha\nbeta\ngamma";
    output.close();

    const auto content = data_processing::io::Reader::readFile(filePath);

    EXPECT_EQ(content, "alpha\nbeta\ngamma");
    std::filesystem::remove(filePath);
}

TEST(ReaderTests, ReadLinesSplitsByLine) {
    const auto filePath = makeTempPath("lines.txt");
    std::ofstream output(filePath);
    output << "one\ntwo\nthree\n";
    output.close();

    const auto lines = data_processing::io::Reader::readLines(filePath);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[0], "one");
    EXPECT_EQ(lines[1], "two");
    EXPECT_EQ(lines[2], "three");
    std::filesystem::remove(filePath);
}

TEST(ReaderTests, ReadCsvParsesRowsAndColumns) {
    const auto filePath = makeTempPath("data.csv");
    std::ofstream output(filePath);
    output << "name,age\nAlice,30\nBob,25\n";
    output.close();

    const auto table = data_processing::io::Reader::readCsv(filePath);

    ASSERT_EQ(table.size(), 3u);
    EXPECT_EQ(table[0][0], "name");
    EXPECT_EQ(table[0][1], "age");
    EXPECT_EQ(table[1][0], "Alice");
    EXPECT_EQ(table[1][1], "30");
    EXPECT_EQ(table[2][0], "Bob");
    EXPECT_EQ(table[2][1], "25");
    std::filesystem::remove(filePath);
}

TEST(ReaderTests, MissingFileThrowsRuntimeError) {
    const auto missingPath = makeTempPath("missing.txt");
    EXPECT_THROW(data_processing::io::Reader::readFile(missingPath), std::runtime_error);
}

TEST(JsonParserTests, ParseValidJsonReturnsObject) {
    const auto json = data_processing::io::JsonParser::parse(R"({"name":"Alice","age":30})");

    EXPECT_EQ(json["name"], "Alice");
    EXPECT_EQ(json["age"], 30);
}

TEST(JsonParserTests, ParseInvalidJsonThrows) {
    EXPECT_THROW(data_processing::io::JsonParser::parse("{not valid json}"), nlohmann::json::parse_error);
}

TEST(FileWriterTests, WritesTextToFile) {
    const auto filePath = makeTempPath("writer.txt");
    {
        data_processing::io::FileWriter writer(filePath);
        writer.write("hello");
    }

    std::ifstream input(filePath);
    std::string content((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
    input.close();

    EXPECT_EQ(content, "hello\n");
    std::filesystem::remove(filePath);
}

TEST(FileWriterTests, OverwritesExistingFileByDefault) {
    const auto filePath = makeTempPath("overwrite.txt");
    std::ofstream output(filePath);
    output << "old\ncontent\n";
    output.close();

    {
        data_processing::io::FileWriter writer(filePath);
        writer.write("new");
    }

    std::ifstream input(filePath);
    std::string content((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
    input.close();

    EXPECT_EQ(content, "new\n");
    std::filesystem::remove(filePath);
}

TEST(FileWriterTests, InvalidPathThrows) {
    const auto directoryPath = std::filesystem::temp_directory_path();
    EXPECT_THROW(data_processing::io::FileWriter writer(directoryPath), std::runtime_error);
}

TEST(TextParserTests, ParsesTextFileIntoDocument) {
    const auto filePath = makeTempPath("text.txt");
    std::ofstream output(filePath);
    output << "line1\nline2\nline3";
    output.close();

    data_processing::io::TextParser parser(filePath.string());
    const auto document = parser.parse();

    ASSERT_EQ(document.size(), 3u);
    EXPECT_EQ(document[0], "line1");
    EXPECT_EQ(document[1], "line2");
    EXPECT_EQ(document[2], "line3");
    std::filesystem::remove(filePath);
}

namespace {
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
}  // namespace

TEST(IntegrationTests, CsvFlowReadsParsesAndTransforms) {
    const auto filePath = makeTempPath("integration.csv");
    std::ofstream output(filePath);
    output << "name,age\nAlice,30\nBob,25\n";
    output.close();

    const auto csvTable = data_processing::io::CSVParser(filePath).parse();
    const auto csvLines = csvTableToLines(csvTable);

    data_processing::core::services::CsvTransformer transformer;
    const auto dataset = transformer.transform(csvLines);

    ASSERT_EQ(dataset.size(), 2u);
    EXPECT_EQ(dataset.records()[0].get("name").getValueAs<std::string>(), "Alice");
    EXPECT_EQ(dataset.records()[0].get("age").getValueAs<std::string>(), "30");
    EXPECT_EQ(dataset.records()[1].get("name").getValueAs<std::string>(), "Bob");
    EXPECT_EQ(dataset.records()[1].get("age").getValueAs<std::string>(), "25");
    std::filesystem::remove(filePath);
}

TEST(IntegrationTests, JsonFlowReadsParsesAndTransforms) {
    const auto filePath = makeTempPath("integration.json");
    std::ofstream output(filePath);
    output << R"([{"name":"Alice","age":30,"city":"Paris"},{"name":"Bob","age":25,"city":"London"}])";
    output.close();

    const auto rawText = data_processing::io::Reader::readFile(filePath);
    const auto json = data_processing::io::JsonParser::parse(rawText);

    data_processing::core::services::JsonTransformer transformer;
    const auto dataset = transformer.transform(json);

    ASSERT_EQ(dataset.size(), 2u);
    EXPECT_EQ(dataset.records()[0].get("name").getValueAs<std::string>(), "Alice");
    EXPECT_EQ(dataset.records()[0].get("age").getValueAs<int>(), 30);
    EXPECT_EQ(dataset.records()[0].get("city").getValueAs<std::string>(), "Paris");
    EXPECT_EQ(dataset.records()[1].get("name").getValueAs<std::string>(), "Bob");
    EXPECT_EQ(dataset.records()[1].get("age").getValueAs<int>(), 25);
    std::filesystem::remove(filePath);
}

TEST(IntegrationTests, TextFlowReadsParsesAndTransforms) {
    const auto filePath = makeTempPath("integration.txt");
    std::ofstream output(filePath);
    output << "alpha\nbeta\ngamma\n";
    output.close();

    const auto document = data_processing::io::TextParser(filePath).parse();

    data_processing::core::services::TextTransformer transformer;
    const auto dataset = transformer.transform(document);

    ASSERT_EQ(dataset.size(), 3u);
    EXPECT_EQ(dataset.records()[0].get("line").getValueAs<std::string>(), "alpha");
    EXPECT_EQ(dataset.records()[1].get("line").getValueAs<std::string>(), "beta");
    EXPECT_EQ(dataset.records()[2].get("line").getValueAs<std::string>(), "gamma");
    std::filesystem::remove(filePath);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

