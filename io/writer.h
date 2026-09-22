#ifndef DATA_PROCESSING_APPLICATION_IO_WRITER_H
#define DATA_PROCESSING_APPLICATION_IO_WRITER_H

#include <string>
#include <filesystem>
#include <fstream>

namespace data_processing::io {
/**
 * @brief An abstract base class for writing data to various outputs.
 */
class Writer {
public:
    virtual ~Writer() = default;
    virtual void write(const std::string& data) = 0;
};

/**
 * @brief A writer that writes data to a file.
 */
class FileWriter : public Writer {
public:
    explicit FileWriter(const std::filesystem::path& path, bool append = false);
    ~FileWriter() override;
    void write(const std::string& data) override;

private:
    std::filesystem::path path_;
    std::ofstream file_;
};

/**
 * @brief A writer that writes data to the console.
 */
class ConsoleWriter : public Writer {
public:
    void write(const std::string& data) override;
};

/**
 * @brief A writer that sends data over the network.
 */
class NetworkWriter : public Writer {
public:
    void write(const std::string& data) override;
};

}  // namespace data_processing::io

#endif // DATA_PROCESSING_APPLICATION_IO_WRITER_H