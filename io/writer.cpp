#include "writer.h"

#include <iostream>
#include <stdexcept>

namespace data_processing::io {

FileWriter::FileWriter(const std::filesystem::path& path, bool append)
    : path_(path) {
    const auto openMode = append ? (std::ios::out | std::ios::app) : (std::ios::out | std::ios::trunc);
    file_.open(path_, openMode);

    if (!file_.is_open()) {
        throw std::runtime_error("Failed to open file: " + path_.string());
    }
}

FileWriter::~FileWriter() {
    if (file_.is_open()) {
        file_.flush();
        file_.close();
    }
}

void FileWriter::write(const std::string& data) {
    file_ << data << std::endl;
}

void ConsoleWriter::write(const std::string& data) {
    std::cout << data << std::endl;
}

void NetworkWriter::write(const std::string& data) {
    (void)data;
}

}  // namespace data_processing::io